#include "mk_strdup.h"

#include "mk_malloc.h"

#include <string.h>

char* mk_strdup(struct mk_state* state, const char* str) {

  size_t str_size = strlen(str);

  char* str_copy = mk_malloc(state, str_size + 1);
  if (!str_copy) {
    return NULL;
  }

  return memcpy(str_copy, str, str_size + 1);
}
