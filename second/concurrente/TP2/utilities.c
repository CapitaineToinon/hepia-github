#include "utilities.h"

typedef long long LL;

static bool img_init = false;

void get_image_file_size(char *img_filename, int *im_width, int *im_height)
{
	SDL_Surface *img_surface;
	int img_flags = IMG_INIT_PNG;

	if (!img_init)
	{
		img_init = true;
		if (!(IMG_Init(img_flags) & img_flags))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			exit(1);
		}
	}

	assert((img_surface = IMG_Load(img_filename)) != NULL);

	*im_width = img_surface->w;
	*im_height = img_surface->h;
	SDL_FreeSurface(img_surface);
}

SDL_Texture *create_texture_from_image_file(SDL_Renderer *renderer, const char *img_filename, SDL_Rect *img_rect)
{
	SDL_Surface *img_surface;
	SDL_Texture *texture;
	int img_flags = IMG_INIT_PNG;

	if (!img_init)
	{
		img_init = true;
		if (!(IMG_Init(img_flags) & img_flags))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			exit(1);
		}
	}

	img_surface = IMG_Load(img_filename);
	SDL_GetClipRect(img_surface, img_rect);

	assert((texture = SDL_CreateTextureFromSurface(renderer, img_surface)) != NULL);

	SDL_FreeSurface(img_surface);
	return texture;
}

void wait_key_release()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			printf("key released\n");
			break;
		}
	}
}

/* return updated value: time_start + offset_ms. wait until current time reaches
 the time value returned. If delta time is negative, do not wait. */
struct timespec wait_until(struct timespec time_start, unsigned long offset_ms)
{
	struct timespec current_time, ret_time;
	LL end_ns, delay_ns;

	end_ns = (LL)time_start.tv_sec * (LL)1000000000 + (LL)time_start.tv_nsec + (LL)offset_ms * (LL)1000000;
	ret_time.tv_sec = end_ns / 1000000000;
	ret_time.tv_nsec = end_ns - ((LL)ret_time.tv_sec * (LL)1000000000);
	clock_gettime(CLOCK_MONOTONIC, &current_time);
	delay_ns = end_ns - ((LL)current_time.tv_sec * (LL)1000000000 + (LL)current_time.tv_nsec);

	if (delay_ns > 0)
	{
		//printf("wait: %ld us\n", (long)(delay_ns/1000));
		usleep((unsigned long)(delay_ns / 1000));
	}

	return ret_time;
}

long timespec_diff_ms(struct timespec time_start)
{
	struct timespec current_time;
	long start_ms;

	start_ms = (LL)time_start.tv_sec * (LL)1000 + (LL)time_start.tv_nsec / 1000000;
	clock_gettime(CLOCK_MONOTONIC, &current_time);
	return (long int)((current_time.tv_sec * 1000 + (long)(current_time.tv_nsec / 1000000LL)) - start_ms);
}
