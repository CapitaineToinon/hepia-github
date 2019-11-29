#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 5
#define EMPTY '.'
#define X 'X'
#define O 'O'

void init(char game[SIZE][SIZE])
{
  int x, y;
  for (x = 0; x < SIZE; x++)
  {
    for (y = 0; y < SIZE; y++)
    {
      game[x][y] = EMPTY;
    }
  }
}

void printGame(char game[SIZE][SIZE])
{
  int i, x, y;
  for (x = 0; x < SIZE; x++)
  {
    for (y = 0; y < SIZE; y++)
    {
      printf("%c", game[x][y]);

      if (y != SIZE - 1)
        printf(" ");
    }

    /* print the horizontal line */
    if (x != SIZE - 1)
    {
      printf("\n");
    }

    printf("\n");
  }
}

void playerTurn(char game[SIZE][SIZE])
{
  int x, y;
  bool ok = false;
  do
  {
    x = 0, y = 0;
    printf("=> Chose where you would like to play (using format x-y):\n");
    (void)scanf("%d-%d", &x, &y);

    if (x >= SIZE || x < 0 || y >= SIZE || y < 0)
    {
      printf("=> Please enter coordinates between 0;0 and %d;%d.\n", SIZE, SIZE);
      continue;
    }

    if (game[x][y] != EMPTY)
    {
      printf("You cannot play here.\n");
      continue;
    }

    ok = true;
  } while (!ok);

  /* ok */
  game[x][y] = X;
}

bool hasWon3by3(char game[3][3], char who)
{
  int i;

  /* rows */
  for (i = 0; i < 3; i++)
  {
    if (
        game[i][0] == who &&
        game[i][1] == who &&
        game[i][2] == who)
    {
      return true;
    }
  }

  /* columns */
  for (i = 0; i < 3; i++)
  {
    if (
        game[0][i] == who &&
        game[1][i] == who &&
        game[2][i] == who)
    {
      return true;
    }
  }

  /* diagonals */
  if (
      game[0][0] == who &&
      game[1][1] == who &&
      game[2][2] == who)
  {
    return true;
  }

  if (
      game[0][2] == who &&
      game[1][1] == who &&
      game[2][1] == who)
  {
    return true;
  }

  /* no win */
  return false;
}

bool hasWon(char game[SIZE][SIZE], char who)
{
  int x, y;
  int a, b;
  int i, j;

  for (x = 1; x < SIZE - 1; x++)
  {
    for (y = 1; y < SIZE - 1; y++)
    {
      char candidate[3][3];

      for (a = x - 1, i = 0; a < x + 2; a++, i++)
      {
        for (b = y - 1, j = 0; b < y + 2; b++, j++)
        {
          candidate[i][j] = game[a][b];
        }
      }

      if (hasWon3by3(candidate, who))
      {
        return true;
      }
    }
  }

  /* didn't win */
  return false;
}

void cpuTurn(char game[SIZE][SIZE])
{
  int x, y;

  do
  {
    x = 0, y = 0;
    x = (unsigned int)(rand() % SIZE); /* Returns a pseudo-random integer between 0 and SIZE. */
    y = (unsigned int)(rand() % SIZE); /* Returns a pseudo-random integer between 0 and SIZE. */
  } while (game[x][y] != EMPTY);

  printf("CPU played in %d-%d.\n", x, y);
  game[x][y] = O;
}

int main()
{
  srand((unsigned int)time(NULL)); /* Initialization, should only be called once. */
  bool playing = true;
  int plays = 0;
  static char game[SIZE][SIZE];

  init(game);

  do
  {
    playerTurn(game);
    printGame(game);
    plays++;

    if (hasWon(game, X))
    {
      printf("Player has won, game is over.\n");
      printGame(game);
      playing = false;
      continue;
    }

    if (plays >= SIZE * SIZE)
    {
      printf("Seems like it's a tie.\n");
      playing = false;
      continue;
    }
    sleep(1);

    cpuTurn(game);
    printGame(game);
    plays++;

    if (hasWon(game, O))
    {
      printf("CPU has won, game is over.\n");
      printGame(game);
      playing = false;
      continue;
    }

    if (plays >= SIZE * SIZE)
    {
      printf("Seems like it's a tie.\n");
      playing = false;
      continue;
    }
  } while (playing);

  return 0;
}
