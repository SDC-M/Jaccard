#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <locale.h>
#include <getopt.h>

#include "avl/bst.h"
#include "holdall/holdall.h"
#include "jdis/jdis.h"
#include "op/op.h"

#define LEN_LONG_OPT_INITIAL 10
#define LEN_SHORT_OPT_INITIAL 2

int rfree(void *ptr) {
  free(ptr);
  return 0;
}

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  bool graph = false;
  bool escaped_file = false;
  int value_max = 0;
  bool want_punc = false;
  int nb_files = 0;
  char **filenames = malloc((size_t) argc * sizeof(char *));
  if (filenames == nullptr) {
    printf("*** Erreur d'allocation du tableau des noms de fichiers\n");
    return EXIT_FAILURE;
  }
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0) {
      help();
    } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i],
          "--punctuation-like-space") == 0) {
      want_punc = true;
    } else if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--graph") == 0) {
      graph = true;
    } else if (strstr(argv[i], "--initial=") != nullptr) {
      value_max = atoi(argv[i] + LEN_LONG_OPT_INITIAL);
    } else if (strstr(argv[i], "-i") != nullptr) {
      value_max = atoi(argv[i] + LEN_SHORT_OPT_INITIAL);
    } else if (strcmp(argv[i], "--") == 0) {
      escaped_file = true;
    } else if (escaped_file) {
      filenames[nb_files] = argv[i];
      escaped_file = false;
      ++nb_files;
    } else {
      filenames[nb_files] = argv[i];
      ++nb_files;
    }
  }
  holdall *words = holdall_empty();
  bst **tab = malloc((size_t) (nb_files) * sizeof(bst *));
  for (int i = 0; i < nb_files; ++i) {
    tab[i] = file_to_bst(filenames[i], words, value_max, want_punc);
    if (tab[i] == nullptr) {
      fprintf(stderr, "*** Erreur de l'allocation pour le fichier : %s\n",
          filenames[i]);
      for (int j = 0; j < i; ++j) {
        bst_dispose(&tab[j]);
      }
      free(tab);
      return EXIT_FAILURE;
    }
  }
  for (int i = 0; i < nb_files - 1; ++i) {
    for (int j = i + 1; j < nb_files; ++j) {
      size_t card_in = card_intersection(tab[i], tab[j]);
      double jaccard_distance = jdis(tab[i], tab[j], card_in);
      printf("%s\t%s: %.4lf\n", filenames[i], filenames[j], jaccard_distance);
    }
  }
  if (graph) {
    bst *uni = bst_empty((int (*)(const void *, const void *)) strcoll);
    if (uni == nullptr) {
      fprintf(stderr, "*** Erreur d'allocation sur l'union\n");
      goto dispose;
    }
    for (int i = 0; i < nb_files; ++i) {
      bst_dft_infix_apply_context(tab[i], 0, uni,
          (int (*)(void *, const void *)) add_element, nullptr, nullptr);
    }
    for (int i = 0; i < nb_files; ++i) {
      printf("\t%s", filenames[i]);
    }
    printf("\n");
    graph_belonging(tab, uni, nb_files);
    bst_dispose(&uni);
  }
dispose:
  for (int i = 0; i < nb_files; ++i) {
    bst_dispose(&tab[i]);
  }
  holdall_apply(words, rfree);
  holdall_dispose(&words);
  free(tab);
  free(filenames);
  return EXIT_SUCCESS;
}
