#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jdis.h"

#define BUFFER_SIZE 1023

float bst_jdis(char *f1, char *f2) {
  FILE *p1 = fopen(f1, "r");
  if (p1 == nullptr) {
    return 2.;
  }
  FILE *p2 = fopen(f2, "r");
  if (p2 == nullptr) {
    fclose(p1);
    return 2.;
  }
  char x[BUFFER_SIZE + 1];
  char y[BUFFER_SIZE + 1];
  bst *uni = bst_empty((int (*)(const void *, const void *)) strcmp);
  bst *inter = bst_empty((int (*)(const void *, const void *)) strcmp);
  if (uni == nullptr || inter == nullptr) {
    bst_dispose(&uni);
    bst_dispose(&inter);
    fclose(p1);
    fclose(p2);
    return 2.;
  }
  int r1 = fscanf(p1, "%s", x);
  int r2 = fscanf(p2, "%s", y);
  while (r1 != EOF || r2 != EOF) {
    if (r1 != EOF) {
      if (bst_add_endofpath(uni, x) != x) {
        bst_add_endofpath(inter, x);
        r1 = fscanf(p1, "%s", x);
      }
    }
    if (r2 != EOF) {
      if (bst_add_endofpath(uni, y) != y) {
        bst_add_endofpath(inter, y);
        r2 = fscanf(p2, "%s", y);
      }
    }
  }
  return 1 - (float)bst_size(inter) / (float)bst_size(uni);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    return -1;
  }
  printf("%f", bst_jdis(argv[1], argv[2]));
  return 0;
}
