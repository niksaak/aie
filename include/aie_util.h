#pragma once

#ifdef DEBUG_MODE
#define AIE_DEBUG(...) \
  promulgate(aie_PROMUL_DEBUG, __FILE__, __func__, __VA_ARGS__)
#endif

#define AIE_WARN(...) \
  promulgate(aie_PROMUL_WARNING, __FILE__, __func__, __VA_ARGS__)

#define AIE_ERROR(...) \
  promulgate(aie_PROMUL_ERROR, __FILE__, __func__, __VA_ARGS__)

#define AIE_PANIC(...) \
  promulgate(aie_PROMUL_CRITICAL, __FILE__, __func__, __VA_ARGS__)

#define AIE_PTRCALL(fun, ...) \
  fun == NULL ? NULL : fun(__VA_ARGS__)

#define AIE_FREE(ptr) \
  aie_free(&ptr)

typedef enum aie_PromulgationLevel {
  aie_PROMUL_DEBUG,
  aie_PROMUL_WARNING,
  aie_PROMUL_ERROR,
  aie_PROMUL_PANIC
} aie_PromulgationLevel;

extern void
promulgate(aie_PromulgationLevel level, const char* file, const char* func,
    const char* promulgation, ...);
    // printf promulgation to stderr, mentioning file and function 
    // with given level of scare

extern int ran_domo(int min, int max);
    // get pseudorandom number in range

extern void* aie_malloc(size_t size);

extern void aie_free(void** pointer);
