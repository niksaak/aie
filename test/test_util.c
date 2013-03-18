#include <stdio.h>

#include "test_util.h"

int test_status;

int testc;
int failedc;
int passedc;

void test(char* name, test_function_t fun)
{
  printf("Running #%i %s: ", ++testc, name);
  (*fun)();
  test_status ? ++failedc : ++passedc;
}

void test_summary(void)
{
  printf("\nRan: %i. Failed: %i. Passed: %i\n", testc, failedc, passedc);
}

