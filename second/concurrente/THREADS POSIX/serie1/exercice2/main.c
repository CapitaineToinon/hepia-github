#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_COUNT 255

int *NUMBERS;
int COUNT;
int SUM;

void *process_sum()
{
    SUM = 0;
    for (int i = 0; i < COUNT; i++)
        SUM += NUMBERS[i];
    return NULL;
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
    COUNT = argc - 1;
    NUMBERS = (int *)malloc(COUNT * sizeof(int));

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

        NUMBERS[j] = value;
    }

    if (pthread_create(&id, NULL, &process_sum, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pthread_join(id, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("%d\n", SUM);
    free(NUMBERS);
    return 0;
}