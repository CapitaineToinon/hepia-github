#include <stdbool.h>
#include <semaphore.h>
#include <signal.h>
#include "utilities.h"
#include "render.h"
#include "game.h"

#define UPDATE_MS 20

// functions
void cancel_wheel_func();
void cancel_display_func();
void cancel_keyboard_func();
void try_pthread_cancel(pthread_t);
int init_signals();
void signal_handler();

// listen to SIGINT and SIGHUP to properly cancel the program
struct sigaction sa_sigint;
struct sigaction sa_sighup;

// thread data
pthread_t display_func_thread = 0;
pthread_t keyboard_func_thread = 0;
pthread_t wheel_func_threads[WHEEL_COUNT] = {0};

// wheels data
sem_t start_mutex;
sem_t stop_mutex[WHEEL_COUNT];
int wheel_index = 0;
int next_wheel_stop = 0;

// game state that will be mutated to reflect changes on the UI
game_state_t state;

void *wheel_func(void *vargs)
{
    (void)vargs; // no need for vargs

    // data needed at the end of the wheel scroll
    graphics_t g = state.graphics;
    int object_height = g.objects_rect.h / (OBJECT_COUNT + OBJECT_PADDING_COUNT);
    int max_y = object_height * OBJECT_COUNT;

    // dynamically get the wheel_index
    pthread_mutex_lock(&state.mutex);
    int index = wheel_index++;
    pthread_mutex_unlock(&state.mutex);

    while (true) // thread is meant to be cancelled with pthread_cancel
    {
        printf("Wheel %d is waiting...\n", index);
        sem_wait(&start_mutex);
        printf("Wheel %d is spinning\n", index);

        while (true)
        {
            if (sem_trywait(&stop_mutex[index]))
            {
                // make the wheel spin while we're waiting for spacebar input
                pthread_mutex_lock(&state.mutex);
                state.wheels_offsets[index] += 2;
                pthread_mutex_unlock(&state.mutex);
                msleep(6 - index * 2);
            }
            else
            {
                // spacebar was pressed
                break;
            }
        }

        // stop requested but we keep spinning the wheel until the next
        // object is properly aligned
        int objects_offset = state.wheels_offsets[index] % max_y;
        int object_offset = objects_offset % object_height;
        int remaining_offset = object_height - object_offset;

        for (int i = 0; i < remaining_offset; i += 2)
        {
            pthread_mutex_lock(&state.mutex);
            state.wheels_offsets[index] += 2;
            pthread_mutex_unlock(&state.mutex);
            msleep(6 - index * 2);
        }

        printf("Wheel %d is done spinning\n", index);
    }

    return NULL;
}

void *display_func(void *vargs)
{
    (void)vargs;
    struct timespec ts;

    while (true) // thread is meant to be cancelled with pthread_cancel
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

    bool quitting = false;

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
                printf("ESC or Q pressed\n");
                cancel_display_func();
                cancel_wheel_func();
                quitting = true;
                break;
            case SDLK_SPACE:
                printf("SPACE pressed\n");

                // spacebar does nothing if wheels aren't spinning
                if (state.wheels_state == IDLE)
                    break;

                // stop the next wheen if possible
                if (next_wheel_stop < WHEEL_COUNT)
                {
                    sem_post(&stop_mutex[next_wheel_stop]);
                    pthread_mutex_lock(&state.mutex);
                    next_wheel_stop++;
                    pthread_mutex_unlock(&state.mutex);

                    // all the wheels are stopped now, we're back to idle state
                    if (next_wheel_stop >= WHEEL_COUNT)
                        state.wheels_state = IDLE;
                }

                break;
            case SDLK_s:
                printf("S pressed\n");

                // exit if player has no coins left
                if (state.player_coins <= 0)
                    break;

                // exit if a game is already going on
                if (state.wheels_state == SPINNING)
                    break;

                pthread_mutex_lock(&state.mutex);
                state.player_coins -= 1;
                pthread_mutex_unlock(&state.mutex);

                // start all the wheels to spin
                next_wheel_stop = 0;
                state.wheels_state = SPINNING;
                for (int i = 0; i < WHEEL_COUNT; i++)
                    sem_post(&start_mutex);

                break;
            }

            wait_key_release();
            break;
        }
    }

    return NULL;
}

int main()
{
    // listen to signals to properly close the game
    if (init_signals() != 0)
    {
        fprintf(stderr, "Failed to init signals in wsh_loop().\n");
        exit(EXIT_FAILURE);
    }

    // init the mutable game variable
    if (init_game(&state) != 0) // import all the assets and create the SDL2 renderer and game context containing the current game state
    {
        fprintf(stderr, "Error with init_game_state\n");
        exit(EXIT_FAILURE);
    }

    // wheel mutexes
    sem_init(&start_mutex, 0, 0); // global mutex to start all the wheels at once
    for (int i = 0; i < WHEEL_COUNT; i++)
        sem_init(&stop_mutex[i], 0, 0); // global mutex to stop each wheel

    // display_func_thread
    if (pthread_create(&display_func_thread, NULL, &display_func, NULL) != 0)
    {
        fprintf(stderr, "Error with pthread_create: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // wheel_func_threads
    for (int i = 0; i < WHEEL_COUNT; i++)
    {
        if (pthread_create(&wheel_func_threads[i], NULL, &wheel_func, NULL) != 0)
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

void cancel_wheel_func()
{
    for (int i = 0; i < WHEEL_COUNT; i++)
    {
        try_pthread_cancel(wheel_func_threads[i]);
    }
}

void cancel_display_func()
{
    try_pthread_cancel(display_func_thread);
}

void cancel_keyboard_func()
{
    try_pthread_cancel(keyboard_func_thread);
}

void try_pthread_cancel(pthread_t id)
{
    if (id != 0)
    {
        if (pthread_cancel(id) != 0)
        {
            fprintf(stderr, "Failed to cancel thread with pid %ld: %s\n", id, strerror(errno));
            // just inform of the error but don't exit or return here
        }
    }
}

int init_signals()
{
    // SIGHUP
    sa_sighup.sa_handler = signal_handler;
    sigemptyset(&sa_sighup.sa_mask);
    sa_sighup.sa_flags = SA_RESTART;

    if (sigaction(SIGHUP, &sa_sighup, NULL) == -1)
    {
        fprintf(stderr, "Failed to sigaction: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // SIGINT
    sa_sigint.sa_handler = signal_handler;
    sigemptyset(&sa_sigint.sa_mask);
    sa_sigint.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa_sigint, NULL) == -1)
    {
        fprintf(stderr, "Failed to sigaction: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void signal_handler()
{
    // cancel all the thread on signal,
    // main will take care of cleanup
    cancel_wheel_func();
    cancel_display_func();
    cancel_keyboard_func();
}
