#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jdis.h"
#define STR(s) #s
#define XSTR(s) STR(s)
#define WORD_MAX_SIZE 50

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

bst *file_to_bst(char *file_name, holdall *words) {
  if (file_name == nullptr || words == nullptr){
    return nullptr;
  }
  FILE *p = fopen(file_name, "r");
  if (p == nullptr) {
    return nullptr;
  }
  char x[WORD_MAX_SIZE + 1];
  bst *bst_f = bst_empty((int (*)(const void *, const void *)) strcoll);
  if (bst_f == nullptr) {
    fclose(p);
    return nullptr;
  }
  int r = fscanf(p, "%" XSTR(WORD_MAX_SIZE) "s", x);
  while (r != EOF) {
    char *z = malloc(strlen(x) + 1);
    strcpy(z, x);
    holdall_put(words, z);
    if (bst_add_endofpath(bst_f, z) == nullptr) {
      fclose(p);
      bst_dispose(&bst_f);
      return nullptr;
    }
    r = fscanf(p, "%" XSTR(WORD_MAX_SIZE) "s", x);
  }
  fclose(p);
  return bst_f;
}
