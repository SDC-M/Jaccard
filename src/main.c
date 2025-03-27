#include <assert.h>
#include <string.h>

#include "avl/bst.h"
#include "holdall/holdall.h"
#include "jdis/jdis.h"

#define STR(s) #s
#define XSTR(s) STR(s)
#define WORD_MAX_SIZE 50

int rfree(void *ptr) {
  free(ptr);
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    return -1;
  }
  FILE *p1 = fopen(argv[1], "r");
  if (p1 == nullptr) {
    return 2.;
  }
  FILE *p2 = fopen(argv[2], "r");
  if (p2 == nullptr) {
    fclose(p1);
    return 2.;
  }
  char x[WORD_MAX_SIZE + 1];
  char y[WORD_MAX_SIZE + 1];
  bst *bst_f1 = bst_empty((int (*)(const void *, const void *)) strcmp);
  bst *bst_f2 = bst_empty((int (*)(const void *, const void *)) strcmp);
  if (bst_f1 == nullptr || bst_f2 == nullptr) {
    bst_dispose(&bst_f1);
    bst_dispose(&bst_f2);
    fclose(p1);
    fclose(p2);
    return EXIT_FAILURE;
  }
  holdall *words = holdall_empty();
  if (words == nullptr){
    bst_dispose(&bst_f1);
    bst_dispose(&bst_f2);
    fclose(p1);
    fclose(p2);
    return EXIT_FAILURE;
  }
  int r1 = fscanf(p1, "%" XSTR(WORD_MAX_SIZE) "s", x);
  int r2 = fscanf(p2, "%" XSTR(WORD_MAX_SIZE) "s", y);
  while (r1 != EOF || r2 != EOF) {
    char *z1 = malloc(strlen(x) + 1);
    char *z2 = malloc(strlen(y) + 1);
    strcpy(z1, x);
    strcpy(z2, y);
    holdall_put(words, z1);
    holdall_put(words, z2);
    if (r1 != EOF) {
      bst_add_endofpath(bst_f1, z1);
    }
    if (r2 != EOF) {
      bst_add_endofpath(bst_f2, z2);
    }
    if (r1 != EOF) {
      r1 = fscanf(p1, "%" XSTR(WORD_MAX_SIZE) "s", x);
    }
    if (r2 != EOF) {
      r2 = fscanf(p2, "%" XSTR(WORD_MAX_SIZE) "s", y);
    }
  }
  size_t card_in = card_intersection(bst_f1, bst_f2);
  printf("%lf\n", jdis(bst_f1, bst_f2, card_in));
  fclose(p1);
  fclose(p2);
  bst_dispose(&bst_f1);
  bst_dispose(&bst_f2);
  holdall_apply(words, rfree);
  holdall_dispose(&words);
  return EXIT_SUCCESS;
}
