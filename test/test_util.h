#pragma once

#define TEST(fun) test(#fun, &fun)

#define ASSERT(assertion) \
  do { \
    if(assertion) { \
      printf("OK\n"); \
      test_status = 0; \
    } else { \
      printf("FAILED (%s)\n", #assertion); \
      test_status = 1; \
    } \
  } while(0)

extern int testc;
extern int failedc;
extern int passedc;
extern int test_status;

typedef void (*test_function_t)(void);

void test(char* name, test_function_t fun);

void test_summary(void);
