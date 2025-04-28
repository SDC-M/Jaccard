//  bst.c : partie implantation d'un module polymorphe pour la spécification
//   ABINR du TDA ABinR(T).

#include "bst.h"

//=== Type cbst ================================================================

//--- Définition cbst ----------------------------------------------------------

typedef struct cbst cbst;

struct cbst {
  cbst *next[2];
  const void *ref;
  int height;
  size_t size;
};

//--- Raccourcis cbst ----------------------------------------------------------

#define EMPTY()     nullptr
#define IS_EMPTY(p) ((p) == nullptr)
#define LEFT(p)     ((p)->next[0])
#define RIGHT(p)    ((p)->next[1])
#define REF(p)      ((p)->ref)
#define HEIGHT(p) ((p)->height)
#define NEXT(p, d)  ((p)->next[(d) > 0])
#define SIZE(p) ((p)->size)

//--- Divers -------------------------------------------------------------------

// add__size_t : renvoie la somme des deux size_t x1 et x2.
static size_t add__size_t(size_t x1, size_t x2) {
  return x1 + x2;
}

// max__int renvoie le maximum entre les deux entiers x1 et x2.
static int max__int(int x1, int x2) {
  return x1 > x2 ? x1 : x2;
}

// min__size_t : renvoie le minimum entre x1 et x2.
static size_t min__size_t(size_t x1, size_t x2) {
  return x1 < x2 ? x1 : x2;
}

//--- Fonctions cbst -----------------------------------------------------------

//  DEFUN_CBST__MEASURE : définit la fonction récursive de nom « cbst__ ## fun »
//    et de paramètre un pointeur d'arbre binaire, qui renvoie zéro si l'arbre
//    est vide et « 1 + oper(r0, r1) » sinon, où r0 et r1 sont les valeurs
//    renvoyées par les appels récursifs de la fonction avec les pointeurs des
//    sous-arbres gauche et droit de l'arbre comme paramètres.
#define DEFUN_CBST__MEASURE(fun, oper)                                         \
  static size_t cbst__ ## fun(const cbst * p) {                                \
    return IS_EMPTY(p)                                                         \
      ? 0                                                                      \
      : 1 + oper(cbst__ ## fun(LEFT(p)), cbst__ ## fun(RIGHT(p)));             \
  }

//  cbst__size, cbst__height, cbst__distance : définitions.

// cbst__size : si l'arbre binaire de recherche pointé par p est vide renvoie 0
//    sinon sa taille.
static size_t cbst__size(const cbst *p) {
  return IS_EMPTY(p) ? 0 : SIZE(p);
}

// cbst__height : si l'arbre binaire de recherche pointé par p est vide renvoie
//    0 sinon sa hauteur.
static int cbst__height(const cbst *p) {
  return IS_EMPTY(p) ? 0 : HEIGHT(p);
}

// cbst__update_height : met à jour la taille de l'arbre binaire de recherche
//    pointé par p en faisant la somme de c'est deux sous-arbre plus un.
static void cbst__update_height(cbst *p) {
  HEIGHT(p) = 1 + max__int(cbst__height(RIGHT(p)), cbst__height(LEFT(p)));
}

// cbst__update_size : met à jour la taille de l'arbre pointé par p en sommant
//    la taille de ses deux sous-arbres plus un.
static void cbst__update_size(cbst *p) {
  SIZE(p) = 1 + add__size_t(cbst__size(RIGHT(p)), cbst__size(LEFT(p)));
}

// cbst__balance : si l'arbre binaire de recherche pointé par p est vide renvoie
//     0 sinon renvoie la diffèrence entre son sous-arbre gauche et droit.
static int cbst__balance(const cbst *p) {
  return IS_EMPTY(p) ? 0 : cbst__height(LEFT(p)) - cbst__height(RIGHT(p));
}

// cbst__rotation_left : effectue une simple rotation gauche  de l'arbre pointé
//     par *pp.
static void cbst__rotation_left(cbst **pp) {
  cbst *p = *pp;
  *pp = RIGHT(p);
  RIGHT(p) = LEFT(*pp);
  LEFT(*pp) = p;
  cbst__update_size(LEFT(*pp));
  cbst__update_height(LEFT(*pp));
  cbst__update_size(*pp);
  cbst__update_height(*pp);
}

