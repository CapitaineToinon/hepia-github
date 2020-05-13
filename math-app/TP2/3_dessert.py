import numpy as np
import matplotlib.pyplot as plt
from helpers import list_from_file, list_to_file, polar_to_rectangular, rectangular_to_polar

def process_phase_multiplier(data, multiply_by = 2):
  result = np.copy(data)
  result_polar = list(map(rectangular_to_polar, result))

  for item in result_polar:
    radii, angle = item
    item = polar_to_rectangular(radii, angle * multiply_by)

  return result_polar

def process_rootify_amplitude(data):
  result = np.copy(data)
  pass

def process_root(data):
  pass

def process_mirror(data):
  result = np.copy(data)
  result = np.fft.fft(result)
  M = len(result)
  k = M // 4

  print("M ", M)
  print("k ", k)

  for i in range(1, k):
    result[k - i], result[k + i] = result[k + i], result[k - i]
    result[(k * 3) - i], result[(k * 3) + i] = result[(k * 3) + i], result[(k * 3) - i]

  # Group x and y to plot our message on the complex plane
  plt.plot(data, "-" , color = "#663399" , lw = 2 ) 
  plt.show()

  # Group x and y to plot our message on the complex plane
  plt.plot(result, "-" , color = "#663399" , lw = 2 ) 
  plt.show()

  return np.fft.ifft(result)



if __name__ == "__main__":  
  data = list_from_file("src/donnees.data")

  # Extract the x and y coordinates of each of our message's pixel
  x , y = [ np.real(c) for c in data ] , [ np.imag(c) for c in data ]

  # Group x and y to plot our message on the complex plane
  plt.plot(x, y, "-" , color = "#663399" , lw = 2 ) 
  plt.show()

  result = process_phase_multiplier(data)
  #result = process_mirror(data)

  x , y = [ np.real(c) for c in result ] , [ np.imag(c) for c in result ]

  # Group x and y to plot our message on the complex plane
  plt.plot(x, y, "-" , color = "#663399" , lw = 2 ) 
  plt.show()