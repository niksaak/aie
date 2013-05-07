#pragma once

#include <stdio.h>

#define DECLARE_TEST(name) \
  char* name(void)

#define DEFINE_TEST(name) \
  char* name(void) {

#define DEFINE_TEST_END \
  return NULL; }

#define ASSERT(assertion) \
  if(!(assertion)) return #assertion;

#define TEST(name) \
  test(name, #name)

extern int failure_count;
extern int success_count;
extern int test_count;

inline void test(char* (*fun)(void), const char* name)
{
  char* result = (*fun)();

  printf("running %s: ", name);
  test_count++;

  if(result == NULL) {
    printf("OK\n");
    success_count++;
  } else {
    printf("FAIL (%s)\n", result);
    failure_count++;
  }
}