// cbst__rotation_right : effectue une simple rotation droite de l'arbre pointé
//     par *pp.
static void cbst__rotation_right(cbst **pp) {
  cbst *p = *pp;
  *pp = LEFT(p);
  LEFT(p) = RIGHT(*pp);
  RIGHT(*pp) = p;
  cbst__update_size(RIGHT(*pp));
  cbst__update_height(RIGHT(*pp));
  cbst__update_size(*pp);
  cbst__update_height(*pp);
}

// cbst__rotation_left_right : effectue une double rotation gauche droite de
//    l'arbre pointé par *pp.
static void cbst__rotation_left_right(cbst **pp) {
  cbst__rotation_left(&LEFT(*pp));
  cbst__rotation_right(pp);
}

// cbst__rotation_right_left : effectue une double rotation droite gauche de
//    l'arbre pointé par *pp.
static void cbst__rotation_right_left(cbst **pp) {
  cbst__rotation_right(&(RIGHT(*pp)));
  cbst__rotation_left(pp);
}

#if defined WANT_PRINT_ROT

// cbst__balancing : effectue les rotations nécéssaire pour que l'arbre pointé
//    par *pp soit équillibré pour la hauteur.
static int cbst__balancing(cbst **pp) {
  cbst__update_height(*pp);
  cbst__update_size(*pp);
  int balance = cbst__balance(*pp);
  if (balance == 0 || balance == -1 || balance == 1) {
    return 0;
  } else if (balance == 2) {
    int ba = cbst__balance(LEFT(*pp));
    if (ba == 1 || ba == 0) {
      printf("right\n");
      cbst__rotation_right(pp);
    } else {
      printf("left right\n");
      cbst__rotation_left_right(pp);
    }
  } else if (balance == -2) {
    int ba = cbst__balance(RIGHT(*pp));
    if (ba == -1 || ba == 0) {
      printf("left\n");
      cbst__rotation_left(pp);
    } else {
      printf("right left\n");
      cbst__rotation_right_left(pp);
    }
  }
  return 1;
}

#else
static int cbst__balancing(cbst **pp) {
  cbst__update_height(*pp);
  cbst__update_size(*pp);
  int balance = cbst__balance(*pp);
  if (balance == 0 || balance == -1 || balance == 1) {
    return 0;
  } else if (balance == 2) {
    int ba = cbst__balance(LEFT(*pp));
    if (ba == 1 || ba == 0) {
      cbst__rotation_right(pp);
    } else {
      cbst__rotation_left_right(pp);
    }
  } else if (balance == -2) {
    int ba = cbst__balance(RIGHT(*pp));
    if (ba == -1 || ba == 0) {
      cbst__rotation_left(pp);
    } else {
      cbst__rotation_right_left(pp);
    }
  }
  return 1;
}

#endif

DEFUN_CBST__MEASURE(distance, min__size_t)

//  cbst__dispose : libère les ressources allouées à l'arbre binaire de
//    recherche pointé par p.
static void cbst__dispose(cbst *p) {
  if (!IS_EMPTY(p)) {
    cbst__dispose(LEFT(p));
    cbst__dispose(RIGHT(p));
    free(p);
  }
}

// cbst__search : recherche un élément egal à ref au sens de la fonction compar
//    dans l'arbre binaire de recherche pointé par p.
static void *cbst__search(const cbst *p, const void *ref,
    int (*compar)(const void *, const void *)) {
  if (IS_EMPTY(p)) {
    return nullptr;
  }
  int c = compar(ref, REF(p));
  if (c == 0) {
    return (void *) REF(p);
  }
  return cbst__search(NEXT(p, c), ref, compar);
}

// cbst__add_endofpath : tente d'ajouter en bout de chemin  ref à l'arbre
//     binaire de recherche pointé par *pp, en cas d'échec retourne nullptr.
static void *cbst__add_endofpath(cbst **pp, const void *ref,
    int (*compar)(const void *, const void *)) {
  if (IS_EMPTY(*pp)) {
    *pp = malloc(sizeof(cbst));
    if ((*pp) == nullptr) {
      return nullptr;
    }
    REF(*pp) = ref;
    LEFT(*pp) = EMPTY();
    RIGHT(*pp) = EMPTY();
    SIZE(*pp) = 1;
    HEIGHT(*pp) = 1;
    return (void *) ref;
  } else {
    int c = compar(ref, REF(*pp));
    if (c == 0) {
      return (void *) REF(*pp);
    }
    void *res = cbst__add_endofpath(&(NEXT(*pp, c)), ref, compar);
    cbst__update_size(*pp);
    cbst__balancing(pp);
    return res;
  }
}

