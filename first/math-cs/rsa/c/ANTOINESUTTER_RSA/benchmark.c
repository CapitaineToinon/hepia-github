#include <stdio.h>
#include <time.h>
#include <limits.h>
#include "prime.h"
#include "math.h"

#define LOOP 10000

int main()
{
  FILE *fp;
  fp = fopen("benchmark.txt", "w+");
  clock_t start_t, end_t, total_t;
  u_int64_t *decoded;

  for (u_int64_t i = 10; i < INT32_MAX; i += 10000)
  {
    uint64_t p = 0, q = 0;
    printf("%llu\n", i);
    start_t = clock();
    for (int j = 0; j < LOOP; j++)
      (void)get_prime_factors_force_odd(i, &p, &q);
    end_t = clock();
    total_t = (end_t - start_t);

    fprintf(fp, "%llu;%lu\n", i, total_t);
  }

  fclose(fp);
  return 0;
}