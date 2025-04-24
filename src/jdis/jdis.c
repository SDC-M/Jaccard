#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <ctype.h>
#include "jdis.h"

#define WORD_INIT_SIZE 63
#define MULT_COEFF 2

#define OPT_STDIN "-"

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
//  vrai alors gère la ponctuation comme séparateur, en cas de succès renvoie 0
//  sinon EOF.
static int hm__fscanf(FILE *stream, size_t value_max, char **buffer, bool wp,
    bool wc) {
  int c;
  char *p = *buffer;
  size_t cptr = 0;
  while ((c = fgetc(stream)) != EOF) {
    if (wp && ispunct(c)) {
      break;
    }
    if (isspace(c)) {
      break;
    }
    if (cptr == value_max) {
      if (wc) {
        continue;
      } else {
        value_max *= MULT_COEFF;
        *buffer = realloc(*buffer, (size_t) (value_max + 1));
        if (*buffer == nullptr) {
          free(*buffer);
          return EOF;
        }
        p = *buffer + cptr;
      }
    }
    *p = (char) c;
    ++p;
    ++cptr;
  }
  if (cptr > 0) {
    *p = '\0';
  }
  if (cptr >= value_max || cptr >= WORD_INIT_SIZE){
    fprintf(stderr, "***Word \" %s...\" has been cut\n", *buffer);
  }
  if (feof(stream) != 0) {
    return EOF;
  }
  return c == EOF ? EOF : 0;
}

// create__file : En cas de succès renvoie une tête de lecture du fichier ouvert
//  en mode lecture seule de nom file_name, en cas d'échec renvoie nullptr.
static FILE *create__file(char *file_name) {
  if (file_name == nullptr) {
    return nullptr;
  }
  FILE *p = fopen(file_name, "r");
  if (p == nullptr) {
    return nullptr;
  }
  return p;
}

bst *file_to_bst(char *file_name, size_t value_max, bool wp, bst *uni_words) {
  if (uni_words == nullptr) {
    return nullptr;
  }
  FILE *p = nullptr;
  if (strcmp(file_name, OPT_STDIN) == 0) {
    rewind(stdin);
    p = stdin;
  } else {
    if ((p = create__file(file_name)) == nullptr) {
      return nullptr;
    }
  }
  bst *bst_f = bst_empty((int (*)(const void *, const void *)) strcoll);
  if (bst_f == nullptr) {
    fclose(p);
    return nullptr;
  }
  bool wc = false;
  if (value_max == 0) {
    value_max = WORD_INIT_SIZE;
  } else {
    wc = true;
  }
  char *x = malloc((size_t) (value_max + 1));
  if (x == nullptr) {
    fclose(p);
    bst_dispose(&bst_f);
    return nullptr;
  }
  int r = hm__fscanf(p, value_max, &x, wp, wc);
  while (r != EOF) {
    char *y = malloc(strlen(x) + 1);
    strcpy(y, x);
    char *retour_add_uni = bst_add_endofpath(uni_words, y);
    if (retour_add_uni == nullptr) {
      return nullptr;
    }
    char *res;
    if ((res = bst_add_endofpath(bst_f, retour_add_uni)) == nullptr) {
      free(x);
      fclose(p);
      bst_dispose(&bst_f);
      return nullptr;
    }
    if (y != retour_add_uni) {
      free(y);
    }
    r = hm__fscanf(p, value_max, &x, wp, wc);
  }
  free(x);
  if (p != stdin) {
    fclose(p);
  }
  return bst_f;
}

int add_element(bst *t, const void *ref) {
  return (bst_add_endofpath(t, ref) == nullptr) ? 1 : 0;
}
