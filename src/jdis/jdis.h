#include "../avl/bst.h"

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
//  d'échec renvoie une valeur strictement négative.
extern size_t card_intersection(bst *p, bst *q);

// jdis : Retourne la distance de Jaccard entre les arbres binaire de recherche
//  associés à p et à q.
extern double jdis(bst *p, bst *q, size_t card_interction);
