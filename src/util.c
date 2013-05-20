#include <aie_util.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <aie_error.h>

int aie_ran_domo(int min, int max)
{
  int domo = rand();
  int range = max - min;
  int rem = RAND_MAX % range;
  int bucket = RAND_MAX / range;

  if(domo == RAND_MAX) {
    return aie_ran_domo(min, max);
  }
  if(domo < RAND_MAX - rem) {
    return min + domo / bucket;
  }
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

extern inline long aie_roundtol(long num, long divisor);

static int mkdir_ensured(const char* path, mode_t mode)
{
  if(mkdir(path, mode)) {
    if(errno != EEXIST) {
      AIE_ERROR(aie_EERRNO, path);
      return -1;
    }
  }
  return 0;
}

int aie_ensure_dir(const char* pathname)
{
  if(pathname == NULL) {
    AIE_ERROR(aie_ENURUPO, "pathname");
    return -1;
  }

  size_t len = strlen(pathname);
  char buf[len];

  strcpy(buf, pathname);
  if(buf[len - 1] == '/') { // trailing slashes
    buf[len - 1] = 0;       // are not required
  }
  for(char* p = buf + 1; *p; p++) {
    struct stat stt;

    if(*p == '/') {
      *p = 0;
      if(stat(pathname, &stt)) {  // if stat() failed
        if(errno != ENOENT) { // and not because 'not found'
          AIE_ERROR(aie_EERRNO, pathname);
          return -1;
        }
      }
      if(mkdir(pathname, S_IRWXU | S_IRGRP | S_IROTH)) { // if mkdir() failed
        if(errno != EEXIST) { // and not because 'already exists'
          AIE_ERROR(aie_EERRNO, pathname);
          return -1;
        }
      }
      if(mkdir_ensured(pathname, S_IRWXU | S_IRGRP | S_IROTH)) {
        return -1;
      }
      *p = '/';
    }
  }
  if(mkdir_ensured(pathname, S_IRWXU | S_IRGRP | S_IROTH)) {
    return -1;
  }
  return 0;
}

#ifdef AIE_OWN_STRDUP

char* strdup(const char* s)
{
  char* string = aie_malloc(strlen(s) + 1);
  AIE_ERETURN(NULL);
  return strcpy(string, s);
}

#endif
