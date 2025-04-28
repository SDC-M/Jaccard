#include "../avl/bst.h"
#include "../holdall/holdall.h"

// struct bst, bst : type et nom de type d'un contrôleur regroupant les
//  informations nécessaires pour gérer un contexte avec un compteur et un
//  ensemble d'elements quelconques.
typedef struct ctx ctx;

// is_in : Incrémente le compteur de context si une référence égale au sens de
//  la fonction de comparaison à ref dans l'ensemble du context et renvoie une
//  valeur nulle.
extern int is_in(ctx *context, const void *ref);

// card_intersection : Renvoie le cardinal de l'intersection des ensembles
//  représenté par les arbres bianaire de recherche associés à p et à q. En cas
//  d'échec renvoie (size_t) -1.
extern size_t card_intersection(bst *p, bst *q);

// jdis : Retourne la distance de Jaccard entre les arbres binaire de recherche
//  associés à p et à q.
extern double jdis(bst *p, bst *q, size_t card_interction);

// file_to_bst : Tente de créer l'avl associé aux valeurs stockées dans le
//  fichier de nom file_name dans lequel on tente de lire ainsi que d'ajouter
//  les valeurs stockées dans le fourre-tout pointée par words. En cas de succès
//  renvoie l'avl associé, en cas d'erreur retourne nullptr et désalloue toute
//  zone mémoire précédemment allouée, si le nom de fichier ou le fourre-tout
//  sont des pointeurs null, retourne nullptr.
extern bst *file_to_bst(char *file_name, size_t value_max, bool wp,
    bst *uni_words, holdall *words);
