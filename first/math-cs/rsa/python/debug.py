from rsa import crack_rsa
from prime import SieveOfEratosthenes
from math import sqrt

if __name__ == "__main__":
  e = 5
  n = 85
  m = 40

  limit = int(sqrt(n))

  primes = SieveOfEratosthenes(limit)
  for p in primes:
    print(p)
  print(crack_rsa(m, [e, n], primes))
