#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include <aie_util.h>

#include "fibonacci.h"

void promulgate(aie_PromulgationLevel level, const char* file, const char* func,
    const char* promulgation, ...)
{
  static char debug_str[] = "DEBUG";
  static char warning_str[] = "WARNING";
  static char error_str[] = "ERROR";
  static char panic_str[] = "PANIC";
  char* str = NULL;
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

void aie_free(void* pointer)
{
  free(pointer);
}

long aie_fib(int n)
{ // TODO: possibly make this inlined
  if(n > 0) {
    if(n < 39) {
      return aie_fibnums[n];
    } else 
      return aie_fib(n - 1) + aie_fib(n - 2);
  }

  return 0;
}

long aie_nextfib(long n)
{
  unsigned i = 0;

  while(++i) {
    long f = aie_fib(i);

    if(f > n)
      return f;
  }
  
  return -1;
}

long aie_prevfib(long n)
{
  unsigned i = 0;
  long prev = 0;

  while(++i) {
    long f = aie_fib(i);

    if(f > n)
      return prev;

    prev = f;
  }

  return -1;
}

