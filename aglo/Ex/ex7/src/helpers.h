#include <stdio.h>
#include <stdlib.h>

struct method
{
  char name[10];
  void (*sort)(int *, int);
};

/**
 * from https://bits.mdminhazulhaque.io/c/shuffle-items-of-array-in-c.html
 */
void shuffledArray(int *arr, size_t n)
{
  int i;

  for (i = 0; i < n; i++)
    arr[i] = i + 1;

  for (i = n - 1; i > 0; i--)
  {
    int j = rand() % (i + 1);
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

void printValues(int *values, int length)
{
  int i;
  printf("int values[] = { ");

  for (i = 0; i < length; i++)
  {
    printf("%d", values[i]);

    if (i != length - 1)
      printf(", ");
  }
  printf(" };\n");
}