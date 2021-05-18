#include "display.h"

// the following global variables are defined for graphical objects only:
// the images size is 104x128 pixels
static SDL_Renderer *renderer;
static SDL_Texture *one_arm_texture, *objects_texture, *coin_texture;
static SDL_Rect one_arm_rect, coin_rect;
static SDL_Rect object_rect;
int object_height; // global for convenience

void *display_func(void *param)
{
	(void)param; // unused param

	int one_arm_width, one_arm_height;
	SDL_Window *window;
	int i;

	// -------------------------
	// Graphic initialization
	// -------------------------

	assert(SDL_Init(SDL_INIT_VIDEO) == 0);

	get_image_file_size("./img/one_armed_bandit.png", &one_arm_width, &one_arm_height);

	window = SDL_CreateWindow(
		"one_armed_bandit",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		one_arm_width,
		one_arm_height,
		SDL_WINDOW_OPENGL);
	assert(window != NULL);

	renderer = SDL_CreateRenderer(window, -1, 0);
	assert(renderer != NULL);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // allow transparent mode!

	// create the global texture containing all objects:
	one_arm_texture = create_texture_from_image_file(renderer, "./img/one_armed_bandit.png", &one_arm_rect);
	coin_texture = create_texture_from_image_file(renderer, "./img/coin.png", &coin_rect);
	objects_texture = create_texture_from_image_file(renderer, "./img/objects.png", &object_rect);

	assert(one_arm_texture != NULL);
	assert(coin_texture != NULL);
	assert(objects_texture != NULL);

	object_height = object_rect.h / 9;

	// -------------------------
	// example of board display:
	// -------------------------
	SDL_RenderCopy(renderer, one_arm_texture, NULL, &one_arm_rect);

	SDL_Rect coin_rect_pos = {700, 1020, coin_rect.w, coin_rect.h};
	SDL_RenderCopy(renderer, coin_texture, NULL, &coin_rect_pos);

	for (i = 0; i < 2; i++)
	{
		SDL_Rect coin_rect_pos = {700, 400 - 10 * i, coin_rect.w, coin_rect.h};
		SDL_RenderCopy(renderer, coin_texture, NULL, &coin_rect_pos);
	}

	SDL_Rect src_rect, dst_rect = object_rect;
	dst_rect.h = object_height * 1.5; // display 1.5 object on screen for a wheel
	src_rect = dst_rect;
	src_rect.x = 0;

	// src_rect.y is positionned here on the 2nd object of objects.png
	src_rect.y = object_height;
	dst_rect.x = 85;
	dst_rect.y = 410 - object_height / 2; // setup the coord. of the icon in the global renderer
	SDL_RenderCopy(renderer, objects_texture, &src_rect, &dst_rect);

	SDL_RenderPresent(renderer);

	// -------------------------
	// managing events:
	// -------------------------
	SDL_Event event;
	bool quit = false;

	do
	{
		SDL_WaitEvent(&event); // passive waiting on an event
		switch (event.type)
		{
		case SDL_QUIT: // if SDL_Quit() called
			quit = true;
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_CLOSE) // if windows closed by user
			{
				quit = true;
			}
			break;
		case SDL_KEYDOWN: // if key pressed
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				quit = true;
				break;
			case SDLK_SPACE:
				wait_key_release();
				break;
			case SDLK_s:
				wait_key_release();
				break;
			}
		}
	} while (!quit);

	SDL_DestroyTexture(objects_texture);
	SDL_DestroyTexture(one_arm_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return NULL;
}
