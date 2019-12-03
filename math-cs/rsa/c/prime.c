#include "prime.h"

/**
 * Generate prime numbers using the Sieve Of Eratosthenes algorithm.
 * In order to save memory, it is using 64 bits numbers and using each bit
 * as a flag for a specific prime number. 
 * 
 * For example, the highest bit would be a 0 if 0 is prime or a 1 if it's not.
 * Then the 2nd highest bit would be a 0 if 1 is prime or a 1 if it's not, etc. 
 * until it reaches 64 and starts the same logic on the next uint64.
 * 
 * The returned valued is an array of uint64.
 */ 
u_int64_t *SieveOfEratosthenes(u_int64_t n, u_int64_t *array_size)
{
  u_int64_t size = (n / sizeof(u_int64_t));

  // Ensure we have at least one group
  if (n % 64 != 0)
    size += 1;

  u_int64_t *flags = malloc(size * sizeof(u_int64_t));
  u_int64_t p = 2;
  u_int64_t flag_group, flag;

  for (u_int64_t i = 0; i < size; i++)
    flags[i] = 0UL;

  while (p * p <= n)
  {
    flag_group = p / 64;
    if (((flags[flag_group] >> (63 - p)) & 1) == 0)
    {
      for (int i = p * 2; i <= n + 1; i += p)
      {
        flag_group = i / 64;
        flag = i % 64;
        u_int64_t newflag = flags[flag_group] | 1UL << (63 - flag);
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
void get_prime_factors(u_int64_t *flags, __uint128_t x, u_int64_t *p, u_int64_t *q)
{
  u_int64_t flag_group, flag;
  u_int64_t max = (u_int64_t)floor(sqrt(x));
  *p = 1;
  *q = x;

  for (__uint128_t candidate = max; candidate >= 0; candidate--)
  {
    flag_group = candidate / 64;
    flag = candidate % 64;

    // if the bit corresponding to that number is a 0, then candidate is prime
    if (((flags[flag_group] >> (63 - candidate)) & 1) == 0)
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