#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jdis.h"

#define WORD_MAX_SIZE 50

#define PUNC_SEPARATORS "“`‘\'\"-_)(;,!?:.'\n "
#define CLASSIC_SEPARATORS "\n "

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

// hm__fscanf : tente de lire dans le fichier pointé par stream au plus
//  value_max caractères, stocke dans la zone pointée par buffer et si wp est à
//  vrai alors gère la ponctuation comme séparateur.
static int hm__fscanf(FILE *stream, int value_max, char *buffer, bool wp) {
  int c;
  char *p = buffer;
  int cptr = 0;
  const char *punc = wp == true ? PUNC_SEPARATORS : CLASSIC_SEPARATORS;
  while ((c = fgetc(stream)) != EOF) {
    if (strchr(punc, c) != nullptr || cptr >= value_max) {
      *p = '\0';
      return 0;
    }
    *p = (char) c;
    ++p;
    ++cptr;
  }
  *p = '\0';
  return EOF;
}

bst *file_to_bst(char *file_name, holdall *words, int value_max, bool wp) {
  if (file_name == nullptr || words == nullptr) {
    return nullptr;
  }
  FILE *p = fopen(file_name, "r");
  if (p == nullptr) {
    return nullptr;
  }
  if (value_max == 0) {
    value_max = WORD_MAX_SIZE;
  }
  char x[value_max + 1];
  bst *bst_f = bst_empty((int (*)(const void *, const void *)) strcoll);
  if (bst_f == nullptr) {
    fclose(p);
    return nullptr;
  }
  int r = hm__fscanf(p, value_max, x, wp);
  while (r != EOF) {
    char *z = malloc(strlen(x) + 1);
    strcpy(z, x);
    holdall_put(words, z);
    if (bst_add_endofpath(bst_f, z) == nullptr) {
      fclose(p);
      bst_dispose(&bst_f);
      return nullptr;
    }
    r = hm__fscanf(p, value_max, x, wp);
  }
  fclose(p);
  return bst_f;
}
