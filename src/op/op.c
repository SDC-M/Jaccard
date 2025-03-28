#include <stdlib.h>
#include <stdio.h>

int help() {
  fprintf(stdout,
      "Here's a list of the operation on the module Jaccard dissimilarity :\n");
  fprintf(stdout, "\t -? : Affiche ce menu d'aide\n");
  fprintf(stdout,
      "\t -g : Affiche le graphe d'appartenance des mots "
      "dans les fichiers passés sur la ligne de commande\n");
  fprintf(stdout, "\t -i VALUE : permet de fixer la longueur maximale "
      "des mots à VALUE\n");
  fprintf(stdout, "\t -p : Considère les caractères de ponctuation "
      "comme des caractères d'espacement");
  fprintf(stdout,
      "\t -- : Indique que l'argument qui suit doit être "
      "considéré comme un fichier\n");
  return 0;
}

int graph_belonging(bst **t, int (*compar)(const void *, const void *)) {
  bst *union = bst_empty(compar);
  bst *q = t;
  // union de tous les arbres du tableau de bst pointé de *t
  while (q != nullptr) {
    bst_dft_infix_apply_context(q, 0, union, bst_add_endofpath, bst_search,
        bst_search);
    q += 1;
  }
  // affichage graphique
  return bst_dft_infix_apply_context(union, 0, t, scptr_display, nullptr,
        nullptr);
}

static int scptr_display(bst **context, const char *ref, ) {
  bst *q = *context;
  printf("%s\t", *ref);
  while (q != nullptr) {
    if (bst_search(q, ref) != nullptr) {
      printf("x\t");
    } else {
      printf("-\t");
    }
    ++q;
  }
  printf("\n");
}

int set_max_value(int value, int limit) {
  if (value == 0) {
    value = limit;
  }
  return 0;
}

// MANQUE UN PARAMÈTRE OU ON STOCKE LE RÉSULTAT DE LA FONCTION
// MANQUE GESTION D'ERREUR
int punctuation_like_space(char *buffer) {
  char *m1;
  char *p = buffer;
  while (p != nullptr) {
    if (*p == '.' && *p != ';' && *p != ',' && *p != '-') {
      *m1 = *p;
      m1 += 1;
      p += 1;
    }
  }
  return 0;
}
