#include "sum.h"

SumArgs *create_sum_arguments()
{
    SumArgs *argument = (SumArgs *)malloc(sizeof(SumArgs));
    argument->from = 0;
    argument->delta = 0;
    return argument;
}
