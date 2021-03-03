#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS 16

void *thread(void *thread_id)
{
    int id = *((int *)thread_id);
    printf("Hello from thread %d\n", id);
    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];

    int ids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        ids[i] = i;
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_create(&threads[i], NULL, thread, &ids[i]) != 0)
        {
            fprintf(stderr, "pthread_create failed!\n");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            fprintf(stderr, "pthread_join failed!\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}