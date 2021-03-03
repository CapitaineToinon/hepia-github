#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct sum_args_t
{
    int *numbers;
    int count;
    int total;
};

typedef struct sum_args_t SumArgs;

SumArgs *create_sum_arguments()
{
    SumArgs *argument = (SumArgs *)malloc(sizeof(SumArgs));
    argument->count = 0;
    argument->total = 0;
    return argument;
}

void free_sum_argement(SumArgs *argument)
{
    if (argument != NULL)
    {
        free(argument->numbers);
        free(argument);
    }
}

void *process_sum(void *vargs)
{
    intptr_t sum = 0;
    SumArgs *args = (SumArgs *)vargs;

    if (args == NULL)
    {
        fprintf(stderr, "vargs cannot be NULL\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < args->total; i++)
        sum += args->numbers[i];

    return (void *)sum;
}

int stringtoi(const char *__str)
{
    errno = 0;
    char *endptr;
    int value = (int)strtol(__str, &endptr, 0);

    if (errno != 0)
    {
        fprintf(stderr, "Error trying to convert argument to number: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (__str == endptr)
    {
        fprintf(stderr, "Error trying to convert argument to number '%s'\n", __str);
        exit(EXIT_FAILURE);
    }

    return value;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "todo print help here\n");
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

    // create all arguments for each thread
    SumArgs **arguments = (SumArgs **)malloc(m * sizeof(SumArgs *));
    for (int thread_id = 0; thread_id < m; thread_id++)
    {
        arguments[thread_id] = create_sum_arguments();
    }

    // count how many numbers each thread will have to add
    for (int i = 1; i <= n; i++)
    {
        int thread_id = i % m;
        arguments[thread_id]->total += 1;
    }

    // malloc numbers for each threads
    for (int thread_id = 0; thread_id < m; thread_id++)
    {
        arguments[thread_id]->numbers = (int *)malloc(arguments[thread_id]->total * sizeof(int));
    }

    // add all the numbers
    for (int i = 1, j = 0; i <= n; i++, j++)
    {
        int thread_id = j % m;
        arguments[thread_id]->numbers[arguments[thread_id]->count] = i;
        arguments[thread_id]->count++;
    }

    // call the threads
    pthread_t threads_ids[m];
    intptr_t total_sum = 0;

    for (int i = 0; i < m; i++)
    {
        if (pthread_create(&threads_ids[i], NULL, &process_sum, arguments[i]) != 0)
        {
            fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

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
        free_sum_argement(arguments[i]);
    }
    
    free(arguments);

    return 0;
}