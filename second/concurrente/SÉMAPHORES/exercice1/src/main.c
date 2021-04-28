#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

sem_t sem_1;
sem_t sem_2;

void *thread_a(void *vargs)
{
    (void)vargs;
    printf("a1\n");
    sem_post(&sem_1);
    sem_wait(&sem_2);
    printf("a2\n");
    return NULL;
}

void *thread_b(void *vargs)
{
    (void)vargs;

    printf("b1\n");
    sem_post(&sem_2);
    sem_wait(&sem_1);
    printf("b2\n");
    return NULL;
}

int main(void)
{
    pthread_t a, b;

    sem_init(&sem_1, 0, 0);
    sem_init(&sem_2, 0, 0);

    if (pthread_create(&b, NULL, &thread_b, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&a, NULL, &thread_a, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pthread_join(a, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pthread_join(b, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return 0;
}
