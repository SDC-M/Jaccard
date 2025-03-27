#include <assert.h>
#include <string.h>
#include <math.h>

#include "greatest/greatest.h"
#include "../src/avl/bst.h"
#include "../src/holdall/holdall.h"
#include "../src/jdis/jdis.h"

TEST all_dispose(void) {
  bst *p = bst_empty((int (*)(const void *, const void *)) strcmp);
  ASSERT_NEQ(p, nullptr);
  bst_add_endofpath(p, "nullptr");
  bst_dispose(&p);
  ASSERT_EQ(p, nullptr);
  PASS();
}

TEST is_inside(void){
  bst *p = bst_empty((int (*)(const void *, const void *)) strcmp);
  bst_add_endofpath(p, "nullptr");
  ASSERT_NEQ(p, nullptr);
  ASSERT_EQ(bst_search(p, "a"), nullptr);
  ASSERT_NEQ(bst_search(p, "nullptr"), nullptr);
  bst_dispose(&p);
  PASS();
}

TEST jaccard_distance(void){
  holdall *words = holdall_empty();
  bst *a = file_to_bst((char *)"../tests/x0.txt", words);
  ASSERT_NEQ(a, nullptr);
  bst *b = file_to_bst((char *)"../tests/x1.txt", words);
  ASSERT_NEQ(b, nullptr);
  bst *c = file_to_bst((char *)"../tests/x2.txt", words);
  ASSERT_NEQ(c, nullptr);
  ASSERT_EQ(jdis(a, b, card_intersection(a, b)), 0.800);
  ASSERT_EQ(jdis(b, c, card_intersection(b, c)), 0.9375);
  bst_dispose(&a);
  bst_dispose(&b);
  bst_dispose(&c);
  PASS();
}

SUITE(the_suite) {
  RUN_TEST(all_dispose);
  RUN_TEST(is_inside);
  RUN_TEST(jaccard_distance);
}

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(the_suite);
  GREATEST_MAIN_END();
}
