#include "test.h"

int failure_count;
int success_count;
int test_count;

extern inline void test(char* (*fun)(void), const char* name);

