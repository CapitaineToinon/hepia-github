#include <stdbool.h>
#include <semaphore.h>
#include "utilities.h"
#include "render.h"
#include "game.h"

#define UPDATE_MS 20

game_state_t state;

bool spinning = false;
bool quitting = false;

void *wheel_func(void *vargs)
{
    // todo not do that apparently
    int *index = (int *)vargs;

    printf("Index %d\n", *index);

    pthread_mutex_lock(&state.mutex);
    state.wheels_offsets[*index] += 2;
    pthread_mutex_unlock(&state.mutex);
    msleep(6 - (*index) * 2);

    return NULL;
}

void *display_func(void *vargs)
{
    (void)vargs;
    struct timespec ts;

    while (!quitting)
    {
        render_game(&state);
        clock_gettime(CLOCK_REALTIME, &ts);
        msleep(UPDATE_MS);
    }

    return NULL;
}

void *keyboard_func(void *vargs)
{
    (void)vargs;
    SDL_Event event;

    while (!quitting)
    {
        if (SDL_WaitEvent(&event) == 0)
        {
            fprintf(stderr, "Error with SDL_WaitEvent: %s\n", SDL_GetError());
            quitting = true;
            continue;
        }

        switch (event.type)
        {
        case SDL_QUIT:
            // if SDL_Quit() called
            quitting = true;
            break;
        case SDL_WINDOWEVENT:
            // if windows closed by user
            quitting = event.window.event == SDL_WINDOWEVENT_CLOSE;
            break;
        case SDL_KEYDOWN:
            // if key pressed
            switch (event.key.keysym.sym)
            {
            case SDLK_q:
            case SDLK_ESCAPE:
                quitting = true;
                break;
            case SDLK_SPACE:
                printf("Spaced pressed\n");
                state.wheels_state = state.wheels_state == SPINNING ? IDLE : SPINNING;
                break;
            case SDLK_s:
                printf("S pressed\n");
                try_spend_coin(&state);
                break;
            }
            break;
        }
    }

    return NULL;
}

int main()
{
    pthread_t display_func_thread;
    pthread_t keyboard_func_thread;
    pthread_t wheel_func_threads[WHEEL_COUNT];
    int wheel_indexes[WHEEL_COUNT];

    if (init_game(&state) == EXIT_FAILURE) // import all the assets and create the SDL2 renderer and game context containing the current game state
    {
        fprintf(stderr, "Error with init_game_state\n");
        exit(EXIT_FAILURE);
    }

    // wheel_indexes
    for (int i = 0; i < WHEEL_COUNT; i++)
    {
        wheel_indexes[i] = i;
    }

    // display_func_thread
    if (pthread_create(&display_func_thread, NULL, &display_func, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // wheel_func_threads
    for (int i = 0; i < WHEEL_COUNT; i++)
    {
        if (pthread_create(&wheel_func_threads[i], NULL, &wheel_func, &wheel_indexes[i]) != 0)
        {
            fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    // keyboard_func_thread
    if (pthread_create(&keyboard_func_thread, NULL, &keyboard_func, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // display_func_thread
    if (pthread_join(display_func_thread, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // wheel_func_threads
    for (int i = 0; i < WHEEL_COUNT; i++)
    {
        if (pthread_join(wheel_func_threads[i], NULL) != 0)
        {
            fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    // keyboard_func_thread
    if (pthread_join(keyboard_func_thread, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    destroy_game(&state); // destroy context once we're done
    return EXIT_SUCCESS;
}
