#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *custom_thread(void *vargs)
{
    printf("THREAD start\n");
    sleep(5);
    printf("THREAD end\n");
    return NULL;
}

void *thread_with_args_and_return(void *vargs)
{
    printf("Hello %s\n", (char *)vargs);
    return (void *)42;
}

int main()
{
    // compile with:
    // gcc thread_example.c -o thread_example -lpthread
    pthread_t id;
    printf("Before calling thread\n");
    if (pthread_create(&id, NULL, &custom_thread, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("During the thread calling thread\n");
    sleep(1);
    printf("I can do things here\n");
    sleep(1);
    printf("Now waiting for threads to end...\n");

    if (pthread_join(id, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&id, NULL, &thread_with_args_and_return, "Antoine") != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    void *returned_value;

    if (pthread_join(id, &returned_value) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Thread returned the number %ld\n", (intptr_t)returned_value);
    printf("After thread is over\n");
}