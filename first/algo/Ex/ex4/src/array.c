#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define ARRAY_SIZE 11
#define MAX

static int min(int *values, unsigned int size);
static int max(int *values, unsigned int size);
static int mean(int *values, unsigned int size);
static int median(int *values, unsigned int size);
static double variance(int *values, unsigned int size);
static bool unique(int *values, unsigned int size, int candidate);

int main()
{
  static int values[ARRAY_SIZE];
  int i, candidate;

  srand((unsigned int)time(NULL)); /* Initialization, should only be called once. */

  for (i = 0; i < ARRAY_SIZE; i++)
  {
    do
    {
      candidate = rand() % 100;
    } while (!unique(values, ARRAY_SIZE, candidate));

    printf("%d\n", candidate);
    values[i] = candidate;
  }

  printf("Min: %d\n", min(values, ARRAY_SIZE));
  printf("Max: %d\n", max(values, ARRAY_SIZE));
  printf("Average: %d\n", mean(values, ARRAY_SIZE));
  printf("Variance: %1.2f\n", variance(values, ARRAY_SIZE));
  printf("Median: %d\n", median(values, ARRAY_SIZE));

  return 0;
}

/**
 * Returns true if value is unique
 */ 
static bool unique(int *values, unsigned int size, int candidate)
{
  unsigned int i;

  for (i = 0; i < size; i++)
  {
    if (values[i] == candidate)
    {
      return false;
    }
  }

  return true;
}

/**
 * Returns the min value in an array
 */
static int min(int *values, unsigned int size)
{
  int min = (size > 0) ? values[0] : 0;
  unsigned int i;

  for (i = 0; i < size; i++)
  {
    if (values[i] < min)
      min = values[i];
  }

  return min;
}

/**
 * Returns the max value in an array
 */
static int max(int *values, unsigned int size)
{
  int max = (size > 0) ? values[0] : 0;
  unsigned int i;

  for (i = 0; i < size; i++)
  {
    if (values[i] > max)
      max = values[i];
  }

  return max;
}

/**
 * Get the average of the sum of the values in an array
 */
static int mean(int *values, unsigned int size)
{
  int sum = 0;
  unsigned int i;

  if (size < 1)
    return sum;

  for (i = 0; i < size; i++)
  {
    sum += values[i];
  }

  return sum / size;
}

/**
 * Returns the variance of the values in an array
 */
static double variance(int *values, unsigned int size)
{
  unsigned int i;
  int sum = 0;
  int average = mean(values, size);

  for (i = 0; i < size; i++)
  {
    sum += pow((double)(values[i] - average), 2);
  }

  return (double)sum / size;
}

/**
 * Returns the median value of values in an array
 */
static int median(int *values, unsigned int size)
{
  int temp;
  unsigned int i, j;

  for (i = 0; i < size - 1; i++)
  {
    for (j = i + 1; j < size; j++)
    {
      if (values[i] > values[j])
      {
        temp = values[i];
        values[i] = values[j];
        values[j] = temp;
      }
    }
  }

  return values[(size / 2)];
}