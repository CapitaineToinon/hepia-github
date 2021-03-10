#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "helper.h"
#include "sum.h"

void *process_sum(void *vargs)
{
    intptr_t sum = 0;
    SumArgs *args = (SumArgs *)vargs;

    if (args == NULL)
    {
        fprintf(stderr, "vargs cannot be NULL\n");
        exit(EXIT_FAILURE);
    }

    #ifdef DEBUG
    pthread_t id = pthread_self();
    printf("Thread %ld is adding numbers from %d to %d\n", id, args->from, args->from + args->delta);
    #endif

    for (int i = args->from; i < args->from + args->delta; i++)
        sum += i;

    return (void *)sum;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <numbers_to_add> <amount_of_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n, m;
    n = stringtoi(argv[1]);
    m = stringtoi(argv[2]);

    if (n < 1)
    {
        fprintf(stderr, "N needs to be an integer greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    if (m < 1)
    {
        fprintf(stderr, "M needs to be an integer greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    if (m > n)
        m = n;

    #ifdef DEBUG
    printf("n: %d, m: %d\n", n, m);
    #endif

    int delta = n / m; // how many numbers each thread will add together
    SumArgs **arguments = (SumArgs **)malloc(m * sizeof(SumArgs *));
    pthread_t threads_ids[m];
    intptr_t total_sum = 0;

    for (int thread = 0; thread < m; thread++)
    {
        arguments[thread] = create_sum_arguments();
        arguments[thread]->from = (thread * delta) + 1;
        arguments[thread]->delta = delta;
    }

    // Adding remaining numbers to the last thread
    arguments[m - 1]->delta += n % m;

    // call the threads
    for (int thread = 0; thread < m; thread++)
    {
        if (pthread_create(&threads_ids[thread], NULL, &process_sum, arguments[thread]) != 0)
        {
            fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    // join the threads
    for (int i = 0; i < m; i++)
    {
        void *local_sum = 0;

        if (pthread_join(threads_ids[i], &local_sum) != 0)
        {
            fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        total_sum += (intptr_t)local_sum;
    }

    // result, we're done
    fprintf(stdout, "%ld\n", total_sum);

    for (int i = 0; i < m; i++)
    {
        free(arguments[i]);
    }

    free(arguments);

    return 0;
}