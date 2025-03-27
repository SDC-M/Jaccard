#include <stdlib.h>
#include "bst.h"

int help() {
  fprintf(stdout, "Calcul distance de Jaccard\n");
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

int graph_belonging(/*Paramètre WIP*/) {

}

int set_maxlength()

// rfree : libère la mémoire pointé par ptr et renvoie 0.
