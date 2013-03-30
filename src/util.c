#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include <aie_util.h>

static aie_PromulgationCallback promul_cb = NULL;

void aie_promulgate(aie_PromulgationLevel level,
    const char* file, const char* func, const char* promulgation, ...)
{
  static char debug_str[] =   "  DEBUG";
  static char message_str[] = "       ";
  static char warning_str[] = "WARNING";
  static char error_str[] =   "  ERROR";
  static char panic_str[] =   "  PANIC";
  char* str = NULL;
  va_list va_args;

  switch(level)
  {
    case aie_PROMUL_DEBUG:
      str = debug_str;
      break;
    case aie_PROMUL_MESSAGE:
      str = message_str;
      break;
    case aie_PROMUL_WARNING:
      str = warning_str;
      break;
    case aie_PROMUL_ERROR:
      str = error_str;
      break;
    case aie_PROMUL_PANIC:
      str = panic_str;
      break;
  }

  fputs(str, stderr);
  fprintf(stderr, " @ %s in %s():\n    ", file, func);

  va_start(va_args, promulgation);
  vfprintf(stderr, promulgation, va_args);
  va_end(va_args);

  fputc('\n', stderr);

  AIE_FUNCALL(promul_cb, level, file, func, promulgation);

  if(level == aie_PROMUL_PANIC)
    exit(EXIT_FAILURE);
}

void aie_set_promulgation_callback(aie_PromulgationCallback fun)
{
  if(promul_cb != NULL) {
    AIE_WARN("Resetting promulgation callback.");
  }
  promul_cb = fun;
}

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

  if(ptr == NULL || errno) {
    AIE_PANIC("Unable to malloc %z bytes, errno: %s", size, strerror(errno));
  }

  return ptr;
}

void* aie_realloc(void* pointer, size_t size)
{
  errno = 0;
  pointer = realloc(pointer, size);

  if(pointer == NULL || errno) {
    AIE_PANIC("Unable to reallocate %z bytes, errno: %s",
        size, strerror(errno));
  }

  return pointer;
}

static bool findin(int c, char* chars)
{ // there was a great function, but now there's just a wrapper T___T
  return strchr(chars, c) != NULL;
}

int aie_strtoks(const char* string, char* delims,
                const char* dest[], size_t count)
{
  unsigned i = 0;
  unsigned j = 0;
  size_t len = strlen(string);

  dest[j++] = string;

  for(; i < len && j <= count; i++) {
    if(findin(string[i], delims) && !findin(string[i + 1], delims)) {
      dest[j++] = &string[i + 1];
    }
  }

  return j;
}

char* aie_tokcpy(char* dest, const char* src, int delim, size_t size)
{
  size_t len = strlen(src);
  unsigned i = 0;

  for(; i < len && i < size && src[i] != delim; i++)
    dest[i] = src[i];
  dest[i] = '\0';

  return dest;
}

