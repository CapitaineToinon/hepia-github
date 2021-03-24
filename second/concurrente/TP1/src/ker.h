#ifndef __KER_H_SEEN__
#define __KER_H_SEEN__

#include <stdlib.h>

typedef struct ker_st
{
    int size;
    double *data;
} ker_t;

ker_t *alloc_ker(int);
void free_ker(ker_t *);

#endif