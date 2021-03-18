#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

volatile bool mutex;

void init_lock(volatile bool *lock)
{
    *lock = false;
}

void aquire_lock(volatile bool *lock)
{
    while (*lock == true)
    {
        sleep(0);
    }

    while (*lock == false)
    {
        (void)__sync_val_compare_and_swap(lock, false, true);
    }

    printf("LOCKED!\n");
}

void release_lock(volatile bool *lock)
{
    while (*lock == true)
    {
        (void)__sync_val_compare_and_swap(lock, true, false);
    }

    printf("UNLOCKED!\n");
}

void *thread_sum(void *vargs)
{
    aquire_lock(&mutex);
    int *total = (int *)vargs;

    // takes a lot of clock cylces
    for (int i = 0; i < 10000000; i++)
        *total += 1;

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