#ifndef __GRAPHICS_H_SEEN__
#define __GRAPHICS_H_SEEN__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

typedef struct graphics_t
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *one_arm_texture, *objects_texture, *coin_texture;
    SDL_Rect one_arm_rect, objects_rect, coin_rect;
} graphics_t;

/**
 * Creates and import all the needed files for the game to work
 * 
 * @warning needs to call destroy_context when done using the structure created by this function
 * @return context_t
 */
int init_graphics(graphics_t *);

/**
 * Destroyes all objects and free the context
 */
void destroy_graphics(graphics_t *);

#endif
