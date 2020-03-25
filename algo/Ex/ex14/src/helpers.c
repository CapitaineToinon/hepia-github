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