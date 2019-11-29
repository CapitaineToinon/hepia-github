import itertools
import random
from math import sqrt

def miller_rabin_prime_generator(base):
  if (base % 2 == 0):
    base = base + 1

  while True:
    if miller_rabin_is_prime(base):
      yield base
    
    base = base - 2

def miller_rabin_is_prime(n):
  accuracy = 8
  s = 0
  d = n - 1

  while d % 2 == 0:
    d = d >> 1
    s = s + 1

  def trial_composite(a):
    if pow(a, d, n) == 1:
      return False
    for i in range(s):
      if pow(a, 2**i * d, n) == n-1:
        return False
    return True  

  for i in range(accuracy):
    a = random.randrange(2, n)
    if trial_composite(a):
      return False

  return True

def is_prime(n, primes):
  for p in primes:
    if n % p == 0:
      return False
  return True

def generate_prime():
  primes = set()
  for n in itertools.count(2):
    if is_prime(n, primes):
      primes.add(n)
      yield n

def generate_primes(n):
  g = generate_prime()
  primes = set()
  for i in range(n):
    primes.add(next(g))
  return primes

def get_prime_factors(x):
  base = int(sqrt(x))
  g = miller_rabin_prime_generator(base)
  p = 1
  q = x

  print("Trying to break {}, start with {}".format(x, base))

  while True:
    candidate = next(g)
    if x % candidate == 0:
      p = candidate
      q = x // candidate
      return [p, q]