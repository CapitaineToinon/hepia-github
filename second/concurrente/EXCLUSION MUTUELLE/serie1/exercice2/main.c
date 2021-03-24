#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

volatile int mutex;

void init_lock(volatile int *lock)
{
    *lock = 0;
}

void aquire_lock(volatile int *lock)
{
    while (__sync_val_compare_and_swap(lock, 0, 1))
    {
        sleep(0);
    }
    
    printf("LOCKED!\n");
}

void release_lock(volatile int *lock)
{
    *lock = 0;
    printf("UNLOCKED!\n");
}

void *thread_sum(void *vargs)
{
    int *total = (int *)vargs;
    aquire_lock(&mutex);

    // takes a lot of clock cylces
    for (int i = 0; i < 10000000; i++)
    {
        *total += 1;
    }

    release_lock(&mutex);
    return NULL;
}

int main()
{
    int total = 0;
    int threads_counts = 10;
    pthread_t threads[threads_counts];
    init_lock(&mutex);

    // call the threads
    for (int thread = 0; thread < threads_counts; thread++)
    {
        if (pthread_create(&threads[thread], NULL, &thread_sum, &total) != 0)
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

    printf("Total count: %d\n", total);
    return 0;
}