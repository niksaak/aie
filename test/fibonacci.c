#include "fibonacci.h"

#include <aie_fibonacci.h>

DEFINE_TEST(nextfib_1_ok)
{
  long fib = aie_nextfib(1);

  ASSERT(fib > 1);
}
DEFINE_TEST_END

