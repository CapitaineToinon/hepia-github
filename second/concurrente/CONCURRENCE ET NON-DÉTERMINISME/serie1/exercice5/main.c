#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#define MAX_STRING_SIZE 256

// Recursive 
char *strtoupper(char *string)
{
    size_t len = strlen(string);
    char *buffer = (char *)malloc((len + 1) * sizeof(char));

    buffer[0] = toupper(string[0]);

    if (len > 0)
    {
        char *sub = strtoupper(string + 1);
        strncpy(buffer + 1, sub, strlen(sub));
        free(sub);
    }

    buffer[len] = 0;
    return buffer;
}

// Recusrive thread, not working I think?
void *strtoupper_thread(void *vargs)
{
    char *string = (char *)vargs;
    size_t len = strlen(string);
    char *buffer = (char *)malloc((len + 1) * sizeof(char));

    buffer[0] = toupper(string[0]);

    if (len == 0)
    {
        return buffer;
    }

    pthread_t id;
    if (pthread_create(&id, NULL, &strtoupper_thread, string + 1) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    void *thread_result;
    if (pthread_join(id, &thread_result) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    char *sub = (char *)thread_result;
    strncpy(buffer + 1, sub, strlen(sub));
    free(sub);

    buffer[len] = 0;
    return (void *)buffer;
}

int main()
{
    char *upper = strtoupper("hello");
    printf("%s\n", upper);
    free(upper);

    pthread_t id;
    if (pthread_create(&id, NULL, &strtoupper_thread, "world") != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    void *thread_result;
    if (pthread_join(id, &thread_result) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    upper = (char *)thread_result;
    printf("%s\n", upper);
    free(upper);
    return 0;
}