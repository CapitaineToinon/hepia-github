from rsa import *
from prime import *
from bezout import *
from math import *

def crack_rsa(message, public_key):
  e, n = public_key

  print("brute force the prime factors...")
  p, q = get_prime_factors(n)
  print("Found {0} x {1} = {2}".format(p, q, n))

  # # φ(n)
  # fn = (p -1) * (q - 1)

  # # get gcd, d and f (d being the private key that's all we care about)
  # gcd, d, f = get_gcd_bezout(e, fn)

  # # be sure d isn't negative
  # if (d < 0):
  #   d = d % fn

  # public_key = [e, n]
  # private_key = d
  # return decode(message, public_key, private_key)

if __name__ == "__main__":
  # e = 5
  # n = 85
  # crypted_message = 40
  e = 471131
  n = 19395215754271188593
  groups = [
    1213428545899287006,
    128066609881241950,
    1524544144808352117,
    5739320063815598366,
    5318541268334496831,
    10099198048166427552,
    1294578297747087516,
    5219535363202696744,
    8873085490801871488,
    9767440483961474498,
    6090604250892558071,
    7230451814471929172,
    16327068223369301731,
    3332316276070559024,
  ]

  # print("Crypted message: {0}".format(groups[0]))
  # print("Cracked message using only the public key: {0}".format(crack_rsa(groups[0], [e, n])))


  lmao = (229 * 839) 
  print(get_prime_factors(groups[0]))