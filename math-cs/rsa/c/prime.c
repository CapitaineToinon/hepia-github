#include "prime.h"

/**
 * Generate prime numbers using the Sieve Of Eratosthenes algorithm.
 * In order to save memory, it is using 64 bits numbers and using each bit
 * as a flag for a specific prime number. 
 * 
 * For example, the if the number number 27 is prime, then the bit 2 ^ 27 would be 0.
 * The returned valued is an array of uint64.
 */ 
u_int64_t *SieveOfEratosthenes(u_int64_t n, u_int64_t *array_size)
{
  u_int64_t size = (n / sizeof(u_int64_t));

  // Ensure we have at least one group
  if (n % sizeof(u_int64_t) != 0)
    size += 1;

  u_int64_t *flags = malloc(size * sizeof(u_int64_t));
  u_int64_t p = 2;
  u_int64_t flag_group, flag;

  // Only need to check numbers all to way to the square root of n
  while (p * p <= n)
  {
    flag_group = p / sizeof(u_int64_t);
    if (((flags[flag_group] >> p) & 1) == 0)
    {
      // p is prime! Update all its factors to none primes
      for (int i = p * 2; i <= n + 1; i += p)
      {
        flag_group = i / 64;
        flag = i % 64;
        u_int64_t newflag = flags[flag_group] | 1UL << flag;
        memcpy(&flags[0], &newflag, sizeof(newflag));
      }
    }
    p += 1;
  }

  *array_size = size;
  return flags;
}

/**
 * Returns the prime factors for the value x.
 * The list of primes first needs to be processed by the function SieveOfEratosthenes
 */ 
void get_prime_factors(u_int64_t *flags, u_int64_t x, u_int64_t *p, u_int64_t *q)
{
  u_int64_t flag_group, flag;
  u_int64_t max = (u_int64_t)floor(sqrt(x));
  *p = 1;
  *q = x;

  for (u_int64_t candidate = max; candidate >= 0; candidate--)
  {
    flag_group = candidate / sizeof(u_int64_t);
    flag = candidate % sizeof(u_int64_t);

    // if the bit corresponding to that number is a 0, then candidate is prime
    if (((flags[flag_group] >> candidate) & 1) == 0)
    {
      if (x % candidate == 0)
      {
        *p = candidate;
        *q = x / candidate;
        break;
      }
    }
  }
}