// cbst__remove_max : supprime le noeud ayant la valeur maximale dans l'arbre
//    de recherche pointée par *pp.
static void *cbst__remove_max(cbst **pp) {
  if (IS_EMPTY(*pp)) {
    return nullptr;
  }
  if (IS_EMPTY(RIGHT(*pp))) {
    const void *max = REF(*pp);
    cbst *t = *pp;
    *pp = LEFT(*pp);
    free(t);
    return (void *) max;
  }
  void *res = cbst__remove_max(&(RIGHT(*pp)));
  cbst__update_size(*pp);
  cbst__balancing(pp);
  return res;
}

// cbst__remove_root : supprime la racine de l'arbre de recherche pointée par
//    *pp et fait remonter la valeur maximale de son sous-arbre gauche.
static void cbst__remove_root(cbst **pp) {
  const void *v = cbst__remove_max(&LEFT(*pp));
  cbst__update_size(*pp);
  cbst__balancing(pp);
  if (v != nullptr) {
    REF(*pp) = v;
  } else {
    cbst *tmp = *pp;
    *pp = RIGHT(tmp);
    free(tmp);
  }
}

// cbst__remove_climbup_left : supprime la valeur égale à ref au sens de la
//    focntion compar et effectue une remontée gauche dans l'arbre binaire
//    de recherche pointée par *pp.
static void *cbst__remove_climbup_left(cbst **pp, const void *ref,
    int (*compar)(const void *, const void *)) {
  if (IS_EMPTY(*pp)) {
    return nullptr;
  }
  int c = compar(ref, REF(*pp));
  if (c == 0) {
    const void *val_ref = REF(*pp);
    cbst__remove_root(pp);
    return (void *) val_ref;
  }
  void *res = cbst__remove_climbup_left(&NEXT(*pp, c), ref, compar);
  cbst__update_size(*pp);
  cbst__balancing(pp);
  return res;
}

// cbst__rank : retourne le rang de l'element égal à ref au sens de la fonction
//    compar dans l'arbre binaire de recherche pointé par p.
static size_t cbst__rank(const cbst *p, const void *ref,
    int (*compar)(const void *, const void *), size_t rank) {
  if (IS_EMPTY(p)) {
    return rank;
  }
  int c = compar(ref, REF(p));
  if (c == 0) {
    return rank + cbst__size(LEFT(p));
  }
  if (c > 0) {
    rank += 1 + cbst__size(LEFT(p));
  }
  return cbst__rank(NEXT(p, c), ref, compar, rank);
}

// cbst__number : retourne le numéro de l'element égal à ref au sens de la
//    fonction compar dans l'arbre binaire de recherche pointé par p.
static size_t cbst__number(const cbst *p, const void *ref,
    int (*compar)(const void *, const void *), size_t number) {
  if (IS_EMPTY(p)) {
    return number;
  }
  int c = compar(ref, REF(p));
  if (c == 0) {
    return number;
  }
  number *= 2;
  if (c > 0) {
    number += 1;
  }
  return cbst__number(NEXT(p, c), ref, compar, number);
}

#define REPR__TAB 4

#define REPR_GRAPH_EMPTY "ᗕ"
#define REPR_LEFT_BRANCH "ᒪ---"
#define REPR_RIGHT_BRANCH "ᒥ---"
#define REPR_VERT_LINE "ᑊ"
#define REPR_GRAPH_SEP " "

//  cbst__repr_graphic : affiche la représentation graphique par rotation
//    antihoraire d'un quart de tour du sous-arbre binaire p avec une
//    indentation par niveau de REPR_TAB caractères. Le niveau du sous-arbre est
//    supposé être la valeur de level.

