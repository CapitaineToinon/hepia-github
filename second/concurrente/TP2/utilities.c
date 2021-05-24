#include "utilities.h"

typedef long long LL;

int msleep(unsigned int tms)
{
	return usleep(tms * 1000);
}

void wait_keyup()
{
	SDL_Event event;
	while (1)
	{
		if (SDL_PollEvent(&event))
			if (event.type == SDL_KEYUP)
				break;
	}
}

bool are_values_equal(const int a[], int n)
{
	while (--n > 0 && a[n] == a[0])
	{
		// nothing
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
