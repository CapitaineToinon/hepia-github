#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include "utilities.h"

void *display_func(void *param);

// height of one single object
extern int object_height; // global for convenience

#endif