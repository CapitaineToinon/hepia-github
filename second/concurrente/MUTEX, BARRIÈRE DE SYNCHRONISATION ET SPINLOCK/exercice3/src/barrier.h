#ifdef __APPLE__

#ifndef PTHREAD_BARRIER_H_
#define PTHREAD_BARRIER_H_

/**
 * This is just a dummy file to have
 * headers available for when I'm 
 * coding on macos, since barriers
 * aren't implemented there.
 * 
 * In the end, I execute the code
 * on linux so ultimately this
 * header file is ignored
 */

#include <pthread.h>
#include <errno.h>

typedef int pthread_barrierattr_t;
typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int tripCount;
} pthread_barrier_t;

int pthread_barrier_init(pthread_barrier_t *, const pthread_barrierattr_t *, unsigned int);
int pthread_barrier_destroy(pthread_barrier_t *);
int pthread_barrier_wait(pthread_barrier_t *);

#endif
#endif