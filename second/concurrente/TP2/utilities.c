#include "utilities.h"

typedef long long LL;

void wait_key_release()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			return;
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
