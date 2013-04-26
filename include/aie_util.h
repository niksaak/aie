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

int aie_strtoks(const char* string, char* delims,
                const char* dest[], size_t count);
    // put pointers to substrings of 'string' delimited by 'delim' to 'dest',
    // 'count' pointers at most. return count of pointers

char* aie_tokcpy(char* dest, const char* src, int delim, size_t size);
    // copy string 'src' until first occurance of 'delim' to dest,
    // 'size' bytes at most. returns 'dest'

