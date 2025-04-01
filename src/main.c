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

// add_element : Renvoie une valeur nulle si l'ajout en bout de chemin de ref
// dans l'arbre binaire de recherche associé à t se passe correctement. Renvoie
// une valeur différente de nulle sinon
int add_element(bst *t, const void *ref) {
  return (bst_add_endofpath(t, ref) != ref) ? 1 : 0;
}

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  int option;
  bool graph = false;
  int escaped_file = 0;
  int value_max = 0;
  bool want_punc = false;
  while ((option = getopt(argc, argv, "?pg-:i:")) != -1) {
    switch (option) {
      case '?':
        help();
        return EXIT_SUCCESS;
      case 'p':
        want_punc = true;
        break;
      case '-':
        escaped_file += 1;
        break;
      case 'i':
        value_max = atoi(optarg);
        break;
      case 'g':
        graph = true;
        break;
      default:
        return EXIT_FAILURE;
    }
  }
  if (argc - optind + escaped_file < 2) {
    fprintf(stderr, "Pas assez d'arguments.\n");
    return EXIT_FAILURE;
  }
  holdall *words = holdall_empty();
  bst **tab = malloc((size_t) (argc - optind + escaped_file) * sizeof(bst *));
  for (int i = optind - escaped_file; i < argc; ++i) {
    printf("%s\n", argv[i]);
    if (strcmp(argv[i], "-") == 0){
      printf("là\n");
    }
    tab[i - optind] = file_to_bst(argv[i], words, value_max, want_punc);
    if (tab[i - optind] == nullptr) {
      fprintf(stderr, "*** Erreur de l'allocation pour le fichier : %s\n",
          argv[i]);
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
      printf("%s\t%s: %.4lf\n", argv[i + optind], argv[j + optind],
          jaccard_distance);
    }
  }
  if (graph) {
    bst *uni = bst_empty((int (*)(const void *, const void *)) strcoll);
    if (uni == nullptr) {
      fprintf(stderr, "Erreur d'allocation sur l'union\n");
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
