#include "barrier.h"

int barrier_init(barrier_t *barrier, int count)
{
    barrier->finished = 0;
    barrier->count = count;
    barrier->init = true;
    barrier->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    return EXIT_SUCCESS;
}

int barrier_wait(barrier_t *barrier)
{
    if (barrier == NULL)
    {
        errno = EINVAL;
        return EINVAL;
    }

    pthread_mutex_lock(&(barrier->mutex));
    printf("%d/%d are over\n", barrier->finished + 1, barrier->count);
    barrier->finished += 1;
    pthread_mutex_unlock(&(barrier->mutex));

    while (barrier->finished < barrier->count)
    {
        sleep(0);
    }

    return EXIT_SUCCESS;
}

int barrier_destroy(barrier_t *barrier)
{
    if (barrier != NULL)
    {
        barrier->init = false;
        pthread_mutex_destroy(&(barrier->mutex));
    }

    return 0;
}