// pseudo code, not meant to be compiled
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>

void get_serving_from_pot();
void put_servings_in_pot(int n);
void eat();

#define N 100

int readers = 0;
sem_t empty;
pthread_mutex_t mutex;

unsigned int left = N;

void cannibal()
{
    while (true)
    {
        pthread_mutex_lock(&mutex);
        readers++;

        if (readers ==)

            get_serving_from_pot();
        pthread_mutex_unlock(&mutex);
        eat();
    }
}

void *chef(void *vargs)
{
    while (true)
    {
        sem_wait(&empty);
        put_servings_in_pot(N);
        sem_post(&empty);
    }
}

int main(void)
{
    sem_init(&empty, 0, 1);
    return 0;
}
