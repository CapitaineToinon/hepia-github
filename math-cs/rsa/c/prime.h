#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

u_int64_t *SieveOfEratosthenes(u_int64_t n, u_int64_t *array_size);
void get_prime_factors(u_int64_t *flags, u_int64_t x, u_int64_t *p, u_int64_t *q);