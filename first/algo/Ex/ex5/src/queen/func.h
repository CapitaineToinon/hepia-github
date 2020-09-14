#include <stdbool.h>
#define SIZE 8

struct Coordinate
{
  char x;
  char y;
};

bool isCoordinateValid(struct Coordinate coordiate);
bool *getQueenReach(struct Coordinate queen);
bool isSafeFromQueen(struct Coordinate coordinate, struct Coordinate queen);
void printQueenReach(struct Coordinate queen, bool *board);
void printQueens(struct Coordinate *queens, unsigned int size);