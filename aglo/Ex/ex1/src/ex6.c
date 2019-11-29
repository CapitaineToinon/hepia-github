#include <stdio.h>
#define MIN 1
#define MAX 1000
#define NELEMS(x) (unsigned int)(sizeof(x) / sizeof((x)[0]))

int main()
{
  unsigned int amount, i = 0;
  unsigned int bills[] = {100, 50, 20, 10};
  unsigned int coins[] = {5, 2, 1};
  unsigned int billsLenght = NELEMS(bills);
  unsigned int coinsLenght = NELEMS(coins);
  unsigned int count = 0;

  do
  {
    printf("How much you wanna get change from? (%u CHF MAX) ", MAX);
    (void)scanf("%u", &amount);
  } while (amount < MIN || amount > MAX);

  for (i = 0; i < billsLenght; i++)
  {
    count = amount / bills[i];
    amount = amount - (bills[i] * count);

    if (count > 0)
      printf("%u bills of %u CHF \n\r", count, bills[i]);
  }

  for (i = 0; i < coinsLenght; i++)
  {
    count = amount / coins[i];
    amount = amount - (coins[i] * count);

    if (count > 0)
      printf("%u coins of %u CHF \n\r", count, coins[i]);
  }

  return 0;
}