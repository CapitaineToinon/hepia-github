#include <stdbool.h>
#include <semaphore.h>
#include <signal.h>
#include <math.h>
#include "utilities.h"
#include "barrier.h"
#include "render.h"
#include "game.h"

#define UPDATE_MS 20
#define SIMPLE_WIN_COUNT 2

// functions
void cancel_wheel_func();
void cancel_display_func();
void cancel_keyboard_func();
void try_pthread_cancel(pthread_t);
int init_signals();
void signal_handler();

// game functions
void try_start_wheels();
void try_stop_wheel();
void set_winning_wheels(int number);

// listen to SIGINT and SIGHUP to properly cancel the program
struct sigaction sa_sigint;
struct sigaction sa_sighup;

// thread data
pthread_t display_func_thread = 0;
pthread_t keyboard_func_thread = 0;
pthread_t victory_func_thread = 0;
pthread_t wheel_func_threads[WHEEL_COUNT] = {0};

// wheels data
sem_t start_mutex;
sem_t stop_mutex[WHEEL_COUNT];
pthread_barrier_t finish_barrier;
int wheel_index = 0;
int next_wheel_stop = 0;

// game state that will be mutated to reflect changes on the UI
game_state_t state;

/**
 * Thread function reponsable for spinning a wheel.
 * Thread is meant to be cancelled with pthread_cancel.
 * 
 * Codeflow is :
 * 1. thread spawns, gets its own index automatically
 * 2. wait on a semaphore to start all the wheels to spin at the same time
 * 3. wheel starts spinning, wait for an interput
 * 4. once interput is requested, keeps spinning until the next object is aligned
 * 5. wait on a barrier common to all wheels to know when all wheels are done spinning
 * 6. goes back to 2.
 * 
 * @param void* vargs (unused)
 * @return NULL
 */
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

        // now that we're done spinning, figure out what object
        // we stopped at and store the index
        int object_index = (state.wheels_offsets[index] / object_height) % OBJECT_COUNT;
        state.wheels_results[index] = object_index;

        // wait for all wheels to be done spinning
        pthread_barrier_wait(&finish_barrier);
    }

    return NULL;
}

/**
 * Thread function that refreshed the UI at 50hz, using SDL2.
 * Thread is meant to be cancelled with pthread_cancel.
 * If the thread is busy rendering when cancel is requested, the cancel will be delayed until render is done.
 * 
 * @param void* vargs (unused)
 * @return NULL
 */
void *display_func(void *vargs)
{
    (void)vargs;

    while (true) // thread is meant to be cancelled with pthread_cancel
    {
        // prevent the thread to be cancelled while rendering, otherwise
        // destroying assets or doing SDL_Quit will fail
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        render_game(&state);
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        msleep(UPDATE_MS);
    }

    return NULL;
}

/**
 * Thread function that listens to keyboard events.
 * Thread is meant to be cancelled with pthread_cancel.
 * 
 * @param void* vargs (unused)
 * @return NULL
 */
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
                quitting = true;
                break;
            case SDLK_SPACE:
                printf("SPACE pressed\n");
                try_stop_wheel();
                break;
            case SDLK_s:
                printf("S pressed\n");
                try_start_wheels();
                break;
            }

            wait_keyup();
            break;
        }
    }

    // quit was requested, cancel the other running threads
    cancel_display_func();
    cancel_wheel_func();

    return NULL;
}

/**
 * Tries to starts spinning the next wheel, if possible. 
 * Takes take of spending coins and start spinning all the wheels
 * at the start of the game.
 */
void try_start_wheels()
{
    // exit if player has no coins left
    if (state.player_coins <= 0)
        return;

    // exit if the bank has no coins left
    if (state.bank_coins <= 0)
        return;

    // exit if a game is already going on
    if (!state.is_idle)
        return;

    set_winning_wheels(-1);
    pthread_mutex_lock(&state.mutex);
    state.player_coins -= GAME_PRICE;
    state.bank_coins += GAME_PRICE;
    pthread_mutex_unlock(&state.mutex);

    // start all the wheels to spin
    next_wheel_stop = 0;
    state.is_idle = false;
    for (int i = 0; i < WHEEL_COUNT; i++)
        sem_post(&start_mutex);
}

/**
 * Tries stop spinning the next wheel, if possible
 * Takes take of rewarding the players with coins at the end of the game.
 */
