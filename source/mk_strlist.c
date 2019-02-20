#include "mk_strlist.h"

#include <stdlib.h>

struct mk_strlist {
  char** str_array;
  size_t str_count;
  size_t str_max;
};

struct mk_strlist* mk_strlist_create() {

  struct mk_strlist* strlist = malloc(sizeof(*strlist));
  if (!strlist) {
    return NULL;
  }

  strlist->str_array = NULL;
  strlist->str_count = 0;
  strlist->str_max = 0;

  return strlist;
}

void mk_strlist_destroy(struct mk_strlist* strlist) {

  for (size_t i = 0; i < strlist->str_count; i++) {
    free(strlist->str_array[i]);
  }

  free(strlist->str_array);

  free(strlist);
}

static int mk_strlist_expand(struct mk_strlist* strlist) {

  size_t str_max = strlist->str_max + 4;

  char** str_array = strlist->str_array;

  str_array = realloc(str_array, str_max * sizeof(char*));
  if (!str_array) {
    return -1;
  }

  strlist->str_array = str_array;
  strlist->str_max = str_max;

  return 0;
}

int mk_strlist_push(struct mk_strlist* strlist, char* str) {

  if (strlist->str_count >= strlist->str_max) {
    int err = mk_strlist_expand(strlist);
    if (!err) {
      return err;
    }
  }

  strlist->str_array[strlist->str_count++] = str;

  return 0;
}
