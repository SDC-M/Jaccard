#include <stdlib.h>
#include <stdio.h>

#include "op.h"

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

static int scptr_display(bst *context, const char *ref) {
  printf("%s\t", ref);
  if (bst_search(context, ref) != nullptr) {
    printf("x\t");
  } else {
    printf("-\t");
  }
  return printf("\n");
}

int graph_belonging(bst **t, bst *uni) {
  return bst_dft_infix_apply_context(uni, 0, t,(int (*)(void *, const void *)) scptr_display, nullptr,
        nullptr);
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
