#include "helper.h"

int stringtoi(const char *__str)
{
    errno = 0;
    char *endptr;
    int value = (int)strtol(__str, &endptr, 0);

    if (errno != 0)
    {
        fprintf(stderr, "Error trying to convert string '%s' to number: %s\n", __str, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (__str == endptr)
    {
        fprintf(stderr, "Error trying to convert string '%s' to number\n", __str);
        exit(EXIT_FAILURE);
    }

    return value;
}

double get_elapsed_ms(struct timespec start, struct timespec finish)
{
    double elapsed_ms = 1000 * (finish.tv_sec - start.tv_sec);
    elapsed_ms += (finish.tv_nsec - start.tv_nsec) / 1000000.0;
    return elapsed_ms;
}