#include "bezout.h"

void get_gcd_bezout(u_int64_t a, u_int64_t b, u_int64_t *gcd, int64_t *x, int64_t *y)
{
  /**
   * Important to keep track if we swapped or not
   * since it changes which coef we care about.
   * a needs to be bigger than b so we swap if it's not the case
   */ 
  bool swapped = false;
  u_int64_t *remainders = malloc(100 * sizeof(u_int64_t));
  int64_t *array_x = malloc(100 * sizeof(int64_t));
  int64_t *array_y = malloc(100 * sizeof(int64_t));

  if (b > a)
  {
    u_int64_t tmp = a;
    a = b;
    b = tmp;
    swapped = true;
  }

  // setup
  int64_t x0 = 1, y0 = 0,
          x1 = 0, y1 = 1;

  memcpy(&remainders[0], &a, sizeof(a)); // R0
  memcpy(&remainders[1], &b, sizeof(b)); // R1
  memcpy(&array_x[0], &x0, sizeof(x0));
  memcpy(&array_y[0], &y0, sizeof(y0));
  memcpy(&array_x[1], &x1, sizeof(x1));
  memcpy(&array_y[1], &y1, sizeof(y1));

  u_int64_t step = 2;

  for (;;)
  {
    int64_t coef = remainders[step - 2] / remainders[step - 1];
    u_int64_t remainder = remainders[step - 2] % remainders[step - 1];

    // We're done!
    if (remainder == 0)
    {
      int64_t out_x = array_x[step - 1];
      int64_t out_y = array_y[step - 1];

      if (swapped)
      {
        int64_t tmp = out_x;
        out_x = out_y;
        out_y = tmp;
      }

      *gcd = remainders[step - 1];
      *x = out_x;
      *y = out_y;
      return;
    }

    int64_t next_x = array_x[step - 2] - ((int64_t)coef * array_x[step - 1]);
    int64_t next_y = array_y[step - 2] - ((int64_t)coef * array_y[step - 1]);

    memcpy(&remainders[step], &remainder, sizeof(remainder));
    memcpy(&array_x[step], &next_x, sizeof(next_x));
    memcpy(&array_y[step], &next_y, sizeof(next_y));
    step += 1;
  }
}