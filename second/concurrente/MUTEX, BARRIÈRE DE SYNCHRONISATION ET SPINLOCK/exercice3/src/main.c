#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "variables.h"
#include "barrier.h"

#define A 100

double x(double k, double i)
{
    double temp = sin(k + i);
    return temp > 0 ? temp : 0;
}

double r(double k, double M)
{
    double result = 0;

    for (double i = 0; i < M; i += 1)
    {
        result += (A * x(k, i));
    }

    return result;
}

// void *sum_thread(void *vargs)
// {
//     variables_t *var = (variables_t *)vargs;

//     for (int i = 0; i < var->M; i += 1)
//     {
//         pthread_barrier_wait(&var->barrier);
//         var->sum += var->arguments[i];
//     }

//     return NULL;
// }

// void *arguments_thread(void *vargs)
// {
//     variables_t *var = (variables_t *)vargs;

//     for (int i = 0; i < var->M; i += 1)
//     {
//         var->arguments[i] = A * x(var->k, (double)i);
//         pthread_barrier_wait(&var->barrier);
//     }

//     return NULL;
// }

// double get_elapsed_ms(struct timespec start, struct timespec finish)
// {
//     double elapsed_ms = 1000 * (finish.tv_sec - start.tv_sec);
//     elapsed_ms += (finish.tv_nsec - start.tv_nsec) / 1000000.0;
//     return elapsed_ms;
// }

void *phase_B(void *vargs)
{
    variables_t *var = (variables_t *)vargs;

    pthread_barrier_wait(&var->barrier);

    for (int i = 0; i < var->M; i++)
    {
        pthread_mutex_lock(&var->mutex);
        var->sum += var->arguments[i];
        pthread_mutex_unlock(&var->mutex);
    }

    return NULL;
}

void *phase_A(void *vargs)
{
    variables_t *var = (variables_t *)vargs;

    for (int i = 0; i < var->M; i++)
    {
        pthread_mutex_lock(&var->mutex);
        var->arguments[i] = (A * x(var->k, i));
        pthread_mutex_unlock(&var->mutex);
    }

    pthread_barrier_wait(&var->barrier);
    return NULL;
}

int main(/*int argc, char const *argv[]*/)
{
    // Used to time how long the program takes
    // struct timespec start, finish;

    pthread_t sum, args;
    // double I = 10;
    double k = 1;
    double M = 100;
    // double N = 2;
    variables_t var;
    init_variables(&var, k, M);

    if (pthread_create(&sum, NULL, &phase_B, &var) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&args, NULL, &phase_A, &var) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pthread_join(args, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (pthread_join(sum, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Normal result: %f\n", r(k, M));
    printf("Thread result: %f\n", var.sum);

    destroy_variables(&var);

    return 0;
}
