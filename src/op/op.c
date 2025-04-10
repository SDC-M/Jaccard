#include <stdlib.h>
#include <stdio.h>

#include "op.h"

struct context {
  bst **t;
  int nb_bst;
};

void help(void) {
  fprintf(stdout,
      "Utilisation : ./jdis [OPTIONS]... FICHIER1 FICHIER2 ...\n\n");
  fprintf(stdout,
      "Liste des opérations réalisables :\n\n");
  fprintf(stdout, " -? : Affiche ce menu d'aide.\n");
  fprintf(stdout,
      " -g : Affiche le graphe d'appartenance des mots."
      "dans les fichiers passés sur la ligne de commande.\n");
  fprintf(stdout, " -i VALUE : permet de fixer la longueur maximale."
      "des mots à VALUE.\n");
  fprintf(stdout, " -p : Considère les caractères de ponctuation."
      "comme des caractères d'espacement.\n");
  fprintf(stdout,
      " -- : Indique que l'argument qui suit doit être "
      "considéré comme un fichier.\n");
  fprintf(stdout, " - : À la place d'un nom de fichier indique qu'on prend en "
      "concidération l'entrée standart.\n");
}

static int scptr_display(context *ctx, const char *ref) {
  printf("%s\t", ref);
  for (int i = 0; i < ctx->nb_bst; ++i) {
    if (bst_search(ctx->t[i], ref) != nullptr) {
      fprintf(stdout, "x\t");
    } else {
      fprintf(stdout,"-\t");
    }
  }
  printf("\n");
  return 0;
}

int graph_belonging(bst **t, bst *uni, int nb_file) {
  context *ctx = malloc(sizeof *ctx);
  if (ctx == nullptr) {
    return -1;
  }
  ctx->nb_bst = nb_file;
  ctx->t = t;
  bst_dft_infix_apply_context(uni, 1, ctx,
      (int (*)(void *, const void *)) scptr_display, nullptr, nullptr);
  free(ctx);
  return 0;
}
