'''
Antoine Sutter
antoinesutter@gmail.com
'''
import matplotlib.pyplot as plt
import wave as w
import numpy
from functools import reduce
from math import pi, sin, cos, sqrt, acos
from helpers import array_to_file, open_file

INPUT = './'
OUTPUT = './'

def open_wav(name):
    '''Opens a wave file by its name, from the INPUT folder'''
    return open_file("{}/{}.wav".format(INPUT, name))

def save_wav(array, name):
    '''Saves a wave file by its name, in the OUTPUT folder'''
    return array_to_file(array, "{}/{}.wav".format(OUTPUT, name))

def boost_signal(results, factor=2):
    '''Boost a signal by a specific factor'''
    return [r * factor for i, r in enumerate(results)]

def clamp(n, minn=-32767, maxn=32767):
    '''Clamps a number between minn and maxn value'''
    return max(min(maxn, n), minn)

def filter_signal(results, threshold):
    return [
        0j if (numpy.real(r) < -threshold or numpy.real(r) > threshold) else r
        for i, r in enumerate(results)
    ]

def filter_frequency(results, min, max):
    return [
        0j if i < min or i > max else r
        for i, r in enumerate(results)
    ]

def process_file(file):
    '''Process a wave file, returns a cleaned Array of processed values, ready to be saved on disk'''
    print("Starting to process file {}".format(file))
    data = numpy.fft.rfft(open_wav(file))
    data = filter_frequency(data, min=100, max=2000)
    data = filter_signal(data, threshold=2*4096)
    data = boost_signal(data, factor=100j)
    data = numpy.fft.irfft(data)
    data = map(int, data)
    data = map(clamp, data)
    return data

if __name__ == "__main__":
    '''Process all the files and joins them in one array'''
    save_wav(reduce(
        (lambda x, y: [*x, *y]), [
            process_file(f)
            for f in ['wave0', 'wave1', 'wave2', 'wave3']
        ]
    ), 'flatten')