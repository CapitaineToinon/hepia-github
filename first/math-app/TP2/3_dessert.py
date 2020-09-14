import numpy as np
import matplotlib.pyplot as plt
import math
from cmath import phase, rect, polar
from helpers import list_from_file, list_to_file

def process_phase_multiplier(data, multiply_by = 5):
  result = np.copy(data)

  for i in range(len(data)):
    r, angle = polar(data[i])
    result[i] = rect(r, angle * 2)

  return np.fft.ifft(result)

if __name__ == "__main__":  
  data = list_from_file("src/donnees.data")
  data = np.fft.fft(data)

  # Extract the x and y coordinates of each of our message's pixel
  x , y = [ np.real(c) for c in data ] , [ np.imag(c) for c in data ]

  # Group x and y to plot our message on the complex plane
  plt.plot(x, y, color = "#663399" , lw = 2 ) 
  plt.show()

  result = process_phase_multiplier(data)

  x , y = [ np.real(c) for c in result ] , [ np.imag(c) for c in result ]

  # Group x and y to plot our message on the complex plane
  plt.plot(x, y, color = "#663399" , lw = 2 ) 
  plt.ylim(-20, 20)
  plt.show()