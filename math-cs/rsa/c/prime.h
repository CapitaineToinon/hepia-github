#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PRIME 0ULL
#define NOT_PRIME 1ULL
#define GET_FLAG(group, flag) ((group >> flag) & 1)
#define IS_PRIME(group, flag) (GET_FLAG(group, flag) == PRIME)
#define SET_FLAG(group, flag, value) (group | value << flag)
#define SET_NOT_PRIME(group, flag) (SET_FLAG(group, flag, NOT_PRIME))

u_int64_t *sieve_of_eratosthenes(u_int64_t n, u_int64_t *array_size);
void get_prime_factors(u_int64_t *flags, u_int64_t x, u_int64_t *p, u_int64_t *q);