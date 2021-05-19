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

enum WheelsState
{
    IDLE,
    SPINNING,
};

typedef struct game_state_t
{
    pthread_mutex_t mutex;
    graphics_t graphics;
    enum WheelsState wheels_state;
    pthread_t wheels_ids[WHEEL_COUNT];
    int wheels_offsets[WHEEL_COUNT];
    int player_coins;
    int bank_coins;
} game_state_t;

int init_game(game_state_t *);
void destroy_game(game_state_t *);

#endif
