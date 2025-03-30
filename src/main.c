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

int rfree(void *ptr) {
  free(ptr);
  return 0;
}

//void put(const char *ref) {
  //printf("%s", ref);
//}

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  bool graph = false;
  int option;
  while ((option = getopt(argc, argv, "?pg-:i:")) != -1) {
    switch (option) {
      case '?':
        help();
        return EXIT_SUCCESS;
      case 'p':
        printf("punctuaction like space : on\n");
        break;
      case '-':
        printf("fichier : %s\n", optarg);
        break;
      case 'i':
        printf("long max mot : %d\n", atoi(optarg));
        break;
      case 'g':
        printf("affichage graphique : on\n");
        graph = true;
        break;
      default:
        return EXIT_FAILURE;
    }
  }
  if (argc - optind < 2) {
    fprintf(stderr,
        "Pas assez d'arguments.\n");
    return EXIT_FAILURE;
  }
  holdall *words = holdall_empty();
  bst **tab = malloc((size_t) (argc - optind) * sizeof(bst *));
  for (int i = optind; i < argc; ++i) {
    tab[i - optind] = file_to_bst(argv[i], words);
    if (tab[i - optind] == nullptr) {
      fprintf(stderr, "*** Échec : %s\n", argv[i]);
      for (int j = optind; j < i; ++j) {
        bst_dispose(&tab[j - optind]);
      }
      free(tab);
      return EXIT_FAILURE;
    }
  }
  for (int i = 0; i < argc - optind - 1; ++i) {
    for (int j = i + 1; j < argc - optind; ++j) {
      size_t card_in = card_intersection(tab[i], tab[j]);
      double jaccard_distance = jdis(tab[i], tab[j], card_in);
      printf("%s\t%s: %lf\n", argv[i + optind], argv[j + optind],
          jaccard_distance);
    }
  }
  if (graph) {
    bst *uni = bst_empty((int (*)(const void *, const void *)) strcoll);
    if (uni == nullptr) {
      fprintf(stderr, "Erreur d'allocation sur l'Union");
      goto dispose;
    }
    for (int i = 0; i < argc - optind; ++i) {
      bst_dft_infix_apply_context(tab[i], 0, uni,
          (int (*)(void *, const void *)) add_element, nullptr, nullptr);
    }
    for (int i = 0; i < argc - optind; ++i) {
      printf("\t%s", argv[i + optind]);
    }
    printf("\n");
    //bst_repr_graphic(uni, (void (*)(const void *)) put);
    graph_belonging(tab, uni, argc - optind);
    bst_dispose(&uni);
  }
dispose:
  for (int i = 0; i < argc - optind; ++i) {
    bst_dispose(&tab[i]);
  }
  holdall_apply(words, rfree);
  holdall_dispose(&words);
  free(tab);
  return EXIT_SUCCESS;
}
