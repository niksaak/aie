#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <aie_util.h>

void promulgate(aie_PromulgationLevel level, const char* file, const char* func,
    const char* promulgation, ...)
{
  static char debug_str[] = "DEBUG";
  static char warning_str[] = "WARNING";
  static char error_str[] = "ERROR";
  static char panic_str[] = "PANIC";
  char* str;
  va_list va_args;

  switch(level)
  {
    case aie_PROMUL_DEBUG:
      str = debug_str;
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

  if(level == aie_PROMUL_PANIC)
    exit(EXIT_FAILURE);
}

int ran_domo(int min, int max)
{
  int domo = rand();
  int range = max - min;
  int rem = RAND_MAX % range;
  int bucket = RAND_MAX / range;

  if(domo == RAND_MAX)
    return ran_domo(min, max);

  if(domo < RAND_MAX - rem)
    return min + domo / bucket;
  return ran_domo(min, max);
}

void* aie_malloc(size_t size)
{
  void* ptr;

  errno = 0;
  ptr = malloc(size);

  if(ptr == NULL) {
    AIE_ERROR("Unable to malloc %z bytes, errno: %s", size, strerror(errno));
  }

  return ptr;
}

void aie_free(void** pointer)
{
  free(*pointer);
  *pointer = NULL;
}

