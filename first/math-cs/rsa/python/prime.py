import itertools
import random
from math import sqrt

'''
Python program to print all primes smaller than or equal to 
n using Sieve of Eratosthenes 
'''
def SieveOfEratosthenes(n): 
  # Create a boolean array "prime[0..n]" and initialize 
  # all entries it as true. A value in prime[i] will 
  # finally be false if i is Not a prime, else true. 
  print("Allocating the array of {} elements...".format(n))
  prime = [True for i in range(n + 1)] 
  p = 2

  print("Start the aglorithm...")
  while (p * p <= n): 
    print("{}/{}".format(p * p, n))
    # If prime[p] is not changed, then it is a prime 
    if (prime[p] == True): 
      # Update all multiples of p 
      for i in range(p * 2, n + 1, p): 
        prime[i] = False
    p += 1
  prime[0]= False
  prime[1]= False

  print("Done generating, now filtering the prime numbers...")
  
  # return all prime numbers 
  primes = set()
  for p in range(n + 1): 
    if prime[p]:
      primes.add(p)

  return primes

'''
Returns the prime factors for x
the list of primes given needs to be processed first
'''
def get_prime_factors(x, primes):
  p = 1
  q = x

  for prime in primes:
    if x % prime == 0:
      p = prime
      q = x // prime
      break
  
  return [p, q]  