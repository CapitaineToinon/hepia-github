
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
// #include <unistd.h>
// #include <pthread.h>
// #include <semaphore.h>
#include "utilities.h"
// #include "display.h"
#include "graphics.h"

#define OBJECT_NB 9
#define WHEEL_NB 3
#define COIN_NB 5

typedef struct game_state_t
{
    graphics_t graphics;
    int coins;
} game_state_t;

int init_game_state(game_state_t *state)
{
    state->coins = COIN_NB;
    return init_graphics(&state->graphics);
}

void destroy_game_state(game_state_t *state)
{
    if (state != NULL)
    {
        destroy_graphics(&state->graphics);
    }
}

void draw_initial_state(game_state_t *state)
{
    graphics_t g = state->graphics;

    int object_height = g.objects_rect.h / OBJECT_NB;
    SDL_Rect coin_rect_pos = {700, 1020, g.coin_rect.w, g.coin_rect.h};

    SDL_RenderCopy(g.renderer, g.one_arm_texture, NULL, &g.one_arm_rect);
    SDL_RenderCopy(g.renderer, g.coin_texture, NULL, &coin_rect_pos);

    for (int i = 0; i < COIN_NB; i++)
    {
        SDL_Rect coin_rect_pos = {700, 400 - 10 * i, g.coin_rect.w, g.coin_rect.h};
        SDL_RenderCopy(g.renderer, g.coin_texture, NULL, &coin_rect_pos);
    }

    SDL_Rect src_rect, dst_rect = g.objects_rect;
    dst_rect.h = object_height * 1.5; // display 1.5 object on screen for a wheel
    src_rect = dst_rect;
    src_rect.x = 0;

    // src_rect.y is positionned here on the 2nd object of objects.png
    src_rect.y = object_height;
    dst_rect.x = 85;
    dst_rect.y = 410 - object_height / 2; // setup the coord. of the icon in the global renderer
    SDL_RenderCopy(g.renderer, g.objects_texture, &src_rect, &dst_rect);
    SDL_RenderPresent(g.renderer);
}

void update_coins(game_state_t *state)
{
    (void)state;
}

// Main event loop
void loop(game_state_t *state)
{
    (void)state;
    SDL_Event event;
    bool quit = false;

    while (!quit)
    {
        if (SDL_WaitEvent(&event) == 0)
        {
            fprintf(stderr, "Error with SDL_WaitEvent: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        switch (event.type)
        {
        case SDL_QUIT:
            // if SDL_Quit() called
            quit = true;
            break;
        case SDL_WINDOWEVENT:
            // if windows closed by user
            quit = event.window.event == SDL_WINDOWEVENT_CLOSE;
            break;
        case SDL_KEYDOWN:
            // if key pressed
            switch (event.key.keysym.sym)
            {
            case SDLK_q:
            case SDLK_ESCAPE:
                quit = true;
                break;
            case SDLK_SPACE:
                printf("Spaced pressed\n");
                // wait_key_release();
                break;
            case SDLK_s:
                printf("S pressed\n");
                // wait_key_release();
                break;
            }
            break;
        }
    }
}

int main()
{
    game_state_t state;

    if (init_game_state(&state) == EXIT_FAILURE) // import all the assets and create the SDL2 renderer and game context containing the current game state
    {
        fprintf(stderr, "Error with init_game_state\n");
        exit(EXIT_FAILURE);
    }

    draw_initial_state(&state); // draw the initial position of everything on the game
    loop(&state);               // main even loop that listens for keyboard events
    destroy_game_state(&state); // destroy context once we're done
    return EXIT_SUCCESS;
}
