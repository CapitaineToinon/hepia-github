// from windef.h
#ifndef NOMINMAX

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif  /* NOMINMAX */

#ifndef __HELPER_H_SEEN__
#define __HELPER_H_SEEN__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int stringtoi(const char *);

#endif