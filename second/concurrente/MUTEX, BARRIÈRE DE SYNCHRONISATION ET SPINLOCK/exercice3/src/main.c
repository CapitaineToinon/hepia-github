#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include "helper.h"
#include "barrier.h"

#define ARGUMENT_COUNT 4
#define A 100

typedef struct phase_a_t
{
    int from;
    int to;
} phase_a_t;

/**
 * Prints the help on how to use this program
 * 
 * @param dest the file to which the help should be sent
 * @param filename the name of the current program
 */
void print_help(FILE *dest, char *filename)
{
    fprintf(dest, "Usage: %s <M> <N> <I>\n", filename);
}

// global variables used by the threads
int M, N, I;
pthread_barrier_t barrier;
double **phase_a_values;

double x(int k, int i)
{
    double temp = sin(k + i);
    return temp > 0 ? temp : 0;
}

double r(double k, double M)
{
    double result = 0;

    for (int i = 0; i < M; i += 1)
    {
        result += (A * x(k, i));
    }

    return result;
}

/**
 * Thread use to do the sums of the values computed by the phase_A thread(s)
 * 
 * @param vargs vargs is not used
 */
void *phase_B(void *vargs)
{
    (void)vargs;
    for (int k = 0; k < I; k++)
    {
        pthread_barrier_wait(&barrier);
        double result = 0;

        for (int i = 0; i < M; i++)
        {
            result += phase_a_values[k][i];
        }

        printf("r(%d)=%f\n", k, result);
    }

    return NULL;
}

/**
 * Thread use to do the sums of the values computed by the phase_A thread(s)
 * 
 * @param vargs context of type phase_a_t used by the thread to know its own context
 */
void *phase_A(void *vargs)
{
    phase_a_t *args = (phase_a_t *)vargs;

    for (int k = 0; k < I; k++)
    {
        for (int i = args->from; i < args->to; i++)
        {
            phase_a_values[k][i] = (A * x(k, i));
        }

        pthread_barrier_wait(&barrier);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != ARGUMENT_COUNT)
    {
        print_help(stderr, argv[0]);
        exit(EXIT_FAILURE);
    }

    int phase_a_N;
    struct timespec start, finish;
    M = stringtoi(argv[1]);
    N = stringtoi(argv[2]);
    I = stringtoi(argv[3]);
    phase_a_N = N - 1;

    if (N < 2)
    {
        fprintf(stderr, "N needs to be equal or greater than 2\n");
        exit(EXIT_FAILURE);
    }

    if (M < 1)
    {
        fprintf(stderr, "M needs to be a positif interger\n");
        exit(EXIT_FAILURE);
    }

    if (I < 1)
    {
        fprintf(stderr, "I needs to be a positif interger\n");
        exit(EXIT_FAILURE);
    }

    if (N > M)
    {
        fprintf(stderr, "N cannot be greater than M\n");
        exit(EXIT_FAILURE);
    }

    // The barrier that will be used by both phase_A and phase_B
    pthread_barrier_init(&barrier, NULL, N);

    if ((phase_a_values = malloc(sizeof(double *) * I)) == NULL)
    {
        fprintf(stderr, "Error with malloc: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (int k = 0; k < I; k++)
    {
        if ((phase_a_values[k] = malloc(sizeof(double) * M)) == NULL)
        {
            fprintf(stderr, "Error with malloc: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    // create the contexts for the threads,
    // passing all the needed variables
    pthread_t sum;
    pthread_t args[phase_a_N];
    phase_a_t args_t[phase_a_N];

    int chunk_size = M / phase_a_N;
    int chunk_extra = M % phase_a_N;

    for (int i = 0; i < phase_a_N; i++)
    {
        int from = chunk_size * i;
        int to = from + chunk_size;
        args_t[i] = (phase_a_t){from, to};
    }

    args_t[phase_a_N - 1].to += chunk_extra;

    clock_gettime(CLOCK_MONOTONIC, &start);
    // create the one and only thread used to sum the results
    if (pthread_create(&sum, NULL, &phase_B, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // create all the needed threads for the computation of the args
    for (int i = 0; i < phase_a_N; i++)
    {
        if (pthread_create(&args[i], NULL, &phase_A, &args_t[i]) != 0)
        {
            fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    // join back the threads
    if (pthread_join(sum, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < phase_a_N; i++)
    {
        if (pthread_join(args[i], NULL) != 0)
        {
            fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);
    printf("Total time %f\n", get_elapsed_ms(start, finish));

    return 0;
}
