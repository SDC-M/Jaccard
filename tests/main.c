#include <assert.h>
#include <string.h>

#include "greatest/greatest.h"
#include "../src/avl/bst.h"

TEST all_dispose(void) {
  bst *p = bst_empty((int (*)(const void *, const void *)) strcmp);
  bst_add_endofpath(p, "nullptr");
  bst_dispose(&p);
  ASSERT_EQ(p, nullptr);
  PASS();
}

SUITE(the_suite) {
  RUN_TEST(all_dispose);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(the_suite);
  GREATEST_MAIN_END();
}
