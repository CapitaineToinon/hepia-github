#ifndef __BARRIER_H_SEEN__
#define __BARRIER_H_SEEN__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>

typedef struct barrier_t
{
    int finished;
    int count;
    bool init;
    pthread_mutex_t mutex;
} barrier_t;

int barrier_init(barrier_t *, int);
int barrier_wait(barrier_t *);
int barrier_destroy(barrier_t *);

#endif