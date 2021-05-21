#ifndef __UTILS_H_SEEN_
#define __UTILS_H_SEEN_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>

/**
 * Wait for any key to be released (SDL based function)
 */
void wait_key_release();

/**
 * msleep -- suspend execution for an interval of time
 * @param int sleep milliseconds
 * @return The sleep utility exits 0 on success, and >0 if an error occurs.
 */
int msleep(unsigned int tms);

/**
 * Calculate the difference of time between a timestamp and present time
 * @param timespec past start timestamp. Can be get at first with clock_gettime(CLOCK_MONOTONIC, &time_start)
 * @return long time elapsed between time_start and now [ms]
 */
long timespec_diff_ms(struct timespec time_start);

bool are_values_equal(const int a[], int n);
int cmp_int(const void *a, const void *b);
int count_occurrences(int arr[], int n, int x);

#endif
