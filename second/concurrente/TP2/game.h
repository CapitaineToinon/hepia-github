#ifndef __GAME_H_SEEN__
#define __GAME_H_SEEN__

#include <pthread.h>
#include <stdbool.h>
#include "graphics.h"

#define OBJECT_COUNT 7
#define OBJECT_PADDING_COUNT 2
#define WHEEL_COUNT 3
#define PLAYERS_COIN_COUNT 10
#define BANK_COIN_COUNT 30
#define GAME_PRICE 1

typedef struct game_state_t
{
    pthread_mutex_t mutex;
    graphics_t graphics;
    bool is_idle;
    pthread_t wheels_ids[WHEEL_COUNT];
    int wheels_offsets[WHEEL_COUNT];
    int wheels_results[WHEEL_COUNT];
    bool wheels_flicker[WHEEL_COUNT];
    int player_coins;
    int bank_coins;
} game_state_t;

/**
 * Init a game_state_t object
 * Needs to call destroy_game before quitting
 * 
 * @return 0 if no error
 */
int init_game(game_state_t *);

/**
 * Destroys a game_state_t object
 */
void destroy_game(game_state_t *);

#endif
