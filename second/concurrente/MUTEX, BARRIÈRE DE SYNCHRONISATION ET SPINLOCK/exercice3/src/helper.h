#ifndef __HELPER_H_SEEN__
#define __HELPER_H_SEEN__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

/**
 * Converts a string to a int but exit and sends error to stderr if failed, unlike the regular strtol
 * 
 * @param string The string to convert to an int
 * @returns Converted int
 */
int stringtoi(const char *);

/**
 * Get the total elaspsed miliseconds between 2 timespec
 * 
 * @param start start timespec
 * @param end end timespec
 */
double get_elapsed_ms(struct timespec, struct timespec);

#endif