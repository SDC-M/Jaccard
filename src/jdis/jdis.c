#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <ctype.h>
#include "jdis.h"

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

// hm__fscanf : tente de lire dans le fichier pointé par stream au plus
//  value_max caractères, stocke dans la zone pointée par buffer et si wp est à
//  vrai alors gère la ponctuation comme séparateur, en cas de succès renvoie 0
//  sinon EOF.
static int hm__fscanf(FILE *stream, int value_max, char *buffer, bool wp) {
  int c;
  char *p = buffer;
  int cptr = 0;
  while ((c = fgetc(stream)) != EOF) {
    if (wp && ispunct(c)) {
      break;
    }
    if (isspace(c) || cptr >= value_max) {
      break;
    }
    *p = (char) c;
    ++p;
    ++cptr;
  }
  if (cptr > 0){
    *p = '\0';
  }
  return c == EOF ? EOF : 0;
}


// stdin__to_file : En cas de succès renvoie une tête de lecture vers un fichier
//  temporaire en mode lecture seule, en cas d'échec renvoie nullptr.
static FILE* stdin__to_file () {
  FILE *p = tmpfile();
  if (p == nullptr) {
    fprintf(stderr, "*** Erreur lors de l'allocation pour l'entrée standart.");
    return nullptr;
  }
  int c;
  rewind(stdin);
  while ((c = fgetc(stdin)) != EOF) {
    fputc(c, p);
  }
  rewind(p);
  return p;
}

// create__file : En cas de succès renvoie une tête de lecture du fichier ouvert
//  en mode lecture seule de nom file_name, en cas d'échec renvoie nullptr.
static FILE* create__file (char *file_name) {
  if (file_name == nullptr) {
      return nullptr;
  }
  FILE *p = fopen(file_name, "r");
  if (p == nullptr) {
      return nullptr;
  }
  return p;
}

bst *file_to_bst(char *file_name, holdall *words, int value_max, bool wp) {
  if (words == nullptr) {
    return nullptr;
  }
  FILE *p = nullptr;
  if (strcmp(file_name, "-") == 0) {
    if ((p = stdin__to_file()) == nullptr){
      return nullptr;
    }
   } else {
    if ((p = create__file(file_name)) == nullptr){
      return nullptr;
    }
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
    char *res;
    if ((res = bst_add_endofpath(bst_f, z)) == nullptr) {
      fclose(p);
      bst_dispose(&bst_f);
      return nullptr;
    }
    if (res == z) {
      holdall_put(words, z);
    } else {
      free(z);
    }
    r = hm__fscanf(p, value_max, x, wp);
  }
  fclose(p);
  return bst_f;
}

int add_element(bst *t, const void *ref) {
  return (bst_add_endofpath(t, ref) != ref) ? 1 : 0;
}
