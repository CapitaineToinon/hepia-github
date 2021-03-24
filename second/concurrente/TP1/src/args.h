#ifndef __ARGS_H_SEEN__
#define __ARGS_H_SEEN__

#include "ppm.h"
#include "ker.h"

typedef struct kernel_params
{
    img_t *img;
    ker_t *kernel;
    int size;
    int offset;
    img_t *result;
} KernelParams;

KernelParams *alloc_kernel_params();

#endif