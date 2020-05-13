from struct import pack, unpack
import numpy as np
import wave as w
import pickle

def open_file(file):
  '''opens a file from disk'''
  return wav_to_list(w.open(file, 'rb'))

def clamp(n, minn=-32767, maxn=32767):
  '''Clamps a number between minn and maxn value'''
  return max(min(maxn, n), minn)

def wav_to_list(wave_file_object ):
  '''Transforme un objet wave_read (une seule piste mono) en liste'''
  length = wave_file_object.getnframes()
  list_out = []
  for t in range(length):
    aaa = unpack('h', wave_file_object.readframes(1))       
    list_out += [int(aaa[0])]
  return list_out

def list_to_wav(liste, name, nchannels = 1, sampwidth = 2, framerate = 8192):
  '''
  Transforme une liste en un wave_write object avec par defaut un encodage sur 16 bits et un sampling a 8192Hz
  Attention, les valeurs dans la liste doivent etre des entiers entre -32678 et 32677.
  '''
  f = w.open(name ,'wb')
  f.setnchannels(nchannels)
  f.setsampwidth(sampwidth)
  f.setframerate(framerate)
  f.setcomptype('NONE', 'none')
  for k in liste:
    data = pack('h', k)
    f.writeframes(data)
  return f

def list_from_file(filepath):
  '''Load a list from the user's hard drive'''
  with open(filepath, "rb") as filehandle:
    data = pickle.load(filehandle)

  return data

def list_to_file(filepath, data):
  '''Save an existing list to a specific location on the user's hard drive.'''
  with open(filepath, "wb") as filehandle:
    pickle.dump( data , filehandle)


# Original author : tom10 on StackOverflow - Converting from polar to rect coordinates in python
# https://stackoverflow.com/questions/16444719/python-numpy-complex-numbers-is-there-a-function-for-polar-to-rectangular-co
def polar_to_rectangular(radii, angles):
    return [radii * np.exp(1j*angles)]

def rectangular_to_polar(x):
    return [abs(x), np.angle(x)]