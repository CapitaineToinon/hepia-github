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
void wait_keyup();

/**
 * msleep -- suspend execution for an interval of time
 * @param int sleep milliseconds
 * @return The sleep utility exits 0 on success, and >0 if an error occurs.
 */
int msleep(unsigned int tms);

/**
 * Check of all values in a int array are equal
 * 
 * @param int[] the array to be tested
 * @param int the size of the array
 * @return if all values in a int array are equal
 */
bool are_values_equal(const int a[], int n);

/**
 * Function that compares to int
 * 
 * @param void* pointer to the first int
 * @param void* pointer to the second int
 * @return the int result of the compare
 */
int cmp_int(const void *a, const void *b);

/**
 * Counts how many occurrences of an int in an array of int
 * 
 * @param int[] the array to be tested
 * @param int the size of the array
 * @param int the number to be counted
 * @return the amount of occurrences found in the array
 */
int count_occurrences(int arr[], int n, int x);

#endif
