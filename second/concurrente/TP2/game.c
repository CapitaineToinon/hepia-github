#include "game.h"

int init_game(game_state_t *state)
{
    state->wheels_state = IDLE;
    state->player_coins = PLAYERS_COIN_COUNT;
    state->bank_coins = BANK_COIN_COUNT;

    memset(state->wheels_offsets, 0, WHEEL_COUNT * sizeof(int));
    pthread_mutex_init(&state->mutex, NULL);

    return init_graphics(&state->graphics);
}

void destroy_game(game_state_t *state)
{
    if (state == NULL)
        return;

    destroy_graphics(&state->graphics);
}

void try_spend_coin(game_state_t *state)
{
    if (state == NULL)
        return;

    if (state->player_coins <= 0)
        return;

    pthread_mutex_lock(&state->mutex);
    state->player_coins -= 1;
    pthread_mutex_unlock(&state->mutex);
}
