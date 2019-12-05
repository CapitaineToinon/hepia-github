#include <stdio.h>
#include <stdlib.h>
#include "prime.h"
#include "bezout.h"

u_int64_t encode(u_int64_t message, u_int64_t e, u_int64_t n);
u_int64_t decode(u_int64_t message, u_int64_t d, u_int64_t n);

typedef u_int64_t *(*crack_func)(u_int64_t *groups, u_int64_t length, u_int64_t e, u_int64_t n);
u_int64_t *crack_groups(u_int64_t *groups, u_int64_t length, u_int64_t e, u_int64_t n);
u_int64_t *crack_groups_force(u_int64_t *groups, u_int64_t length, u_int64_t e, u_int64_t n);
u_int64_t *crack_groups_force_odd(u_int64_t *groups, u_int64_t length, u_int64_t e, u_int64_t n);
