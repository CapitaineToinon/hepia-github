#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "bubblesort.h"
#include "insertsort.h"
#include "selectsort.h"
#include "helpers.h"
#define SIZE 1000
#define ITTERATIONS 10000

int main()
{
  srand((unsigned int)time(NULL)); /* Initialization, should only be called once. */

  // bubble
  struct method Bubble;
  strcpy(Bubble.name, "Bubble sort");
  Bubble.sort = &bubblesort;

  // insert
  struct method Insert;
  strcpy(Insert.name, "Insert sort");
  Insert.sort = &insertsort;

  // select
  struct method Select;
  strcpy(Select.name, "Select sort");
  Select.sort = &selectsort;

  struct method methods[] = {Bubble, Insert, Select};

  printf("Showing off the methods with simple arrays of size 10.\n");

  int values[10] = {};
  for (int i = 0; i < (sizeof(methods)/sizeof(*methods)); i++)
  {
    struct method c = methods[i];
    printf("<--- %s --->\n", c.name);
    shuffledArray(values, 10);
    printf("Before => ");
    printValues(values, 10);

    // sort
    (*c.sort)(values, 10);

    printf("After => ");
    printValues(values, 10);
  }

  printf("Timing the various methods with arrays or size %d over %d itterations.\n", SIZE, ITTERATIONS);

  int timing[SIZE] = {};
  for (int i = 0; i < (sizeof(methods) / sizeof(*methods)); i++)
  {
    struct method c = methods[i];

    printf("<--- %s --->\n", c.name);
    clock_t begin = clock();
    for (int j = 0; j < ITTERATIONS; j++)
    {
      shuffledArray(timing, SIZE);
      (*c.sort)(timing, SIZE);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Done %d itterations in %f seconds\n", ITTERATIONS, time_spent);
  }
}