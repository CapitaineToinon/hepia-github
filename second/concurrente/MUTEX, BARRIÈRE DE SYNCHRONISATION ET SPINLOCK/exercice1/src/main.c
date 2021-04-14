#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "barrier.h"

#define MAX_DURATION 60
#define MIN_DURATION 30

void *thread_with_barrier(void *vargs)
{
    barrier_t *b = (barrier_t *)vargs;
    int r = MIN_DURATION + (int)(((double)rand() / (double)RAND_MAX) * MAX_DURATION);

    // heavy work
    printf("Thread work for %ds\n", r);
    sleep(r);

    if (barrier_wait(b) != 0)
    {
        printf("BARRIER FAILED\n");
        exit(EXIT_FAILURE);
        return NULL;
    }

    printf("Ready to go!\n"); // this will happen at the same time in all threads

    return NULL;
}

int main(void)
{
    srand(time(NULL));

    barrier_t my_barrier;
    int threads_counts = 12;
    pthread_t threads[threads_counts];

    if (barrier_init(&my_barrier, threads_counts) != 0)
    {
        fprintf(stderr, "Error with barrier_init: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Threads count: %d\n", my_barrier.count);

    // call the threads
    for (int thread = 0; thread < threads_counts; thread++)
    {
        if (pthread_create(&threads[thread], NULL, &thread_with_barrier, &my_barrier) != 0)
        {
            fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    // join the threads
    for (int thread = 0; thread < threads_counts; thread++)
    {
        if (pthread_join(threads[thread], NULL) != 0)
        {
            fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    printf("All threads are done.\n");
    barrier_destroy(&my_barrier);
    return 0;
}

/**
 * Question
 * Que pouvez-vous conclure de votre implémentation au niveau de l’utilisation processeur ?
 * 
 * CPU sont full quand ils attendent
 */
