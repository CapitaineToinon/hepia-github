/**
 * Author:  Antoine Sutter
 * Email:   antoinesutter@gmail.com
 * Github:  CapitaineToinon
 */
#include "input.h"

/**
 * Returns the column the player wants to play in
 */
int get_player_column(int min, int max)
{
    int input = min;
    bool valid;

    do
    {
        valid = true;
        printf("Chose the column you'd like to play:\n");

        if (scanf("%d", &input) != 1)
        {
            fflush(stdin);
            valid = false;
        }

        if (input < min || input > max)
        {
            valid = false;
        }
    } while (!valid);

    return input;
}