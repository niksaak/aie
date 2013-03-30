#pragma once

#include <stdlib.h>

#ifdef DEBUG_MODE
#define AIE_DEBUG(...) \
  aie_promulgate(aie_PROMUL_DEBUG, __FILE__, __func__, __VA_ARGS__)
#else
#define AIE_DEBUG(...)
#endif

#define AIE_MSG(...) \
  aie_promulgate(aie_PROMUL_MESSAGE, __FILE__, __func__, __VA_ARGS__)

#define AIE_WARN(...) \
  aie_promulgate(aie_PROMUL_WARNING, __FILE__, __func__, __VA_ARGS__)

#define AIE_ERROR(...) \
  aie_promulgate(aie_PROMUL_ERROR, __FILE__, __func__, __VA_ARGS__)

#define AIE_PANIC(...) \
  aie_promulgate(aie_PROMUL_PANIC, __FILE__, __func__, __VA_ARGS__)

#define AIE_FUNCALL(fun, ...) \
  (fun == NULL) ? (void)0 : (*fun)(__VA_ARGS__)

typedef enum aie_PromulgationLevel {
  aie_PROMUL_DEBUG,
  aie_PROMUL_MESSAGE,
  aie_PROMUL_WARNING,
  aie_PROMUL_ERROR,
  aie_PROMUL_PANIC
} aie_PromulgationLevel;

typedef void
(*aie_PromulgationCallback)(aie_PromulgationLevel level,
    const char* file, const char* func, const char* promulgation);
    // promulgation callback

void aie_promulgate(aie_PromulgationLevel level, const char* file,
    const char* func, const char* promulgation, ...);
    // printf promulgation to stderr, mentioning file and function
    // with given level of scare

void aie_set_promulgation_callback(aie_PromulgationCallback fun);
    // set function to be called on each promulgation

int aie_ran_domo(int min, int max);
    // get pseudorandom number in range

void* aie_malloc(size_t size);
    // convenient wrapper around malloc()

void* aie_realloc(void* pointer, size_t size);
    // convenient wrapper around realloc()

int aie_strtoks(const char* string, char* delims,
                const char* dest[], size_t count);
    // put pointers to substrings of 'string' delimited by 'delim' to 'dest',
    // 'count' pointers at most. return count of pointers

char* aie_tokcpy(char* dest, const char* src, int delim, size_t size);
    // copy string 'src' until first occurance of 'delim' to dest,
    // 'size' bytes at most. returns 'dest'

