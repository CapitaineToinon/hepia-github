#include <stdio.h>
#define MAX 5

int main()
{
  int numbers[MAX];
  int i = 0, average = 0;

  for (i = 0; i < MAX; i++)
  {
    printf("Enter number #%d", i + 1);
    (void)scanf("%d", &numbers[i]);
  }

  for (i = 0; i < MAX; i++)
  {
    average = average + numbers[i];
  }

  average = average / MAX;

  printf("Average: %d", average);

  return 0;
}