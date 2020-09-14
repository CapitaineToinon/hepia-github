#include <stdio.h>
#include <math.h>
#define PI acos(-1)
#define MAX_LOOP 100000

/* function headers */
static double Method1(int itterations);
static double Method2(int itterations);
static double Method3(int itterations);

int main()
{
  printf("Actual pi value using acos(-1): %f\n", PI);
  printf("Result of Method 1 after %d itterations: %lf\n", MAX_LOOP, Method1(MAX_LOOP));
  printf("Result of Method 2 after %d itterations: %lf\n", MAX_LOOP, Method2(MAX_LOOP));
  printf("Result of Method 3 after %d itterations: %lf\n", MAX_LOOP, Method3(MAX_LOOP));
  return 0;
}

/**
 * Method 1
 */
static double Method1(int itterations)
{
  int n;
  double sum = 0, result = 0;

  for (n = 1; n <= itterations; n++)
  {
    sum += 1 / pow((double)n, 4);
  }

  result = sqrt(sqrt((sum * 90)));
  return result;
}

/**
 * Method 2
 */
static double Method2(int itterations)
{
  int i;
  double numerator, denominator, n, sum = 0, result = 0;

  for (i = 1; i <= itterations; i++)
  {
    n = (double)i;
    numerator = pow(-1, n + 1);
    denominator = pow(n, 2);
    sum += numerator / denominator;
  }

  result = sqrt((sum * 12));
  return result;
}

/**
 * Method 3
 */
static double Method3(int itterations)
{
  int i;
  double n, left, right, product = 1, result = 0;

  for (i = 1; i <= itterations; i++)
  {
    n = (double)i;
    left = (double)(2 * n) / (double)(2 * n - 1);
    right = (double)(2 * n) / (double)(2 * n + 1);
    product *= left * right;
  }

  result = product * 2;
  return result;
}