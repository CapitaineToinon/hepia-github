#include <stdio.h>
#include "output.h"

void print_board(int **p, int N, int M)
{
    int i, j;

    for (i = 0; i < M; i++)
        printf("%d", i + 1);

    printf("\n");

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            if (p[i][j] == 1)
                printf("%c", PLAYER_CHAR);

            if (p[i][j] == 2)
                printf("%c", CPU_CHAR);

            if (p[i][j] == 0)
                printf("%c", NOONE_CHAR);
        }
        printf("\n");
    }
}

void print_winner(int winner)
{
    if (winner == 1)
        printf("You won!\n");

    if (winner == 2)
        printf("The CPU won!\n");
}

/**
 * Prints the help
 */
void help(const char *filename)
{
    printf("Welcome to the count 4 game. Please select a game mode passing it as the first argument of this program.\n\n"
           "Game modes:\n"
           "\t1: player vs player\n"
           "\t2: player vs stupid cpu\n"
           "\t3: player vs smart cpu\n\n"
           "Example usage: %s 1\n",
           filename);
}