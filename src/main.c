#include <assert.h>
#include <string.h>
#include <locale.h>

#include "avl/bst.h"
#include "holdall/holdall.h"
#include "jdis/jdis.h"
#include "op/op.h"

int rfree(void *ptr) {
  free(ptr);
  return 0;
}

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  if (argc < 3) {
    return EXIT_FAILURE;
  }
  holdall *words = holdall_empty();
  bst **tab = malloc((size_t)(argc - 1) * sizeof(bst *));
  for (int i = 1; i < argc; ++i) {
    tab[i - 1] = file_to_bst(argv[i], words);
    if (tab[i - 1] == nullptr) {
      fprintf(stderr, "*** Failed : %s\n", argv[i]);
      for (int j = 0; j < i - 1; ++j) {
        bst_dispose(&tab[j]);
      }
      free(tab);
      return EXIT_FAILURE;
    }
  }

  bst *uni = bst_empty((int (*)(const void *, const void *)) strcoll);

  //choldall *q = *words;

  //while (q != nullptr){
    //bst_add_endofpath(uni, q->ref);
    //q->next;
  //}

  graph_belonging(tab, uni);
  for (int i = 0; i < argc - 2; ++i) {
    for (int j = i + 1; j < argc - 1; ++j) {
      size_t card_in = card_intersection(tab[i], tab[j]);
      double jaccard_distance = jdis(tab[i], tab[j], card_in);
      printf("%s\t%s: %lf\n", argv[i + 1],
          argv[j + 1], jaccard_distance);
    }
  }

  for (int i = 0; i < argc - 1; ++i) {
    bst_dispose(&tab[i]);
  }
  holdall_apply(words, rfree);
  holdall_dispose(&words);
  free(tab);
  return EXIT_SUCCESS;
}
