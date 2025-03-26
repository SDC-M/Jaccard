#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jdis.h"

#define STR(s) #s
#define XSTR(s) STR(s)
#define WORD_MAX_SIZE 50

//static void put(const void *p) {
//printf("%s", (char *) p);
//}

double bst_jdis(char *f1, char *f2) {
  FILE *p1 = fopen(f1, "r");
  if (p1 == nullptr) {
    return 2.;
  }
  FILE *p2 = fopen(f2, "r");
  if (p2 == nullptr) {
    fclose(p1);
    return 2.;
  }
  char x[WORD_MAX_SIZE + 1];
  char y[WORD_MAX_SIZE + 1];
  bst *uni = bst_empty((int (*)(const void *, const void *)) strcmp);
  bst *inter = bst_empty((int (*)(const void *, const void *)) strcmp);
  if (uni == nullptr || inter == nullptr) {
    bst_dispose(&uni);
    bst_dispose(&inter);
    fclose(p1);
    fclose(p2);
    return 2.;
  }
  int r1 = fscanf(p1, "%" XSTR(WORD_MAX_SIZE) "s", x);
  int r2 = fscanf(p2, "%" XSTR(WORD_MAX_SIZE) "s", y);
  while (r1 != EOF || r2 != EOF) {
    char *z1 = malloc(strlen(x) + 1);
    char *z2 = malloc(strlen(y) + 1);
    strcpy(z1, x);
    strcpy(z2, y);
    if (r1 != EOF) {
      bst_add_endofpath(uni, z1);
      if (strcmp(x, y) == 0) {
        bst_add_endofpath(inter, z1);
      }
    }
    if (r2 != EOF) {
      bst_add_endofpath(uni, z2);
    }
    if (r1 != EOF) {
      r1 = fscanf(p1, "%" XSTR(WORD_MAX_SIZE) "s", x);
    }
    if (r2 != EOF) {
      r2 = fscanf(p2, "%" XSTR(WORD_MAX_SIZE) "s", y);
    }
  }
  //printf("union : \n");
  //bst_repr_graphic(uni, put);
  //printf("intersection : \n");
  //bst_repr_graphic(inter, put);
  printf("%f\n", (float) bst_size(uni));
  printf("%f\n", (float) bst_size(inter));
  fclose(p1);
  fclose(p2);
  double res = 1.0;
  res = 1 - (double) bst_size(inter) / (double) bst_size(uni);
  bst_dispose(&uni);
  bst_dispose(&inter);
  return res;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    return -1;
  }
  printf("%.4lf\n", bst_jdis(argv[1], argv[2]));
  return 0;
}