static void cbst__repr_graphic(const cbst *p, void (*put)(const void *ref),
    size_t level, size_t num) {
  if (IS_EMPTY(p)) {
    if (level != 0) {
      for (size_t i = 1; i < level; ++i) {
        if (((num >> (level - i)) % 2) != ((num >> (level - i - 1)) % 2)) {
          printf(REPR_VERT_LINE);
          printf("%*s", (int) (REPR__TAB - 1), REPR_GRAPH_SEP);
        } else {
          printf("%*s", (int) (REPR__TAB), REPR_GRAPH_SEP);
        }
      }
      printf((num % 2 == 1) ? REPR_RIGHT_BRANCH : REPR_LEFT_BRANCH);
    }
    printf(REPR_GRAPH_EMPTY "\n");
  } else {
    cbst__repr_graphic(RIGHT(p), put, level + 1, 1 + 2 * num);
    if (level != 0) {
      for (size_t i = 1; i < level; ++i) {
        if (((num >> (level - i)) % 2) != ((num >> (level - i - 1)) % 2)) {
          printf(REPR_VERT_LINE);
          printf("%*s", (int) (REPR__TAB - 1), REPR_GRAPH_SEP);
        } else {
          printf("%*s", (int) (REPR__TAB), REPR_GRAPH_SEP);
        }
      }
      printf((num % 2 == 1) ? REPR_RIGHT_BRANCH : REPR_LEFT_BRANCH);
    }
    put(REF(p));
    printf(" b =  %d h = %d t = %zu", cbst__balance(p), cbst__height(p),
        cbst__size(p));
    printf("\n");
    cbst__repr_graphic(LEFT(p), put, level + 1, 2 * num);
  }
}

static int cbst__dft_infix_apply_context(cbst *t, int dir,
    void *context, int (*fun)(void *context, const void *ref),
    int (*fun_pre)(void *context), int (*fun_post)(void *context)) {
  if (t == nullptr) {
    return 0;
  }
  cbst *fst = dir <= 0 ? RIGHT(t) : LEFT(t);
  cbst *snd = dir > 0 ? RIGHT(t) : LEFT(t);
  if (fun_pre != nullptr) {
    int r = 0;
    if ((r = fun_pre(context)) != 0) {
      return r;
    }
  }
  cbst__dft_infix_apply_context(fst, dir, context, fun, fun_pre, fun_post);
  int r = 0;
  if ((r = fun(context, REF(t))) != 0) {
    return r;
  }
  if (fun_post != nullptr) {
    int r = 0;
    if ((r = fun_post(context)) != 0) {
      return r;
    }
  }
  cbst__dft_infix_apply_context(snd, dir, context, fun, fun_pre, fun_post);
  return 0;
}

//=== Type bst =================================================================

//--- Définition bst -----------------------------------------------------------

struct bst {
  int (*compar)(const void *, const void *);
  cbst *root;
};

//--- Fonctions bst ------------------------------------------------------------

bst *bst_empty(int (*compar)(const void *, const void *)) {
  bst *t = malloc(sizeof *t);
  if (t == nullptr) {
    return nullptr;
  }
  t->compar = compar;
  t->root = EMPTY();
  return t;
}

void bst_dispose(bst **tptr) {
  if (*tptr == nullptr) {
    return;
  }
  cbst__dispose((*tptr)->root);
  free(*tptr);
  *tptr = nullptr;
}

void *bst_remove_climbup_left(bst *t, const void *ref) {
  if (ref == nullptr) {
    return nullptr;
  }
  return cbst__remove_climbup_left(&(t->root), ref, t->compar);
}

void *bst_add_endofpath(bst *t, const void *ref) {
  return cbst__add_endofpath(&(t->root), ref, t->compar);
}

void *bst_search(bst *t, const void *ref) {
  return cbst__search(t->root, ref, t->compar);
}

size_t bst_size(bst *t) {
  return cbst__size(t->root);
}

int bst_height(bst *t) {
  return cbst__height(t->root);
}

size_t bst_distance(bst *t) {
  return cbst__distance(t->root);
}

size_t bst_rank(bst *t, const void *ref) {
  if (ref == nullptr) {
    return (size_t) -1;
  }
  return cbst__rank(t->root, ref, t->compar, 0);
}

size_t bst_number(bst *t, const void *ref) {
  if (ref == nullptr) {
    return (size_t) -1;
  }
  return cbst__number(t->root, ref, t->compar, 1);
}

void bst_repr_graphic(bst *t, void (*put)(const void *ref)) {
  if (IS_EMPTY(t->root)) {
    return;
  }
  cbst__repr_graphic(t->root, put, 0, 1);
}

int bst_dft_infix_apply_context(bst *t, int dir,
    void *context, int (*fun)(void *context, const void *ref),
    int (*fun_pre)(void *context), int (*fun_post)(void *context)) {
  return cbst__dft_infix_apply_context(t->root, dir, context, fun, fun_pre,
        fun_post);
}
