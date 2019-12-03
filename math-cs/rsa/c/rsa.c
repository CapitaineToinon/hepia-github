#include "rsa.h"

/**
 * Encode a u_int64_t using an RSA public key
 */ 
u_int64_t encode(u_int64_t message, u_int64_t e, u_int64_t n)
{
  u_int64_t r = 1, b = message;

  while (e > 0)
  {
    if (e % 2 == 1)
    {
      r = (r * b) % n;
    }
    b = (b * b) % n;
    e = e / 2;
  }

  return r;
}

/**
 * Decode a u_int64_t encrypted message using an RSA public and private key
 */ 
u_int64_t decode(u_int64_t message, u_int64_t d, u_int64_t n)
{
  return encode(message, d, n);
}

/**
 * Crack RSA using only the public key.
 */ 
u_int64_t *crack_groups(u_int64_t *groups, u_int64_t length, u_int64_t e, u_int64_t n)
{
  // 1. Get all the primes numbers from 2 to the square root of n
  u_int64_t size = 0;
  u_int64_t max = (u_int64_t)floor(sqrt(n));
  u_int64_t *flags = sieve_of_eratosthenes(max, &size);

  // 2. get the prime factors of n
  u_int64_t p, q;
  get_prime_factors(flags, n, &p, &q);

  // 3. f(n)
  u_int64_t fn = (p - 1) * (q - 1);

  // 4. get the gcd and bezout numbers
  u_int64_t gcd;
  int64_t d, f;
  get_gcd_bezout(e, fn, &gcd, &d, &f);

  // 5. ensure d isn't negative
  if (d < 0)
    d = d % fn;

  // 6. decode each groups
  u_int64_t *decoded = malloc(length * sizeof(u_int64_t));
  for (u_int64_t i = 0; i < length; i++)
  {
    decoded[i] = decode(groups[i], d, n);
  }
  
  return decoded;
}