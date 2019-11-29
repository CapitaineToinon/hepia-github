#include <stdio.h>
#include <stdlib.h>
#include "input.h"

int get_player_column(int min, int max)
{
    int input = min;

    do
    {
        printf("Chose the column you'd like to play:\n");
        (void)scanf("%d", &input);
    } while (input < min || input > max);

    return input;
}