from prime import get_prime_factors
from bezout import get_gcd_bezout

'''
Decode a message using private and public keys
'''
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

'''
Actual function that will crack a message
the list of primes needs to be pre-processed
'''
def crack_rsa(message, public_key, primes):
  e, n = public_key

  # p x q = n
  p, q = get_prime_factors(n, primes)
  print("Prime factors of {} are: {} x {}".format(n, p, q))

  # f(n)
  fn = (p -1) * (q - 1)

  # get gcd, d and f (d being the private key that's all we care about)
  gcd, d, f = get_gcd_bezout(e, fn)

  # ensure d isn't negative
  if (d < 0):
    d = d % fn

  public_key = [e, n]
  private_key = d
  return decode(message, public_key, private_key)