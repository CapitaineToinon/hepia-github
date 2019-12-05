#include "prime.h"

/**
 * Generate prime numbers using the Sieve Of Eratosthenes algorithm.
 * In order to save memory, it is using 64 bits numbers and using each bit
 * as a flag for a specific prime number. 
 * 
 * For example:
 *  - if the number 27 is prime, then the bit 2 ^ (27 % 64) would be 0 in the (27 / 64) uint64 in the array
 *  - if the number 75 is prime, then the bit 2 ^ (75 % 64) would be 0 in the (27 / 64) uint64 in the array
 *  
 * The returned valued is an array of uint64.
 */
u_int64_t *sieve_of_eratosthenes(u_int64_t n, u_int64_t *array_size)
{
  (*array_size) = (n / sizeof(u_int64_t));

  // Ensure we have at enough groups
  if (n % sizeof(u_int64_t) != 0)
    (*array_size) += 1;

  u_int64_t *flags = malloc((*array_size) * sizeof(u_int64_t));
  u_int64_t p = 2;
  u_int64_t flag_group, flag;

  // Only need to check numbers all to way to the square root of n
  while (p * p <= n)
  {
    flag_group = p / sizeof(u_int64_t);
    flag = p % sizeof(u_int64_t);

    if (IS_PRIME(flags[flag_group], flag))
    {
      // p is prime! Update all its factors to none primes
      // starting at p * 2 and moving by steps of 2, update
      // all those to being not prime because they're factors
      // of at least p
      for (int i = p * 2; i <= n + 1; i += p)
      {
        flag_group = i / sizeof(u_int64_t);
        flag = i % sizeof(u_int64_t);
        u_int64_t updated_group = SET_NOT_PRIME(flags[flag_group], flag);
        memcpy(&flags[flag_group], &updated_group, sizeof(updated_group));
      }
    }

    p += 1;
  }

  return flags;
}

/**
 * Returns the prime factors for the value x.
 * The list of primes first needs to be processed by the function sieve_of_eratosthenes
 */
void get_prime_factors(u_int64_t *flags, u_int64_t x, u_int64_t *p, u_int64_t *q)
{
  u_int64_t flag_group, flag;
  u_int64_t max = (u_int64_t)floor(sqrt(x));
  *p = 1;
  *q = x;

  // start from max because p and q are likely in the same range ish
  for (u_int64_t candidate = max; candidate >= 0; candidate--)
  {
    flag_group = candidate / sizeof(u_int64_t);
    flag = candidate % sizeof(u_int64_t);

    // if the bit corresponding to that number is a 0, then candidate is prime
    if (IS_PRIME(flags[flag_group], flag))
    {
      if (x % candidate == 0)
      {
        // found it!
        *p = candidate;
        *q = x / candidate;
        break;
      }
    }
  }
}

/**
 * Returns the prime factors for the value x.
 * Brute force version
 */
void get_prime_factors_force(u_int64_t x, u_int64_t *p, u_int64_t *q)
{
  u_int64_t max = (u_int64_t)floor(sqrt(x));
  *p = 1;
  *q = x;

  // start from max because p and q are likely in the same range ish
  for (u_int64_t candidate = max; candidate >= 0; candidate--)
  {
    if (x % candidate == 0)
    {
      // found it!
      *p = candidate;
      *q = x / candidate;
      break;
    }
  }
}

/**
 * Returns the prime factors for the value x.
 * Brute force version
 */
void get_prime_factors_force_odd(u_int64_t x, u_int64_t *p, u_int64_t *q)
{
  u_int64_t max = (u_int64_t)floor(sqrt(x));
  if (max % 2 == 0)
    max += 1;
  *p = 1;
  *q = x;

  // start from max because p and q are likely in the same range ish
  for (u_int64_t candidate = max; candidate >= 0; candidate -= 2)
  {
    if (x % candidate == 0)
    {
      // found it!
      *p = candidate;
      *q = x / candidate;
      break;
    }
  }
}