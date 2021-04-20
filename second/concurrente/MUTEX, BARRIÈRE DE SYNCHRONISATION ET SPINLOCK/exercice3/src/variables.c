#include "variables.h"

void init_variables(variables_t *variables, int k, int M)
{
    variables->k = k;
    variables->M = M;
    variables->sum = 0;
    variables->arguments = malloc(sizeof(double) * M);
    pthread_mutex_init(&variables->mutex, NULL);
    pthread_barrier_init(&variables->barrier, NULL, 2);
}

void destroy_variables(variables_t *var)
{
    free(var->arguments);
    pthread_mutex_destroy(&var->mutex);
    pthread_barrier_destroy(&var->barrier);
}