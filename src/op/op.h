#include "../avl/bst.h"

typedef struct context context;

// help : Affiche le menu d'aide sur la sortie standard et renvoie 0.
extern void help(void);

// graph_belonging : Affiche le graphe d'appartenance des mots dans les fichiers
//  dans argv.
extern int graph_belonging(bst **t, bst *uni, int nb_file);
