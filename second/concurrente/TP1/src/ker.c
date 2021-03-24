#include "ker.h"

ker_t *alloc_ker(int size)
{
    ker_t *ker = malloc(sizeof(ker_t));
    ker->size = size;
    ker->data = malloc(sizeof(double) * size * size);
    return ker;
}

void free_ker(ker_t *ker)
{
    free(ker->data);
    free(ker);
}