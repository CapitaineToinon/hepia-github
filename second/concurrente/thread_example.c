#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *custom_thread(void *vargs)
{
    printf("THREAD start\n");
    sleep(5);
    printf("THREAD end\n");
    return NULL;
}

int main()
{
    // compile with:
    // gcc thread_example.c -o thread_example -lpthread
    pthread_t id;
    printf("Before calling thread\n");
    pthread_create(&id, NULL, &custom_thread, NULL);
    printf("During the thread calling thread\n");
    sleep(1);
    printf("I can do things here\n");
    sleep(1);
    printf("Now waiting for threads to end...\n");
    pthread_join(id, NULL);
    printf("After thread is over\n");
}