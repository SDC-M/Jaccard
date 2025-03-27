#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jdis.h"

struct ctx {
  bst *apply_set;
  size_t cptr;
};

int is_in(ctx *context, const void *ref) {
  if (bst_search(context->apply_set, ref) != nullptr) {
    context->cptr += 1;
  }
  return 0;
}

size_t card_intersection(bst *p, bst *q) {
  if (p == nullptr || q == nullptr) {
    return (size_t) -1;
  }
  ctx *context = malloc(sizeof(*context));
  bst *apply = p;
  context->apply_set = q;
  context->cptr = 0;
  if (bst_size(p) > bst_size(q)) {
    apply = q;
    context->apply_set = p;
  }
  bst_dft_infix_apply_context(apply, 0, context,
      (int (*)(void *context, const void *ref)) is_in, nullptr, nullptr);
  size_t res = context->cptr;
  free(context);
  return res;
}

double jdis(bst *p, bst *q, size_t card_interction) {
  return 1. - (double) card_interction
    / ((double) bst_size(p) + (double) bst_size(q) - (double) card_interction);
}
