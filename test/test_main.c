#include <stdio.h>

#include "test.h"
#include "fibonacci.h"

DEFINE_TEST(general_phailore)
{
  ASSERT(0 == 9);
}
DEFINE_TEST_END

static void print_test_stats(void)
{
  printf("\nSummary:\n"
         "  tests ran: %-i\n"
         "  passed:    %-i\n"
         "  failed:    %-i\n",
         test_count, success_count, failure_count);
}

int main(int argc, char** argv)
{
  TEST(nextfib_1_ok);
  TEST(general_phailore);

  print_test_stats();

  return 0;
}

