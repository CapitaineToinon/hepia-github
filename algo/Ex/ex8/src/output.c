#include <stdio.h>
#include "output.h"

/**
 * Converted from https://stackoverflow.com/questions/48677066/printing-a-grid
 */
void print_board(int **p, int userRows, int userColumns)
{
    printf("\n");
    printf(" ");
    int i = 1, j;
    for (j = 0; j <= 4 * userColumns; j++)
    {
        if (j % 4 == 2)
            printf("%d", i++);
        else
            printf(" ");
    }
    printf("\n");
    for (i = 0; i <= 2 * userRows; i++)
    {
        if (i % 2 != 0)
            printf(" "); //, (char)(i / 2 + ' '));

        for (j = 0; j <= 2 * userColumns; j++)
        {
            if (i % 2 == 0)
            {
                if (j == 0)
                    printf(" ");
                if (j % 2 == 0)
                    printf("+");
                else
                    printf("---");
            }
            else
            {
                if (j % 2 == 0)
                    printf("|");
                else
                {
                    char c;
                    switch (p[i / 2][j / 2])
                    {
                        case 1:
                            c = PLAYER_CHAR;
                            break;
                        case 2:
                            c = CPU_CHAR;
                            break;
                        case 0:
                        default:
                            c = NOONE_CHAR;
                            break;
                    }

                    printf(" %c ", c);
                }
            }
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