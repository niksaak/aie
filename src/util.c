#include <aie_util.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include <aie_error.h>

int aie_ran_domo(int min, int max)
{
  int domo = rand();
  int range = max - min;
  int rem = RAND_MAX % range;
  int bucket = RAND_MAX / range;

  if(domo == RAND_MAX)
    return aie_ran_domo(min, max);

  if(domo < RAND_MAX - rem)
    return min + domo / bucket;
  return aie_ran_domo(min, max);
}

void* aie_malloc(size_t size)
{
  void* ptr;

  errno = 0;
  ptr = malloc(size);

  if(ptr == NULL)
    AIE_PANIC(aie_EERRNO, NULL);

  return ptr;
}

void* aie_realloc(void* ptr, size_t size)
{
  errno = 0;
  ptr = realloc(ptr, size);

  if(ptr == NULL)
    AIE_PANIC(aie_EERRNO, NULL);

  return ptr;
}

extern inline void* aie_free(void* ptr);

char* aie_mkstring(const char* format, ...)
{
  va_list va_args;
  static char string[512];

  va_start(va_args, format);
  vsnprintf(string, 512, format, va_args);
  va_end(va_args);

  return string;
}

#ifdef AIE_OWN_STRDUP

char* strdup(const char* s)
{
  char* string = aie_malloc(strlen(s) + 1);
  AIE_ERETURN(NULL);
  return strcpy(string, s);
}

#endif
