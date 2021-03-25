#include "helper.h"

/**
 * Converts a string to a int
 * but exit and sends error to stderr
 * if failed, unlike the regular strtol
 */ 
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