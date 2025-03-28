#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <locale.h>
#include <getopt.h>

#include "avl/bst.h"
#include "holdall/holdall.h"
#include "jdis/jdis.h"

void help(void) {
  fprintf(stdout,
      "Liste des opérations réalisables :\n\n");
  fprintf(stdout, " -? : Affiche ce menu d'aide.\n");
  fprintf(stdout,
      " -g : Affiche le graphe d'appartenance des mots."
      "dans les fichiers passés sur la ligne de commande.\n");
  fprintf(stdout, " -i VALUE : permet de fixer la longueur maximale."
      "des mots à VALUE\n");
  fprintf(stdout, " -p : Considère les caractères de ponctuation."
      "comme des caractères d'espacement.\n");
  fprintf(stdout,
      " -- : Indique que l'argument qui suit doit être "
      "considéré comme un fichier.\n");
}

int rfree(void *ptr) {
  free(ptr);
  return 0;
}

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  int option;
  while ((option = getopt(argc, argv, "?p-:i:")) != -1) {
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
  for (int i = 0; i < argc - optind; ++i) {
    bst_dispose(&tab[i]);
  }
  holdall_apply(words, rfree);
  holdall_dispose(&words);
  free(tab);
  return EXIT_SUCCESS;
}
