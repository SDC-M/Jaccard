#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <locale.h>
#include <getopt.h>

#include "avl/bst.h"
#include "holdall/holdall.h"
#include "jdis/jdis.h"
#include "op/op.h"

//------------------------------------------------------------------------------

#define WORD_MAX_SIZE 50

#define PUNC_SEPARATORS "“`‘\'\"-_)(;,!?:.'\n "
#define CLASSIC_SEPARATORS "\n "


// hm__fscanf : tente de lire dans le fichier pointé par stream au plus
//  value_max caractères, stocke dans la zone pointée par buffer et si wp est à
//  vrai alors gère la ponctuation comme séparateur.
static int hm__fscanf(FILE *stream, int value_max, char *buffer, bool wp) {
  int c;
  char *p = buffer;
  int cptr = 0;
  const char *punc = wp == true ? PUNC_SEPARATORS : CLASSIC_SEPARATORS;
  while ((c = fgetc(stream)) != EOF) {
    if (strchr(punc, c) != nullptr || cptr >= value_max) {
      *p = '\0';
      return 0;
    }
    *p = (char) c;
    ++p;
    ++cptr;
  }
  *p = '\0';
  return EOF;
}


void put(const void *p) {
  printf("%s", (const char *)p);
}

//------------------------------------------------------------------------------

int rfree(void *ptr) {
  free(ptr);
  return 0;
}

// add_element : Renvoie une valeur nulle si l'ajout en bout de chemin de ref
// dans l'arbre binaire de recherche associé à t se passe correctement. Renvoie
// une valeur différente de nulle sinon
int add_element(bst *t, const void *ref) {
  return (bst_add_endofpath(t, ref) != ref) ? 1 : 0;
}

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  int option;
  bool graph = false;
  int escaped_file = 0;
  int value_max = 0;
  bool want_punc = false;
  while ((option = getopt(argc, argv, "?pg-:i:")) != -1) {
    switch (option) {
      case '?':
        help();
        return EXIT_SUCCESS;
      case 'p':
        want_punc = true;
        break;
      case '-':
        escaped_file += 1;
        break;
      case 'i':
        value_max = atoi(optarg);
        break;
      case 'g':
        graph = true;
        break;
      default:
        return EXIT_FAILURE;
    }
  }
  if (argc - optind + escaped_file < 2) {
    fprintf(stderr, "Pas assez d'arguments.\n");
    return EXIT_FAILURE;
  }
  holdall *words = holdall_empty();
  bst **tab = malloc((size_t) (argc - optind + escaped_file) * sizeof(bst *));
  for (int i = optind - escaped_file; i < argc; ++i) {
    printf("%s\n", argv[i]);
    if (strcmp(argv[i], "-") == 0) {
      FILE *p = tmpfile();
      if (p == nullptr) {
        fprintf(stderr, "Erreur lors de l'allocation pour l'entrée standart.");
        goto dispose;
      }
      int c;
      while ((c = fgetc(stdin)) != EOF) {
        printf("%c", c);
        fputc(c, p);
      }
      rewind(p);
      if (value_max == 0) {
        value_max = WORD_MAX_SIZE;
      }
      char x[value_max + 1];
      bst *bst_f = bst_empty((int (*)(const void *, const void *)) strcoll);
      if (bst_f == nullptr) {
        fclose(p);
      }
      int r = hm__fscanf(p, value_max, x, want_punc);
      while (r != EOF) {
        char *z = malloc(strlen(x) + 1);
        strcpy(z, x);
        char *res;
        if ((res = bst_add_endofpath(bst_f, z)) == nullptr) {
          fclose(p);
          bst_dispose(&bst_f);
        }
        if (res == z) {
          holdall_put(words, z);
        } else {
          free(z);
        }
        r = hm__fscanf(p, value_max, x, want_punc);
      }
      fclose(p);
      bst_repr_graphic(bst_f, put);
      tab[i - optind] = bst_f;
    } else {
      tab[i - optind] = file_to_bst(argv[i], words, value_max, want_punc);
    }
    if (tab[i - optind] == nullptr) {
      fprintf(stderr, "*** Erreur de l'allocation pour le fichier : %s\n",
          argv[i]);
      for (int j = optind; j < i; ++j) {
        bst_dispose(&tab[j - optind]);
      }
      free(tab);
      return EXIT_FAILURE;
    }
  }
  for (int i = 0; i < argc - optind - 1; ++i) {
    for (int j = i + 1; j < argc - optind; ++j) {
      size_t card_in = card_intersection(tab[i], tab[j]);
      double jaccard_distance = jdis(tab[i], tab[j], card_in);
      printf("%s\t%s: %.4lf\n", argv[i + optind], argv[j + optind],
          jaccard_distance);
    }
  }
  if (graph) {
    bst *uni = bst_empty((int (*)(const void *, const void *)) strcoll);
    if (uni == nullptr) {
      fprintf(stderr, "Erreur d'allocation sur l'union\n");
      goto dispose;
    }
    for (int i = 0; i < argc - optind; ++i) {
      bst_dft_infix_apply_context(tab[i], 0, uni,
          (int (*)(void *, const void *)) add_element, nullptr, nullptr);
    }
    for (int i = 0; i < argc - optind; ++i) {
      printf("\t%s", argv[i + optind]);
    }
    printf("\n");
    graph_belonging(tab, uni, argc - optind);
    bst_dispose(&uni);
  }
dispose:
  for (int i = 0; i < argc - optind; ++i) {
    bst_dispose(&tab[i]);
  }
  holdall_apply(words, rfree);
  holdall_dispose(&words);
  free(tab);
  return EXIT_SUCCESS;
}
