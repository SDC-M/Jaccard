//  holdall.c : partie implantation du module holdall.

#include "holdall.h"
#include <stdio.h>

#define CAPACITY_MIN 2
#define CAPACITY_MUL 2

#define VALUE_ALREADY_EXIST 1
#define LACK_OF_MEMORY -1

struct holdall {
  const void **aref;
  size_t cap;
  size_t card;
};

holdall *holdall_empty() {
  holdall *ha = malloc(sizeof *ha);
  if (ha == nullptr) {
    return nullptr;
  }
  ha->aref = malloc(CAPACITY * sizeof(*(ha->aref)));
  if (ha->aref == nullptr) {
    free(ha);
    return nullptr;
  }
  ha->cap = CAPACITY_MIN;
  ha->card = 0;
  return ha;
}

void holdall_dispose(holdall **haptr) {
  if (*haptr == nullptr) {
    return;
  }
  for (size_t i = 0; i < (*haptr)->card; i += 1) {
    free((*haptr)->aref[i]);
  }
  free((*haptr)->aref);
  free(*haptr);
  *haptr = nullptr;
}

int holdall_put(holdall *ha, void *ref) {
  if (ha->card == ha->cap) {
    const void **new_array = realloc(ha->aref,
          CAPACITY_MUL * ha->cap * sizeof(*(ha->aref)));
    if (new_array == nullptr){
      return LACK_OF_MEMORY;
    }
  }
  for (size_t i = 0; i < ha->card; ++i) {
    const void *val = ha->aref[i];
    if (val == ref) {
      return VALUE_ALREADY_EXIST;
    }
  }
  ha->aref[ha->card] = ref;
  ha->card += 1;
  return 0;
}

size_t holdall_card(holdall *ha) {
  return ha->card;
}

int holdall_apply(holdall *ha,
    int (*fun)(void *)) {
  for (const choldall *p = ha->head; p != nullptr; p = p->next) {
    int r = fun(p->ref);
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

int holdall_apply_context(holdall *ha,
    void *context, void *(*fun1)(void *context, void *ptr),
    int (*fun2)(void *ptr, void *resultfun1)) {
  for (const choldall *p = ha->head; p != nullptr; p = p->next) {
    int r = fun2(p->ref, fun1(context, p->ref));
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

int holdall_apply_context2(holdall *ha,
    void *context1, void *(*fun1)(void *context1, void *ptr),
    void *context2, int (*fun2)(void *context2, void *ptr, void *resultfun1)) {
  for (const choldall *p = ha->head; p != nullptr; p = p->next) {
    int r = fun2(context2, p->ref, fun1(context1, p->ref));
    if (r != 0) {
      return r;
    }
  }
  return 0;
}

#if defined HOLDALL_WANT_EXT && HOLDALL_WANT_EXT != 0

#if 0
static void swap(choldall *a, choldall *b) {
  void *temp = a->ref;
  a->ref = b->ref;
  b->ref = temp;
}

extern void holdall_sort(holdall *ha,
    int (*compar)(const void *, const void *)) {
  if (ha->head == nullptr) {
    return;
  }
  int swapped;
  choldall *p = nullptr;
  choldall *q = nullptr;
  for (size_t i = 0;; ++i) {
    swapped = 0;
    p = ha->head;
    while (p->next != nullptr) {
      q = p->next;
      if (compar(p->ref, q->ref) > 0) {
        swap(p, q);
        swapped = 1;
      }
      p = p->next;
    }
    if (swapped == 0) {
      break;
    }
  }
}

#elif 1
static choldall *merge(choldall *p, size_t P, choldall *q, size_t Q,
    int (*compar)(const void *, const void *)) {
  for (size_t i = 0; i < P + Q - 1; ++i) {
    if (compar(p->next->ref, q->next->ref) > 0) {
      choldall *t = q->next;
      q->next = t->next;
      t->next = p->next;
      p->next = t;
      if (Q == 1) {
        break;
      }
      Q = Q - 1;
    } else {
      if (P == 1) {
        while (Q >= 1) {
          q = q->next;
          Q = Q - 1;
        }
        break;
      }
      P = P - 1;
    }
    p = p->next;
  }
  return q;
}

static choldall *holdall_sort_aux(choldall *p, size_t n,
    int (*compar)(const void *, const void *)) {
  size_t Q = n / 2;
  size_t P = n - Q;
  choldall *q = nullptr;
  if (P >= 2) {
    q = holdall_sort_aux(p, P, compar);
    if (Q >= 2) {
      holdall_sort_aux(q, Q, compar);
    }
  } else {
    q = p->next;
  }
  q = merge(p, P, q, Q, compar);
  return q;
}

extern void holdall_sort(holdall *ha,
    int (*compar)(const void *, const void *)) {
  if (ha->head == nullptr) {
    return;
  }
  holdall_sort_aux(ha->head, ha->card, compar);
}

#endif
#endif
