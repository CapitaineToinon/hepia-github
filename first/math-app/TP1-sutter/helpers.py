from struct import pack, unpack
import wave as w

def open_file(file):
  return file_to_array(w.open(file, 'rb'))

######## Transforme un objet wave_read (une seule piste mono) en liste
def file_to_array(wave_file_object ):
  length = wave_file_object.getnframes()
  list_out = []
  for t in range(length):
    aaa = unpack('h', wave_file_object.readframes(1))       
    list_out += [int(aaa[0])]
  return list_out

######## Transforme une liste en un wave_write object avec par defaut un encodage sur 16 bits et un sampling a 8192Hz
######## Attention, les valeurs dans la liste doivent etre des entiers entre -32678 et 32677.
def array_to_file(liste, name, nchannels = 1, sampwidth = 2, framerate = 8192):
  f = w.open(name ,'wb')
  f.setnchannels(nchannels)
  f.setsampwidth(sampwidth)
  f.setframerate(framerate)
  f.setcomptype('NONE', 'none')
  for k in liste:
    data = pack('h', k)
    f.writeframes(data)
  return f