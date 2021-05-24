#include "game.h"

int init_game(game_state_t *state)
{
    state->is_idle = true;
    state->player_coins = PLAYERS_COIN_COUNT;
    state->bank_coins = BANK_COIN_COUNT;

    memset(state->wheels_offsets, 0, WHEEL_COUNT * sizeof(int));
    memset(state->wheels_results, 0, WHEEL_COUNT * sizeof(int));
    memset(state->wheels_flicker, false, WHEEL_COUNT * sizeof(bool));
    pthread_mutex_init(&state->mutex, NULL);

    return init_graphics(&state->graphics);
}

void destroy_game(game_state_t *state)
{
    if (state == NULL)
        return;

    destroy_graphics(&state->graphics);
}
