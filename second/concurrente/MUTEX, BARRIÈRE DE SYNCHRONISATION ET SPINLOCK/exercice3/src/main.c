#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "barrier.h"

double MAX_K_AKA_I = 5;
double M = 10;
pthread_barrier_t barrier;
double **phase_a_values;
double *k_results;

#define A 100

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

double get_elapsed_ms(struct timespec start, struct timespec finish)
{
    double elapsed_ms = 1000 * (finish.tv_sec - start.tv_sec);
    elapsed_ms += (finish.tv_nsec - start.tv_nsec) / 1000000.0;
    return elapsed_ms;
}

void *phase_B(void *vargs)
{
    for (int k = 0; k < MAX_K_AKA_I; k++)
    {
        pthread_barrier_wait(&barrier);
        k_results[k] = 0;
        
        for (int i = 0; i < M; i++)
        {
            k_results[k] += phase_a_values[k][i];
        }

        printf("r(%d)=%f\n", k, k_results[k]);
    }

    return NULL;
}

void *phase_A(void *vargs)
{
    for (int k = 0; k < MAX_K_AKA_I; k++)
    {
        for (int i = 0; i < M; i++)
        {
            phase_a_values[k][i] = (A * x(k, i));
        }

        pthread_barrier_wait(&barrier);
    }

    return NULL;
}

int main(void)
{
    pthread_barrier_init(&barrier, NULL, 2);
    phase_a_values = malloc(sizeof(double *) * MAX_K_AKA_I);
    for (int k = 0; k < MAX_K_AKA_I; k++)
    {
        phase_a_values[k] = malloc(sizeof(double) * M);
    }

    k_results = malloc(sizeof(double) * MAX_K_AKA_I);

    pthread_t sum, args;
    // pthread_t arguments[thread_count];

    // phase B for the thread
    if (pthread_create(&sum, NULL, &phase_B, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&args, NULL, &phase_A, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // for (int i = 0; i < thread_count; i++)
    // {
    //     if (pthread_create(&arguments[i], NULL, &phase_A, &var.contexts[i]) != 0)
    //     {
    //         fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
    //         exit(EXIT_FAILURE);
    //     }
    // }

    if (pthread_join(sum, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pthread_join(args, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (int k = 0; k < MAX_K_AKA_I; k++)
    {
        printf("Normal r(%d)=%f\n", k, r(k, M));
    }
    

    return 0;
}