void try_stop_wheel()
{
    // spacebar does nothing if wheels aren't spinning
    if (state.is_idle)
        return;

    // stop the next wheel
    sem_post(&stop_mutex[next_wheel_stop % WHEEL_COUNT]);
    pthread_mutex_lock(&state.mutex);
    next_wheel_stop++;
    pthread_mutex_unlock(&state.mutex);

    // Handle the end of the game
    if (next_wheel_stop >= WHEEL_COUNT)
    {
        pthread_mutex_lock(&state.mutex);
        next_wheel_stop = 0;
        pthread_mutex_unlock(&state.mutex);

        // wait for all the wheels to be done spinning
        pthread_barrier_wait(&finish_barrier);
        state.is_idle = true;

        // simple win (overly complicated for such a simple check but I didn't want to hardcode the winning condition)
        // count the amount of occurences in the wheels and victory if the count is greated than SIMPLE_WIN_COUNT
        int copy[WHEEL_COUNT];
        memcpy(copy, state.wheels_results, sizeof(state.wheels_results));
        qsort(copy, WHEEL_COUNT, sizeof(int), cmp_int);

        for (int i = 0; i < OBJECT_COUNT; i++)
        {
            // detect a potential win
            // victory conditions are currently hard coded
            if (count_occurrences(copy, WHEEL_COUNT, i) == WHEEL_COUNT)
            {
                // jackpot
                int half = state.bank_coins / 2;
                pthread_mutex_lock(&state.mutex);
                state.player_coins += half;
                state.bank_coins -= half;
                pthread_mutex_unlock(&state.mutex);
                set_winning_wheels(i);
                printf("JACKPOT, winning %d coins\n", half);
                break;
            }

            if (count_occurrences(copy, WHEEL_COUNT, i) == SIMPLE_WIN_COUNT)
            {
                pthread_mutex_lock(&state.mutex);

                // prevent winnings bigger than the remaining coins in the bank
                int wins = 2 * GAME_PRICE;
                if (wins > state.bank_coins)
                {
                    wins = state.bank_coins;
                }

                state.bank_coins -= wins;
                state.player_coins += wins;
                pthread_mutex_unlock(&state.mutex);
                set_winning_wheels(i);
                printf("WIN, winning %d coins\n", wins);
                break;
            }
        }
    }
}

/**
 * Set flags for if a wheel is a winning wheel or not.
 * Used for the UI to add a blinking effect on winning wheels
 * 
 * @param int the number of the object that is a winning object
 */
void set_winning_wheels(int number)
{
    for (int i = 0; i < WHEEL_COUNT; i++)
        state.wheels_flicker[i] = state.wheels_results[i] == number;
}

/**
 * Tries to cancel the wheel threads
 */
void cancel_wheel_func()
{
    // first request all the wheels to stop to prevent the barrier block
    // do it WHEEL_COUNT + 1 to be sure the barrier is gone
    // I do this because I didn't find a way to cancel the barrier
    // using pthread_cancel. Maybe there is a better way to do this
    for (int i = 0; i < WHEEL_COUNT + 1; i++)
    {
        try_stop_wheel();
    }

    for (int i = 0; i < WHEEL_COUNT; i++)
    {
        try_pthread_cancel(wheel_func_threads[i]);
        printf("cancel_wheel_func OK\n");
    }
}

/**
 * Tries to cancel the display thread
 */
void cancel_display_func()
{
    try_pthread_cancel(display_func_thread);
    printf("cancel_display_func OK\n");
}

/**
 * Trie to cnacel the keyboard thread
 */
void cancel_keyboard_func()
{
    try_pthread_cancel(keyboard_func_thread);
    printf("cancel_keyboard_func OK\n");
}

/**
 * Tries to cancel a thread
 * 
 * @param pthread_t the pid of the thread to cancel
 */
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

/**
 * Initializes the signal handlers for SIGINT and SIGHUP
 * 
 * @return 0 if no error
 */
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
    sa_sighup.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa_sigint, NULL) == -1)
    {
        fprintf(stderr, "Failed to sigaction: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
 * Cancel all running threads upon signal 
 */
void signal_handler()
{
    // cancel all the thread on signal,
    // main will take care of cleanup
    cancel_wheel_func();
    cancel_display_func();
    cancel_keyboard_func();
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

    // init wheel related stuff
    sem_init(&start_mutex, 0, 0); // global mutex to start all the wheels at once
    pthread_barrier_init(&finish_barrier, NULL, WHEEL_COUNT + 1);
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

    pthread_barrier_destroy(&finish_barrier);
    destroy_game(&state); // destroy context once we're done
    return EXIT_SUCCESS;
}