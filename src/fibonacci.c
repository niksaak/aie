#include <aie_fibonacci.h>

#define AIE_FIBONACCI_ARR_LEN 42

long aie_fibnums[AIE_FIBONACCI_ARR_LEN] =
{ // lookup table for aie_fib() function
  0,
  1,
  1,
  2,
  3,
  5,
  8,
  13,
  21,
  34,
  55,
  89,
  144,
  233,
  377,
  610,
  987,
  1597,
  2584,
  4181,
  6765,
  10946,
  17711,
  28657,
  46368,
  75025,
  121393,
  196418,
  317811,
  514229,
  832040,
  1346269,
  2178309,
  3524578,
  5702887,
  9227465,
  14930352,
  24157817,
  39088169,
  63245986,
  102334155,
  165580141
    // well, i think even if ai or some plugin-writer would want to allocate
    // more than 158 NYACKIN MEGABYTES in one chunk, it will be fast enough
};

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
  int i = 0;

  while(++i) {
    long f = aie_fib(i);

    if(f > n)
      return f;
  }

  return -1;
}

long aie_prevfib(long n)
{
  int i = 0;
  long prev = 0;

  while(++i) {
    long f = aie_fib(i);

    if(f > n)
      return prev;

    prev = f;
  }

  return -1;
}

