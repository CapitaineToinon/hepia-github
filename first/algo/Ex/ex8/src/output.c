/**
 * Author:  Antoine Sutter
 * Email:   antoinesutter@gmail.com
 * Github:  CapitaineToinon
 */
#include "connect_four.h"
#include "output.h"

/**
 * Converted from a C++ example found here:
 * https://stackoverflow.com/questions/48677066/printing-a-grid
 */
void print_board(int **p, int userRows, int userColumns)
{
    printf("\n ");
    int i = 1, j;
    for (j = 0; j <= 4 * userColumns; j++)
    {
        if (j % 4 == 2)
        {
            if (get_available_row(p, userRows, userColumns, i) != -1)
                printf("%d", i);
            else
                printf(" ");
            i++;
        }
        else
            printf(" ");
    }
    printf("\n");
    for (i = 0; i <= 2 * userRows; i++)
    {
        if (i % 2 != 0)
            printf(" ");

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

                    printf(" %c ", c); // where we actually print the board in position [i/2][j/2]
                }
            }
        }
        printf("\n");
    }
}

/**
 * Print who's the winner
 */ 
void print_winner(int winner)
{
    switch (winner) {
        default:
        case NOONE: 
            printf("Noone won, seems like it's a tie!\n");
            break;
        case PLAYER1: 
            printf("Player 1 won!\n");
            break;
        case PLAYER2: 
            printf("Player 2 won!\n");
            break;
    }
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