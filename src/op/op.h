#include "../avl/bst.h"

typedef struct context context;

// help : Affiche le menu d'aide sur la sortie standard et renvoie 0.
extern void help(void);

// graph_belonging : Affiche le graphe d'appartenance des mots dans les fichiers
//  dans argv.
extern int graph_belonging(bst **t, bst *uni, int nb_file);

// set_max_value : Fixe value à la limit si value est nulle sinon fixe la
//  longueur maximale des mots à value
extern int set_max_value(int value, int limit);

// punctuation_like_space : Sans effet si buffer ne possède pas de ponctuation.
//  Sinon découpe le buffer en plusieurs mots sans les caractères de
// ponctuation.
extern int punctuation_like_space(char *buffer);

// add_element : Renvoie une valeur nulle si l'ajout en bout de chemin de ref
// dans l'arbre binaire de recherche associé à t se passe correctement. Renvoie
// une valeur différente de nulle sinon
extern int add_element(bst *t, const void *ref);
