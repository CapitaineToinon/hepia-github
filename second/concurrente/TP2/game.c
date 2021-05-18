#include "game.h"

game_state_t state;

int init_game()
{
    state.player_coins = PLAYERS_COIN_COUNT;
    state.bank_coins = BANK_COIN_COUNT;
    return init_graphics(&state.graphics);
}

void destroy_game()
{
    destroy_graphics(&state.graphics);
}

void spend_coin()
{
    if (state.player_coins <= 0)
    {
        return;
    }

    state.player_coins -= 1;
}

void update_coins()
{
    graphics_t g = state.graphics;

    // remaining coins
    for (int i = 0; i < state.player_coins; i++)
    {
        SDL_Rect coin_rect_pos = {700, 400 - 10 * i, g.coin_rect.w, g.coin_rect.h};
        SDL_RenderCopy(g.renderer, g.coin_texture, NULL, &coin_rect_pos);
    }

    // spent coins
    for (int i = 0; i < state.bank_coins; i++)
    {
        SDL_Rect coin_rect_pos = {700, 1020 - 10 * i, g.coin_rect.w, g.coin_rect.h};
        SDL_RenderCopy(g.renderer, g.coin_texture, NULL, &coin_rect_pos);
    }
}

// update the game graphics
void update_game()
{
    graphics_t g = state.graphics;

    // upgrade background
    SDL_RenderCopy(g.renderer, g.one_arm_texture, NULL, &g.one_arm_rect);

    // update coins
    update_coins();

    // update wheels @TODO
    int object_height = g.objects_rect.h / OBJECT_NB;
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