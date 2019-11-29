#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 8
#define QUEEN 'Q'
#define EMPTY '-'

void init(bool board[SIZE][SIZE])
{
  int x, y;
  for (x = 0; x < SIZE; x++)
  {
    for (y = 0; y < SIZE; y++)
    {
      board[x][y] = false;
    }
  }
}

void randomFill(bool board[SIZE][SIZE])
{
  int x, y;
  for (x = 0; x < SIZE; x++)
  {
    y = (int)(rand() % SIZE);
    board[x][y] = true;
  }
}

void printBoard(bool board[SIZE][SIZE])
{
  int x, y;
  for (x = 0; x < SIZE; x++)
  {
    for (y = 0; y < SIZE; y++)
    {
      printf("%c ", board[x][y] ? QUEEN : EMPTY);
    }

    printf("\n");
  }
}

bool isCoordinateOk(bool board[SIZE][SIZE], int testx, int testy)
{
  int x, y;
  for (x = 0; x < SIZE; x++)
  {
    for (y = 0; y < SIZE; y++)
    {
      if (x == testx && y == testy)
        continue;

      if (board[x][y])
      {
        if (x == testx || y == testy)
        {
          /* found another queen in horizontal or vertical */
          return false;
        }

        if ((x - testx == y - testy) || (testx - x == y - testy))
        {
          /* found another queen in diagonal */
          return false;
        }
      }
    }
  }

  /* ok */
  return true;
}

bool solved(bool board[SIZE][SIZE])
{
  int x, y;
  for (x = 0; x < SIZE; x++)
  {
    for (y = 0; y < SIZE; y++)
    {
      if (board[x][y] && !isCoordinateOk(board, x, y))
      {
        return false;
      }
    }
  }

  /* ok */
  return true;
}

int main()
{
  srand((unsigned int)time(NULL)); /* Initialization, should only be called once. */
  bool board[SIZE][SIZE];
  int attempts = 0;
  clock_t begin = clock();

  do
  {
    init(board);
    randomFill(board);
    attempts++;
  } while (!solved(board));

  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("Found a %d by %d solution in %d attempts in %f seconds\n", SIZE, SIZE, attempts, time_spent);
  printBoard(board);
  return 0;
}
