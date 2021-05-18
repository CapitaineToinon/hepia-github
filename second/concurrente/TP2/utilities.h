#ifndef __UTILS_H_SEEN_
#define __UTILS_H_SEEN_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>

/**
 * Get the image size from an image file
 * @param char* file name string
 * @param int* pointer on returned width
 * @param int* pointer on returned height
 */
void get_image_file_size(char *bmp_filename, int *im_width, int *im_height);

/**
 * Create a SDL texture from an image file for the specified renderer
 * @param SDL_Renderer* pointer on the SDL renderer
 * @param char* file name string
 * @param SDL_Rect* pointer on the image dimmensions
 * @return SDL_Texture*
 */
SDL_Texture *create_texture_from_image_file(SDL_Renderer *renderer, const char *img_filename, SDL_Rect *img_rect);

/**
 * Wait for any key to be released (SDL based function)
 */
void wait_key_release();

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
