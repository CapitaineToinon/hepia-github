#ifndef __SUM_H_SEEN__
#define __SUM_H_SEEN__

#include <stdlib.h>

struct sum_args_t
{
    int from;
    int delta;
};
typedef struct sum_args_t SumArgs;
SumArgs *create_sum_arguments();

#endif