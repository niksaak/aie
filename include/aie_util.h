/* aie_util.h - general utilities */

#pragma once

#include <stdlib.h>

int aie_ran_domo(int min, int max);
    // get pseudorandom number in range

void* aie_malloc(size_t size);
    // convenient wrapper for malloc()

void* aie_realloc(void* pointer, size_t size);
    // convenient wrapper for realloc()

inline void* aie_free(void* pointer)
{   // wrapper for free() returning NULL after freeing
  free(pointer);
  return NULL;
}

char* aie_mkstring(const char* format, ...);
    // returns pointer to static string with formatted string

inline long aie_roundtol(long num, long divisor)
{
  return num - num % divisor;
}

int aie_ensure_dir(const char* pathname);
    // ensure pathname exists

#if !(_SVID_SOURCE || _BSD_SOURCE || _XOPEN_SOURCE >= 500 || \
      _XOPEN_SOURCE && _XOPEN_SOURCE_EXTENDED || _POSIX_C_SOURCE >= 200809L)

#define AIE_OWN_STRDUP 1
char* strdup(const char* s);
    // define our own strdup() if we don't have one

#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

