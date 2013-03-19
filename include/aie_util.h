#pragma once

#include <stdlib.h>

#ifdef DEBUG_MODE
#define AIE_DEBUG(...) \
  promulgate(aie_PROMUL_DEBUG, __FILE__, __func__, __VA_ARGS__)
#else
#define AIE_DEBUG(...)
#endif

#define AIE_WARN(...) \
  promulgate(aie_PROMUL_WARNING, __FILE__, __func__, __VA_ARGS__)

#define AIE_ERROR(...) \
  promulgate(aie_PROMUL_ERROR, __FILE__, __func__, __VA_ARGS__)

#define AIE_PANIC(...) \
  promulgate(aie_PROMUL_PANIC, __FILE__, __func__, __VA_ARGS__)

#define AIE_FUNCALL(fun, ...) \
  fun == NULL ? NULL : fun(__VA_ARGS__)

typedef enum aie_PromulgationLevel {
  aie_PROMUL_DEBUG,
  aie_PROMUL_WARNING,
  aie_PROMUL_ERROR,
  aie_PROMUL_PANIC
} aie_PromulgationLevel;

void promulgate(aie_PromulgationLevel level, const char* file,
    const char* func, const char* promulgation, ...);
    // printf promulgation to stderr, mentioning file and function
    // with given level of scare

int ran_domo(int min, int max);
    // get pseudorandom number in range

void* aie_malloc(size_t size);
    // convenient wrapper around malloc()

void* aie_realloc(void* pointer, size_t size);
    // convenient wrapper around realloc()

int aie_strtoks(char* string, int delim, char** dest, size_t count);
    // put pointers to substrings of 'string' delimited by 'delim' to 'dest',
    // 'count' pointers at most. return count of pointers

char* aie_tokcpy(char* dest, const char* src, int delim, size_t size);
    // copy string 'src' until first occurance of 'delim' to dest,
    // 'size' bytes at most. returns dest

