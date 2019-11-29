from math import pow

def decode(message, public_key, private_key):
  e, n = public_key
  d = private_key

  r, e, b = [1, d, message]
  while e > 0:
    if e % 2 == 1:
      r = (r * b) % n
    b = (b * b) % n
    e = e // 2

  return r