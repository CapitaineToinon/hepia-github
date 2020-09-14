#include <stdio.h>
#include "func.h"

int main()
{
  bool isValid;
  char response[2];
  bool *board;
  struct Coordinate queen;

  do
  {
    isValid = true;
    printf("Queen's position: ");
    scanf("%s", response);

    queen.x = response[1];
    queen.y = response[0];
    isValid = isCoordinateValid(queen);

    if (!isValid)
    {
      printf("Position not valid\n");
    }

  } while (isValid == false);

  printQueenReach(queen, getQueenReach(queen));

  return 0;
}