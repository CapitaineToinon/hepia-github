#ifndef __KER_H_SEEN__
#define __KER_H_SEEN__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct ker_st
{
    int size;
    double *data;
} ker_t;

ker_t *alloc_ker(int);
ker_t *alloc_identity();
ker_t *alloc_sharpen();
ker_t *alloc_edge();
ker_t *alloc_blur(int);
ker_t *alloc_gauss();
ker_t *alloc_unsharp();

void free_ker(ker_t *);

#endif