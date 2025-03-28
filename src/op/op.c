#include <stdlib.h>
#include <stdio.h>

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

//EN SUPPOSANT QU'ON MET TOUTES LES OPTIONS D'ABORD ET QUE TOUS LES FICHIERS SE
// SUIVENT SUR LA LIGNE DE COMMANDE

//int graph_belonging(bst **t) {
  //bst *union = bst_empty((*t)->);
//}

//static int scptr_display(const char *ref){
  //return printf();
//}

int set_max_value(int value, int limit){
  if (value == 0){
    value = limit;
  }
  return 0;
}

// MANQUE UN PARAMÈTRE OU ON STOCKE LE RÉSULTAT DE LA FONCTION
// MANQUE GESTION D'ERREUR
int punctuation_like_space(char *buffer){
  char *m1;
  char *p = buffer;
  while(p != nullptr){
    if (*p == '.' && *p != ';' && *p != ',' && *p != '-'){
      *m1 = *p;
      m1 += 1;
      p += 1;
    }
  }
  return 0;
}
