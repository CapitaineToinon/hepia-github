#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define PI acos(-1)
#define MAX_LOOP 1000000
#define RADUIS 1

/* function headers */
static double monteCarlo(int itterations);
static double findHypotenuse(double x, double y);
static double randfrom(double min, double max);

int main()
{
  printf("Actual pi value using acos(-1): %lf\n", PI);
  printf("Result of method Monte-Carlo after %d itterations: %lf\n", MAX_LOOP, monteCarlo(MAX_LOOP));
  return 0;
}

/**
 * Method Monte Carlo
 */
static double monteCarlo(int itterations)
{
  int i, hits = 0;
  double x, y, hypotenuse;
  srand((unsigned int)time(NULL)); /* Initialization, should only be called once. */

  for (i = 0; i < itterations; i++)
  {
    x = randfrom(-RADUIS, RADUIS);
    y = randfrom(-RADUIS, RADUIS);
    hypotenuse = findHypotenuse(x, y);

    if (hypotenuse <= (double)RADUIS)
      hits++;
  }

  return ((double)hits * 4 / (double)itterations);
}

/* generate a random floating point number from min to max */
/* Not precise enough apparently, see : https://cboard.cprogramming.com/c-programming/3264-best-way-generate-random-double.html */
static double randfrom(double min, double max)
{
  double range = (max - min);
  double div = RAND_MAX / range;
  return min + (rand() / div);
}

static double findHypotenuse(double a, double b)
{
  double powa = pow(a, 2);
  double powb = pow(b, 2);
  return sqrt(powa + powb);
}