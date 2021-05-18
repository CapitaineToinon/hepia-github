
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
// #include <unistd.h>
#include <pthread.h>
// #include <semaphore.h>
#include "utilities.h"
// #include "display.h"
#include "graphics.h"
#include "game.h"

#define UPDATE_MS 20

void *update_thread(void *vargs)
{
    (void)vargs;
    struct timespec ts;

    while (true) // this thread is meant to be killed by
    {
        update_game();
        clock_gettime(CLOCK_REALTIME, &ts);
        msleep(UPDATE_MS);
    }

    return NULL;
}

void event_loop()
{
    SDL_Event event;
    bool quit = false;

    while (!quit)
    {
        if (SDL_WaitEvent(&event) == 0)
        {
            fprintf(stderr, "Error with SDL_WaitEvent: %s\n", SDL_GetError());
            quit = true;
            continue;
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
                break;
            case SDLK_s:
                printf("S pressed\n");
                spend_coin();
                break;
            }
            break;
        }
    }
}

int main()
{
    pthread_t update_loop;

    if (init_game() == EXIT_FAILURE) // import all the assets and create the SDL2 renderer and game context containing the current game state
    {
        fprintf(stderr, "Error with init_game_state\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&update_loop, NULL, &update_thread, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    event_loop();

    // if we're out of the event loop, it's that we need to close the program
    pthread_cancel(update_loop);

    if (pthread_join(update_loop, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_join: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    destroy_game(); // destroy context once we're done

    return EXIT_SUCCESS;
}
