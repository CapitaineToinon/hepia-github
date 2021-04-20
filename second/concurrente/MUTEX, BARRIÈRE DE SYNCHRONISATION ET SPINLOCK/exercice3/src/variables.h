#ifndef __VARIABLES_H_SEEN__
#define __VARIABLES_H_SEEN__

#include <stdlib.h>
#include <pthread.h>
#include "barrier.h"

typedef struct variables_t
{
    int k;
    int M;
    double sum;
    double *arguments;
    pthread_mutex_t mutex;
    pthread_barrier_t barrier;
} variables_t;

void init_variables(variables_t *, int, int);
void destroy_variables(variables_t *);

#endif