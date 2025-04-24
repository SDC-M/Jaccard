#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <locale.h>
#include <limits.h>

#include "avl/bst.h"
#include "jdis/jdis.h"

#define LEN_LONG_OPT_INITIAL 10
#define LEN_SHORT_OPT_INITIAL 2
#define WORD_INIT_SIZE 63

#define LONG_G "--graph"
#define LONG_H "--help"
#define LONG_I "--initial="
#define LONG_P "--punctuation-like-space"
#define ESCAPE_FILE "--"
#define STDIN "-"

#define SHORT_G "-g"
#define SHORT_H "-h"
#define SHORT_I "-i"
#define SHORT_P "-p"

typedef struct context context;

struct context {
  bst **t;
  int nb_bst;
};

void help(void) {
  fprintf(stdout,
      "Utilisation : ./jdis [OPTIONS]... FICHIER1 FICHIER2 ...\n\n");
  fprintf(stdout,
      "Liste des opérations réalisables :\n\n");
  fprintf(stdout,
      LONG_H " / " SHORT_H " : Affiche ce menu d'aide.\n");
  fprintf(stdout,
      LONG_G " / " SHORT_G
      ": Affiche le graphe d'appartenance des mots."
      "dans les fichiers passés sur la ligne de commande.\n");
  fprintf(stdout,
      LONG_I "/" SHORT_I
      " : permet de fixer la longueur maximale."
      "des mots à VALUE.\n");
  fprintf(stdout,
      LONG_P " / " SHORT_P
      " : Considère les caractères de ponctuation."
      "comme des caractères d'espacement.\n");
  fprintf(stdout,
      ESCAPE_FILE
      " : Indique que l'argument qui suit doit être "
      "considéré comme un fichier.\n");
  fprintf(stdout,
      STDIN
      " : À la place d'un nom de fichier indique qu'on prend en "
      "concidération l'entrée standart.\n");
}

static int scptr_display(context *ctx, const char *ref) {
  printf("%s\t", ref);
  for (int i = 0; i < ctx->nb_bst; ++i) {
    if (bst_search(ctx->t[i], ref) != nullptr) {
      fprintf(stdout, "x\t");
    } else {
      fprintf(stdout, "-\t");
    }
  }
  printf("\n");
  return 0;
}

int graph_belonging(bst **t, bst *uni, int nb_file) {
  context *ctx = malloc(sizeof *ctx);
  if (ctx == nullptr) {
    return -1;
  }
  ctx->nb_bst = nb_file;
  ctx->t = t;
  bst_dft_infix_apply_context(uni, 1, ctx,
      (int (*)(void *, const void *)) scptr_display, nullptr, nullptr);
  free(ctx);
  return 0;
}

int rfree(void *ctx, const void *ref) {
  if (ctx == nullptr) {
  }
  free((void *) ref);
  return 0;
}

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  bool graph = false;
  bool escaped_file = false;
  size_t value_max = 0;
  bool want_punc = false;
  int nb_files = 0;
  char **filenames = malloc((size_t) argc * sizeof(char *));
  if (filenames == nullptr) {
    printf("*** Erreur d'allocation du tableau des noms de fichiers\n");
    return EXIT_FAILURE;
  }
  for (int i = 1; i < argc; ++i) {
    if (escaped_file) {
      filenames[nb_files] = argv[i];
      escaped_file = false;
      ++nb_files;
    } else if (strcmp(argv[i], SHORT_H) == 0 || strcmp(argv[i], LONG_H) == 0) {
      help();
    } else if (strcmp(argv[i], SHORT_P) == 0 || strcmp(argv[i], LONG_P) == 0) {
      want_punc = true;
    } else if (strcmp(argv[i], SHORT_G) == 0 || strcmp(argv[i], LONG_G) == 0) {
      graph = true;
    } else if (strstr(argv[i], LONG_I) != nullptr) {
      value_max = strtoul(argv[i] + LEN_LONG_OPT_INITIAL, nullptr, 10);
      if (value_max == ULONG_MAX) {
        fprintf(stderr, "*** Too high value ==> changing to default\n");
        value_max = WORD_INIT_SIZE;
      }
    } else if (strstr(argv[i], SHORT_I) != nullptr) {
      value_max = strtoul(argv[i] + LEN_SHORT_OPT_INITIAL, nullptr, 10);
      if (value_max == ULONG_MAX) {
        fprintf(stderr, "*** Too high value ==> changing to default\n");
        value_max = WORD_INIT_SIZE;
      }
    } else if (strcmp(argv[i], ESCAPE_FILE) == 0) {
      escaped_file = true;
    } else {
      filenames[nb_files] = argv[i];
      ++nb_files;
    }
  }
  if (nb_files < 2) {
    fprintf(stderr, "*** Pas assez d'arguments.\n");
    help();
    free(filenames);
    return EXIT_FAILURE;
  }
  bst **tab = malloc((size_t) (nb_files) * sizeof(bst *));
  if (tab == nullptr) {
    goto dispose;
  }
  bst *uni_words = bst_empty((int (*)(const void *, const void *)) strcoll);
  if (uni_words == nullptr) {
    fprintf(stderr, "*** Erreur d'allocation sur l'union\n");
    goto dispose2;
  }
  for (int i = 0; i < nb_files; ++i) {
    tab[i] = file_to_bst(filenames[i], value_max, want_punc, uni_words);
    if (tab[i] == nullptr) {
      fprintf(stderr, "*** Erreur de l'allocation pour le fichier : %s\n",
          filenames[i]);
      for (int j = 0; j < i; ++j) {
        bst_dispose(&tab[j]);
      }
      free(tab);
      return EXIT_FAILURE;
    }
  }
  if (graph) {
    for (int i = 0; i < nb_files; ++i) {
      printf("\t%s", filenames[i]);
    }
    printf("\n");
    graph_belonging(tab, uni_words, nb_files);
  } else {
    for (int i = 0; i < nb_files - 1; ++i) {
      for (int j = i + 1; j < nb_files; ++j) {
        size_t card_in = card_intersection(tab[i], tab[j]);
        double jaccard_distance = jdis(tab[i], tab[j], card_in);
        printf("%s\t%s: %.4lf\n", filenames[i], filenames[j], jaccard_distance);
      }
    }
  }
dispose2:
  for (int i = 0; i < nb_files; ++i) {
    bst_dispose(&tab[i]);
  }
  bst_dft_infix_apply_context(uni_words, 0, nullptr, rfree, nullptr,
      nullptr);
  bst_dispose(&uni_words);
dispose:
  free(filenames);
  free(tab);
  return EXIT_SUCCESS;
}
