#include "graphics.h"

int init_graphics(graphics_t *context)
{
    SDL_Surface *one_arm_image, *objects_image, *coin_image;

    // init SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Error with SDL_Init: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // init SDL2_image
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        fprintf(stderr, "Error with IMG_Init: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // load the images
    one_arm_image = IMG_Load("./img/one_armed_bandit.png");
    objects_image = IMG_Load("./img/objects.png");
    coin_image = IMG_Load("./img/coin.png");

    if (one_arm_image == NULL || objects_image == NULL || coin_image == NULL)
    {
        fprintf(stderr, "Error with IMG_Load: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // load rects
    SDL_GetClipRect(one_arm_image, &context->one_arm_rect);
    SDL_GetClipRect(objects_image, &context->objects_rect);
    SDL_GetClipRect(coin_image, &context->coin_rect);

    // create the main window according to the image size
    if ((context->window = SDL_CreateWindow(
             "one_armed_bandit",
             SDL_WINDOWPOS_UNDEFINED,
             SDL_WINDOWPOS_UNDEFINED,
             one_arm_image->w,
             one_arm_image->h,
             SDL_WINDOW_OPENGL)) == NULL)
    {
        fprintf(stderr, "Error with SDL_CreateWindow: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // create the main renderer
    if ((context->renderer = SDL_CreateRenderer(context->window, -1, 0)) == NULL)
    {
        fprintf(stderr, "Error with SDL_CreateRenderer: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // allow transparent mode
    SDL_SetRenderDrawBlendMode(context->renderer, SDL_BLENDMODE_BLEND);

    // load the textures
    context->one_arm_texture = SDL_CreateTextureFromSurface(context->renderer, one_arm_image);
    context->objects_texture = SDL_CreateTextureFromSurface(context->renderer, objects_image);
    context->coin_texture = SDL_CreateTextureFromSurface(context->renderer, coin_image);

    if (context->one_arm_texture == NULL || context->objects_texture == NULL || context->coin_texture == NULL)
    {
        fprintf(stderr, "Error with SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // free the images as we don't need those anymore once textures are created
    SDL_FreeSurface(one_arm_image);
    SDL_FreeSurface(objects_image);
    SDL_FreeSurface(coin_image);

    return EXIT_SUCCESS;
}

void destroy_graphics(graphics_t *context)
{
    if (context != NULL)
    {
        SDL_DestroyTexture(context->one_arm_texture);
        SDL_DestroyTexture(context->objects_texture);
        SDL_DestroyTexture(context->coin_texture);
        SDL_DestroyRenderer(context->renderer);
        SDL_DestroyWindow(context->window);
        SDL_Quit();
    }
}