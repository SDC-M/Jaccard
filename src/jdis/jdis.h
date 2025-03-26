#include "bst.h"

// bst_jdis : Tente de calculer la distance de Jaccard des ensembles représentés
// par les avl associés à p1 et p2; En cas d'échec renvoie une valeur
// strictement supérieur à 1; renvoie sinon la distance de Jaccard calculé
size_t bst_jdis(const bst *p1, const bst *p2);

// bst_intersec : Tente d'allouer les ressources nécessaires pour un avl
// correspondant à l'intersection des ensembles représentés par les avl associés
// à p1 et p2. En cas de dépassement de capacité, libère les ressources allouées
// précédemment et renvoie un pointeur nul. Sinon renvoie un pointeur associé à
// l'avl créé
bst *bst_intersec(const bst *p1, const bst *p2);

// bst_union : Tente d'allouer les ressources nécessaires pour un avl
// correspondant à l'union des ensembles représentés par les avl associés à p1
// et p2. En cas de dépassement de capacité, libère les ressources allouées
// précédemment et renvoie un pointeur nul. Sinon renvoie un pointeur associé à
// l'avl créé
bst *bst_union(const bst *p1, const bst *p2);
