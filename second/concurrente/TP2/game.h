#ifndef __GAME_H_SEEN__
#define __GAME_H_SEEN__

#include <pthread.h>
#include "graphics.h"

#define OBJECT_NB 9
#define WHEEL_NB 3
#define PLAYERS_COIN_COUNT 10
#define BANK_COIN_COUNT 30

typedef struct game_state_t
{
    graphics_t graphics;
    pthread_t update_loop;
    pthread_t event_loop;
    int player_coins;
    int bank_coins;
} game_state_t;

int init_game();
void destroy_game();
void update_game();

void spend_coin();

#endif
