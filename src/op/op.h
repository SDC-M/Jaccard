#include "bst.h"

// help : Affiche le menu d'aide sur la sortie standard et renvoie 0.
extern int help();

// graph_belonging : Affiche le graphe d'appartenance des mots dans les fichiers
// dans argv.
extern int graph_belonging(bst *t, int fst_file, int argc, char *argv[]);

// set_max_value : Fixe value à la limit si value est nulle sinon fixe la
// longueur maximale des mots à value
extern int set_max_value(int value, int limit);

// punctuation_like_space : Sans effet si buffer ne possède pas de ponctuation.
// Sinon découpe le buffer en plusieurs mots sans les caractères de ponctuation.
extern int punctuation_like_space(char *buffer);
