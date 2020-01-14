import matplotlib.pyplot as plt
import wave as w
from helpers import array_to_file, file_to_array

if __name__ == "__main__":
  pitched = w.open('troissinus.wav','rb')
  pitchedArray = file_to_array(pitched)
  
  t = [k/8192 for k in range(1000)]
  plt.figure()
  plt.plot(t , pitchedArray[:1000])
  plt.show()