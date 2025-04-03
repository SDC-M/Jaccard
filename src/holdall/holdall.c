//  holdall.c : partie implantation du module holdall.

#include "holdall.h"
#include <stdio.h>

#define CAPACITY_MIN 2
#define CAPACITY_MUL 2

struct holdall {
  void **aref;
  size_t cap;
  size_t count;
};

holdall *holdall_empty() {
  holdall *ha = malloc(sizeof *ha);
  if (ha == nullptr) {
    return nullptr;
  }
  ha->aref = malloc(CAPACITY_MIN * sizeof(const void *));
  if (ha->aref == nullptr) {
    free(ha);
    return nullptr;
  }
  ha->cap = CAPACITY_MIN;
  ha->count = 0;
  return ha;
}

void holdall_dispose(holdall **haptr) {
  if (*haptr == nullptr) {
    return;
  }
  for (size_t i = 0; i < (*haptr)->count; i += 1) {
    free((*haptr)->aref[i]);
  }
  free((*haptr)->aref);
  free(*haptr);
  *haptr = nullptr;
}

// double_capacity : Renvoie le fourre-tout avec la capacité de son tableau
// dynamique doublée. Renvoie un pointeur nul en cas d'échec
static void *double_capacity(holdall *ha) {
  void **new_array = realloc(ha->aref,
        CAPACITY_MUL * ha->cap * sizeof(*(ha->aref)));
  if (new_array == nullptr) {
    return nullptr;
  }
  ha->cap *= CAPACITY_MUL;
  ha->aref = new_array;
  return ha;
}

int holdall_put(holdall *ha, void *ref) {
  if (ha->count == ha->cap) {
    double_capacity(ha);
  }
  for (size_t i = 0; i < ha->count; ++i) {
    void *val = ha->aref[i];
    if (val == ref) {
      return 1;
    }
  }
  ha->aref[ha->count] = ref;
  ha->count += 1;
  return 0;
}

size_t holdall_count(holdall *ha) {
  return ha->count;
}

int holdall_apply(holdall *ha,
    int (*fun)(void *)) {
  for (size_t i = 0; i < ha->count; ++i) {
    int r = fun(ha->aref[i]);
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

int holdall_apply_context(holdall *ha,
    void *context, void *(*fun1)(void *context, void *ptr),
    int (*fun2)(void *ptr, void *resultfun1)) {
  for (size_t i = 0; i < ha->count; ++i) {
    int r = fun2(ha->aref[i], fun1(context, ha->aref[i]));
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

int holdall_apply_context2(holdall *ha,
    void *context1, void *(*fun1)(void *context1, void *ptr),
    void *context2, int (*fun2)(void *context2, void *ptr, void *resultfun1)) {
  for (size_t i = 0; i < ha->count; ++i) {
    int r = fun2(context2, ha->aref[i], fun1(context1, ha->aref[i]));
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

#if defined HOLDALL_WANT_EXT && HOLDALL_WANT_EXT != 0

static void swap(const void **aref, size_t i, size_t j) {
  const void *tmp = aref[i];
  aref[i] = aref[j];
  aref[j] = tmp;
}

//  heapsort_down : il est supposé que base est l'adresse du premier composant
//    d'un tableau de longueur nmemb et de taille de composants size, que
//    nmemb >= 1, que k <= nmemb - 1 et que le tableau est un maximier sur
//    [ k + 1 ... nmemb - 1 ] relativement à la fonction de comparaison pointée
//    par compar. Descend le composant d'indice k à la bonne place de manière à
//    faire du tableau un maximier sur [ k ... nmemb - 1 ].

static void heapsort_down(char *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *), size_t k) {
  size_t i = k;
  while (true) {
    size_t max = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;
    if (left < nmemb && compar(base + left * size, base + max * size) > 0) {
      max = left;
    }
    if (right < nmemb && compar(base + right * size, base + max * size) > 0) {
      max = right;
    }
    if (max == i) {
      break;
    }
    mem_swap(base + i * size, base + max * size, size);
    i = max;
  }
}

void holdall_sort(holdall *ha
    int (*compar)(const void *, const void *)) {
  for (size_t k = 0 ; k < ha->count; k += 1) {
    size_t icur = ((nmemb / 2) - k - 1);
    heapsort_down((char *) base, nmemb, size, compar, icur);
  }
  for (size_t nb = ha->count; nb > 0; --nb) {
    swap(ha->aref, nb, 0);
    heapsort_down(ha->aref, ha->count, 1, compar, 0);
  }
}

#endif
