#include "helpers.h"

/* not sercure at all */
void arraycpy(int *source, int *dest, int size)
{
    for (int i = 0; i < size; i++)
    {
        dest[i] = source[i];
    }
}

/* not sercure at all */
bool arrayAreSame(int *a, int *b, int size)
{
    for (int i = 0; i < size; i++)
        if (a[i] != b[i])
            return false;

    return true;
}

/* not sercure at all */
void arrayswap(int *source, int i, int j)
{
    int tmp = source[i];
    source[i] = source[j];
    source[j] = tmp;
}

/* not sercure at all */
void printArray(int *array, int size)
{
    printf("{");
    for (int i = 0; i < size; i++)
    {
        printf("%d", array[i]);

        if (i != size - 1)
            printf(", ");
    }
    printf("}\n");
}

/**
 * Prints a tab according to its size.
 * Example for a 3 by 3:
 * 
 *  +---+
 *  |012|
 *  |345|
 *  |678|
 *  +---+
 * 
 */ 
void printTab(int *tab, int size_x, int size_y)
{
    int print_x = size_x + 2;
    int print_y = size_y + 2; 

    for (int i = 0; i < (print_x * print_y); i++)
    {
        int x = i % print_x;
        int y = i / print_y;

        if (
            (y == 0 && x == 0) ||
            (y == 0 && x == print_x - 1) ||
            (y == print_y - 1 && x == 0) ||
            (y == print_y - 1 && x == print_x - 1))
        {
            printf("+");
        }
        else 
        {
            if ((y == 0 || y == print_y - 1) && x != 0 && x != print_x - 1)
                printf("-");
            else if ((x == 0 || x == print_x - 1) && y != 0 && y != print_y - 1)
                printf("|");
            else {
                int tab_x = x - 1;
                int tab_y = y - 1;
                int index = (tab_y * size_x) + tab_x;
                printf("%d", tab[index]);
            }
        }

        if (x == print_x - 1)
            printf("\n");
    }
}