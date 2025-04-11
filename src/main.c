#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <locale.h>

#include "avl/bst.h"
#include "jdis/jdis.h"
#include "op/op.h"

#define LEN_LONG_OPT_INITIAL 10
#define LEN_SHORT_OPT_INITIAL 2

#define LONG_G "--graph"
#define LONG_H "--help"
#define LONG_I "--initial="
#define LONG_P "--punctuation-like-space"
#define ESCAPE_FILE "--"

#define SHORT_G "-g"
#define SHORT_H "-h"
#define SHORT_I "-i"
#define SHORT_P "-p"

int rfree(void *ctx, const void *ref) {
  if (ctx == nullptr) {
  }
  free((void *) ref);
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
    if (escaped_file) {
      filenames[nb_files] = argv[i];
      escaped_file = false;
      ++nb_files;
    } else if (strcmp(argv[i], SHORT_H) == 0 || strcmp(argv[i], LONG_H) == 0) {
      help();
    } else if (strcmp(argv[i], SHORT_P) == 0 || strcmp(argv[i], LONG_P) == 0) {
      want_punc = true;
    } else if (strcmp(argv[i], SHORT_G) == 0 || strcmp(argv[i], LONG_G) == 0) {
      graph = true;
    } else if (strstr(argv[i], LONG_I) != nullptr) {
      value_max = atoi(argv[i] + LEN_LONG_OPT_INITIAL);
    } else if (strstr(argv[i], SHORT_I) != nullptr) {
      value_max = atoi(argv[i] + LEN_SHORT_OPT_INITIAL);
    } else if (strcmp(argv[i], ESCAPE_FILE) == 0) {
      escaped_file = true;
    } else {
      filenames[nb_files] = argv[i];
      ++nb_files;
    }
  }
  if (nb_files < 2) {
    fprintf(stderr, "*** Pas assez d'arguments.\n");
    help();
    free(filenames);
    return EXIT_FAILURE;
  }
  bst **tab = malloc((size_t) (nb_files) * sizeof(bst *));
  for (int i = 0; i < nb_files; ++i) {
    tab[i] = file_to_bst(filenames[i], value_max, want_punc);
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
  } else {
    for (int i = 0; i < nb_files - 1; ++i) {
      for (int j = i + 1; j < nb_files; ++j) {
        size_t card_in = card_intersection(tab[i], tab[j]);
        double jaccard_distance = jdis(tab[i], tab[j], card_in);
        printf("%s\t%s: %.4lf\n", filenames[i], filenames[j], jaccard_distance);
      }
    }
  }
dispose:
  free(filenames);
  for (int i = 0; i < nb_files; ++i) {
    bst_dft_infix_apply_context(tab[i], 0, nullptr, rfree, nullptr,
        nullptr);
    bst_dispose(&tab[i]);
  }
  free(tab);
  return EXIT_SUCCESS;
}
