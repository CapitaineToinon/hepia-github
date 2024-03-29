#include "render.h"

// private functions
void render_background(game_state_t *state);
void render_coins(game_state_t *state);
void render_wheels(game_state_t *state);

unsigned int wheel_flicker = 20;
unsigned int render_count = 0;

// update the game graphics
void render_game(game_state_t *state)
{
    render_background(state);
    render_wheels(state);
    render_coins(state);
    SDL_RenderPresent(state->graphics.renderer);
}

void render_background(game_state_t *state)
{
    graphics_t g = state->graphics;

    // upgrade background
    SDL_RenderCopy(g.renderer, g.one_arm_texture, NULL, &g.one_arm_rect);
}

void render_coins(game_state_t *state)
{
    graphics_t g = state->graphics;

    int player_coins = state->player_coins;
    int bank_coins = state->bank_coins;

    // remaining coins
    for (int i = 0; i < player_coins; i++)
    {
        SDL_Rect coin_rect_pos = {700, 400 - 10 * i, g.coin_rect.w, g.coin_rect.h};
        SDL_RenderCopy(g.renderer, g.coin_texture, NULL, &coin_rect_pos);
    }

    // spent coins
    for (int i = 0; i < bank_coins; i++)
    {
        SDL_Rect coin_rect_pos = {700, 1020 - 10 * i, g.coin_rect.w, g.coin_rect.h};
        SDL_RenderCopy(g.renderer, g.coin_texture, NULL, &coin_rect_pos);
    }
}

void render_wheels(game_state_t *state)
{
    graphics_t g = state->graphics;
    int object_height = g.objects_rect.h / (OBJECT_COUNT + OBJECT_PADDING_COUNT);

    for (int i = 0; i < WHEEL_COUNT; i++)
    {
        SDL_Rect src_rect, dst_rect = g.objects_rect;
        dst_rect.h = object_height * 1.5; // display 1.5 object on screen for a wheel
        src_rect = dst_rect;
        src_rect.x = 0;

        // src_rect.y is positionned here on the 2nd object of objects.png
        int max_y = object_height * OBJECT_COUNT;
        src_rect.y = state->wheels_offsets[i] % max_y;

        // position of the wheel on the graph
        dst_rect.x = 85 + (g.objects_rect.w) * i;
        dst_rect.y = 410 - object_height / 2; // setup the coord. of the icon in the global renderer
        SDL_RenderCopy(g.renderer, g.objects_texture, &src_rect, &dst_rect);

        // flickering for winning wheels
        if (state->wheels_flicker[i] && render_count % (wheel_flicker * 2) < wheel_flicker)
        {
            SDL_SetRenderDrawColor(g.renderer, 255, 255, 255, 128);
            SDL_Rect white_square;
            white_square.x = dst_rect.x;
            white_square.y = dst_rect.y;
            white_square.h = object_height;
            white_square.w = dst_rect.w;
            SDL_RenderFillRect(g.renderer, &white_square);
        }
    }

    render_count++;
}