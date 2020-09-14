#include <stdio.h>
#include <stdbool.h>
#include "func.h"
#define ASCII_A 65
#define ASCII_H 72
#define ASCII_1 49
#define ASCII_8 56
#define REACHABLE '*'
#define UNREACHABLE '.'
#define QUEEN 'Q'

bool isCoordinateValid(struct Coordinate coordiate)
{
  bool valid;
  int x, y;

  x = (int)coordiate.x - ASCII_1;
  y = (int)coordiate.y - ASCII_A;

  valid = (x >= 0 && x < SIZE && y >= 0 && y < SIZE);

  return valid;
}

bool *getQueenReach(struct Coordinate queen)
{
  static bool board[SIZE * SIZE];
  int queenx, queeny;
  int i, x, y;

  queenx = (int)queen.x - ASCII_1;
  queeny = ASCII_A - (int)queen.y + SIZE - 1;

  for (i = 0; i < SIZE * SIZE; i++)
  {
    x = i % SIZE;
    y = i / SIZE;

    if (x == queenx || y == queeny)
    {
      board[i] = true;
      continue;
    }

    if ((x - queenx == y - queeny) || (queenx - x == y - queeny))
    {
      board[i] = true;
      continue;
    }

    board[i] = false;
  }

  return board;
}

bool isSafeFromQueen(struct Coordinate coordinate, struct Coordinate queen)
{
  bool safe = true;
  int coordinatex, coordinatey, x, y;

  coordinatex = (int)coordinate.x - ASCII_1;
  coordinatey = ASCII_A - (int)coordinate.y + SIZE - 1;

  bool *board = getQueenReach(queen);

  for (x = 0; x < SIZE; x++)
  {
    for (y = 0; y < SIZE; y++)
    {
      if (board[(SIZE * y) + x] &&
          (coordinatex == x && coordinatey == y))
      {
        safe = false;
        break;
      }
    }
  }

  return safe;
}

void printQueenReach(struct Coordinate queen, bool *board)
{
  char toPrint;
  int i, x, y, row, col;
  int queenx, queeny;

  queenx = (int)queen.x - ASCII_1;
  queeny = ASCII_A - (int)queen.y + SIZE - 1;

  for (i = 0; i < (SIZE + 1) * (SIZE + 1); i++)
  {
    row = i / (SIZE + 1);
    col = i % (SIZE + 1);

    x = col - 1;
    y = row;

    if (col == 0)
    {
      if (row == SIZE)
      {
        printf("  ");
      }
      else
      {
        printf("%c ", ASCII_A + SIZE - y - 1);
      }
    }
    else if (row == SIZE && col != 0)
    {
      printf("%d ", x + 1);
    }
    else
    {

      if (x == queenx && y == queeny)
      {
        printf("Q ");
      }
      else
      {
        printf("%c ", board[(SIZE * y) + x] == true ? REACHABLE : UNREACHABLE);
      }
    }

    if (col == SIZE)
    {
      printf("\n");
    }
  }
}

void printQueens(struct Coordinate *queens, unsigned int size)
{
  char toPrint;
  int i, x, y, row, col, q;
  int queenx, queeny;

  for (i = 0; i < (SIZE + 1) * (SIZE + 1); i++)
  {
    row = i / (SIZE + 1);
    col = i % (SIZE + 1);

    x = col - 1;
    y = row;

    if (col == 0)
    {
      if (row == SIZE)
      {
        printf("  ");
      }
      else
      {
        printf("%c ", ASCII_A + SIZE - y - 1);
      }
    }
    else if (row == SIZE && col != 0)
    {
      printf("%d ", x + 1);
    }
    else
    {
      toPrint = '.';

      for (q = 0; q < size; q++)
      {
        queenx = (int)queens[q].x - ASCII_1;
        queeny = ASCII_A - (int)queens[q].y + SIZE - 1;

        if (queenx == x && queeny == y)
        {
          toPrint = 'Q';
          break;
        }
      }

      printf("%c ", toPrint);
    }

    if (col == SIZE)
    {
      printf("\n");
    }
  }
}