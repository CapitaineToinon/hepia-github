#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_COUNT 255

struct sum_args_t
{
    int *numbers;
    int count;
};

typedef struct sum_args_t SumArgs;

void *process_sum(void *vargs)
{
    intptr_t sum = 0;
    SumArgs *args = (SumArgs *)vargs;

    if (args == NULL)
    {
        fprintf(stderr, "vargs needs to be SumArgs\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < args->count; i++)
        sum += args->numbers[i];

    return (void *)sum;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "Pass numbers you wish to add together as arguments\n");
        exit(EXIT_FAILURE);
    }

    if (argc - 1 > MAX_COUNT)
    {
        fprintf(stderr, "You cannot sum more than %d numbers\n", MAX_COUNT);
        exit(EXIT_FAILURE);
    }

    pthread_t id;
    void *sum;
    SumArgs *args = (SumArgs *)malloc(sizeof(SumArgs));
    args->count = (argc - 1);
    args->numbers = (int *)malloc(args->count * sizeof(int));

    for (int i = 1, j = 0; i < argc; i++, j++)
    {
        errno = 0;
        char *endptr;
        int value = (int)strtol(argv[i], &endptr, 0);

        if (errno != 0)
        {
            fprintf(stderr, "Error trying to convert argument to number: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (argv[i] == endptr)
        {
            fprintf(stderr, "Error trying to convert argument to number '%s'\n", argv[i]);
            exit(EXIT_FAILURE);
        }

        args->numbers[j] = value;
    }

    if (pthread_create(&id, NULL, &process_sum, args) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pthread_join(id, &sum) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("%ld\n", (intptr_t)sum);
    free(args->numbers);
    free(args);
    return 0;
}