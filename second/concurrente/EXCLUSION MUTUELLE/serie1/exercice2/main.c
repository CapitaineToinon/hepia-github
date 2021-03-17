#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

typedef struct mutex_t
{
    bool locked;
} Mutex;

Mutex *create_mutex()
{
    Mutex *m = (Mutex *)malloc(sizeof(Mutex *));
    m->locked = false;
    return m;
}

Mutex *mutex;

void aquire_lock(Mutex *lock)
{
    while (lock->locked == true)
    {
        sleep(0);
    }

    while (lock->locked == false)
    {
        (void)__sync_val_compare_and_swap(&lock->locked, false, true);
    }

    printf("LOCKED!\n");
}

void release_lock(Mutex *lock)
{
    while (lock->locked == true)
    {
        (void)__sync_val_compare_and_swap(&lock->locked, true, false);
    }

    printf("UNLOCKED!\n");
}

void *thread_sum(void *vargs)
{
    int *total = (int *)vargs;
    aquire_lock(mutex);

    // takes a lot of clock cylces
    for (int i = 0; i < 10000000; i++)
        *total += 1;

    release_lock(mutex);
    return NULL;
}

int main()
{
    int total = 0;
    int threads_counts = 10;
    pthread_t threads[threads_counts];
    mutex = create_mutex();

    // call the threads
    for (int thread = 0; thread < threads_counts; thread++)
    {
        if (pthread_create(&threads[thread], NULL, &thread_sum, &total) != 0)
        {
            fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
            return EXIT_FAILURE;
        }
    }

    // join the threads
    for (int thread = 0; thread < threads_counts; thread++)
    {
        if (pthread_join(threads[thread], NULL) != 0)
        {
            fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
            return EXIT_FAILURE;
        }
    }

    printf("Total count: %d\n", total);
    free(mutex);
    return 0;
}