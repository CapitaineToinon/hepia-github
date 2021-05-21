#include "utilities.h"

typedef long long LL;

int msleep(unsigned int tms)
{
	return usleep(tms * 1000);
}

void wait_key_release()
{
	SDL_Event event;
	while (1)
	{
		if (SDL_PollEvent(&event))
			if (event.type == SDL_KEYUP)
				break;
	}
}

long ms_since(struct timespec time_start)
{
	struct timespec current_time;
	long start_ms;

	start_ms = (LL)time_start.tv_sec * (LL)1000 + (LL)time_start.tv_nsec / 1000000;
	clock_gettime(CLOCK_MONOTONIC, &current_time);
	return (long int)((current_time.tv_sec * 1000 + (long)(current_time.tv_nsec / 1000000LL)) - start_ms);
}

bool are_values_equal(const int a[], int n)
{
	while (--n > 0 && a[n] == a[0])
	{
	}

	return n == 0;
}

// from https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
int cmp_int(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

// from https://www.geeksforgeeks.org/count-number-of-occurrences-or-frequency-in-a-sorted-array/
int count_occurrences(int arr[], int n, int x)
{
	int res = 0;
	for (int i = 0; i < n; i++)
		if (x == arr[i])
			res++;
	return res;
}

// long now()
// {
// 	struct timespec current_time;
// 	clock_gettime(CLOCK_MONOTONIC, &current_time);
// }
