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
 * Passive waiting from a timestamp for a duration of offset_ms
 * @param timespec start timestamp. Can be get at first with clock_gettime(CLOCK_MONOTONIC, &time_start)
 * @param long duration of the passive waiting [ms]
 * @return timespec
 */
struct timespec wait_until(struct timespec time_start, unsigned long offset_ms);

/**
 * Calculate the difference of time between a timestamp and present time
 * @param timespec past start timestamp. Can be get at first with clock_gettime(CLOCK_MONOTONIC, &time_start)
 * @return long time elapsed between time_start and now [ms]
 */
long timespec_diff_ms(struct timespec time_start);

#endif
