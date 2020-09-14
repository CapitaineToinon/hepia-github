import numpy as np
import matplotlib.pyplot as plt
from functools import reduce
from helpers import open_file, list_to_wav, clamp

INPUT = './src'
OUTPUT = './dist'

def process_mirror(file):
  '''
  Finds where the audio starts and ends in the file
  and mirror all the data around its center
  '''
  data = np.fft.rfft(open_file(file))
  result = np.copy(data)

  threshold = 1000

  # Find the point at which the audio starts
  start = 0
  while (np.imag(result[start]) <= threshold and np.imag(result[start]) >= -threshold):
    start += 1

  # Find the point at which the audio ends
  end = len(result) - 1
  while (np.imag(result[end]) <= threshold and np.imag(result[end]) >= -threshold):
    end -= 1

  # Compute the center and mirror around that point
  center = abs((end - start)) // 2

  for i in range(1, center):
    swapFrom = start + (center - i)
    swapTo = start + (center + i)
    tmp = result[swapFrom]
    result[swapFrom] = result[swapTo]
    result[swapTo] = tmp

  return np.fft.irfft(result)

def process_pitch_shift(file):
  '''
  Finds by how much an audio file and been shifted
  and shifts it back
  '''
  data = np.fft.rfft(open_file(file))
  
  # Find the shift amount by exploring every value under a given threshold
  shiftAmount = 0
  while (np.imag(data[shiftAmount]) <= 500 and np.imag(data[shiftAmount]) >= -500):
    shiftAmount += 1
  
  print("Found ! The message has been shifted by ", shiftAmount)

  # Copy our input array (deep copy)
  result = np.copy(data)

  # Get the number of elements in our input list
  M = len(data)

  for i in range(M-shiftAmount):
    result[i] = data[i + shiftAmount]

  return np.fft.irfft(result)

if __name__ == "__main__":
  # Merge our filtered arrays and export them as .wav
  data = reduce(
    (lambda x, y: [*x, *y]), [
      process_mirror(f)
      for f in [
        './{}/message_part_1.wav'.format(INPUT), 
        './{}/message_part_2.wav'.format(INPUT)
      ]
    ]
  )

  data = map(int, data)
  data = map(clamp, data)

  list_to_wav(data, './{}/{}.wav'.format(OUTPUT, __file__), framerate=44100)