'''
Antoine Sutter
antoinesutter@gmail.com
'''
import matplotlib.pyplot as plt
import wave as w
from functools import reduce
from math import pi, sin, cos, sqrt, acos
from helpers import array_to_file, open_file

INPUT = './INPUT'
OUTPUT = './OUTPUT'

def open_wav(name):
    '''Opens a wave file by its name, from the INPUT folder'''
    return open_file("{}/{}.wav".format(INPUT, name))

def save_wav(array, name):
    '''Saves a wave file by its name, in the OUTPUT folder'''
    return array_to_file(array, "{}/{}.wav".format(OUTPUT, name))
    open

class FourierResult:
    '''Helper class to make code more readable'''
    def __init__(self, a, b, x, o, k):
        self.a = a
        self.b = b
        self.x = x
        self.o = o
        self.k = k

def compute_ak_bk(k, array, sample=8192):
    '''Computes a and b for a given k'''
    ak = 2 * sum([
        cos(2 * pi * k * (i / sample)) * array[i] * 1/sample
        for i in range(sample)
    ])
    bk = 2 * sum([
        sin(2 * pi * k * (i / sample)) * array[i] * 1/sample
        for i in range(sample)
    ])
    return [ak, bk]


def compute_xk(ak, bk):
    '''Compute xx for a given ak and bk'''
    return sqrt((ak * ak) + (bk * bk))

def compute_ok(ak, bk, xk):
    '''Compute Ok for a given ak, bk and xk'''
    ok = abs(acos(bk / xk))
    if (ak < 0):
        ok *= -1
    return ok

def compute_k(k, array):
    '''Compute ak, bk, xk, ok for a given k'''
    ak, bk = compute_ak_bk(k, array)
    xk = compute_xk(ak, bk)
    ok = compute_ok(ak, bk, xk)
    return FourierResult(ak, bk, xk, ok, k)

def filter_signal(results, threshold):
    '''Filter any frequency that is higher than a sepcific threshold'''
    for i, result in enumerate(results):
        if (result.x > threshold):
            print("Removing x ({}) at {}Hz ".format(result.x, result.k))
            results[i].x = 0

    return results

def boost_signal(results, factor=2):
    '''Boost a signal by a specific factor'''
    for i, result in enumerate(results):
        results[i].x *= factor

    return results

def clamp(n, minn=-32767, maxn=32767):
    '''Clamps a number between minn and maxn value'''
    return max(min(maxn, n), minn)

def process_file(file):
    '''Process a wave file, returns a cleaned Array of processed values, ready to be saved on disk'''
    print("Starting to process file {}".format(file))

    # from 100Hz to 2000Hz
    hzMin = 100
    hzMax = 2000

    # Compute the signal, then filter its values, then boost the singal
    results = [
        compute_k(k, open_wav(file)) 
        for k in range(hzMin, hzMax)
    ]
    results = boost_signal(filter_signal(results, threshold=10), factor=200)

    # Build the signal
    return map(clamp, [
        int(sum([
            result.x * sin(2 * pi * result.k * t * 1/8192 + result.o)
            for result in results
        ]))
        for t in range(8192)
    ])

if __name__ == "__main__":
    '''Process all the files and joins them in one array'''
    save_wav(reduce(
        (lambda x, y: [*x, *y]), [
            process_file(f)
            for f in ['wave0', 'wave1', 'wave2', 'wave3']
        ]
    ), 'flatten')