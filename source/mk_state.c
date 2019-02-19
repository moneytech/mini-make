#include "mk_state.h"

#include "mk_malloc.h"
#include "mk_strdup.h"

#include <string.h>

struct mk_var {
  char* key;
  char* val;
};

struct mk_symtab {
  struct mk_var* var_array;
  size_t var_count;
};

struct mk_state {
  struct mk_symtab symtab;
};

struct mk_state* mk_state_create() {

  struct mk_state* state = mk_malloc(NULL, sizeof(struct mk_state));
  if (!state) {
    return NULL;
  }

  state->symtab.var_array = NULL;
  state->symtab.var_count = 0;

  return state;
}

void mk_state_destroy(struct mk_state* state) {

  if (!state) {
    return;
  }

  for (size_t i = 0; i < state->symtab.var_count; i++) {
    mk_free(state, state->symtab.var_array[i].key);
    mk_free(state, state->symtab.var_array[i].val);
  }

  mk_free(state, state->symtab.var_array);

  mk_free(NULL, state);
}

int mk_state_define(struct mk_state* state,
                    const char* name,
                    const char* value) {

  struct mk_symtab* symtab = &state->symtab;

  struct mk_var* var_array = symtab->var_array;

  var_array = mk_realloc(state, var_array, (symtab->var_count + 1) * sizeof(struct mk_var));
  if (!var_array) {
    return -1;
  }

  symtab->var_array = var_array;

  struct mk_var* var = &symtab->var_array[symtab->var_count];

  var->key = mk_strdup(state, name);
  if (!var->key) {
    return -1;
  }

  var->val = mk_strdup(state, value);
  if (!var->val) {
    mk_free(state, var->key);
    return -1;
  }

  symtab->var_count++;

  return 0;
}

const char* mk_state_find(const struct mk_state* state, const char* name) {

  const struct mk_symtab* symtab = &state->symtab;

  for (size_t i = 0; i < symtab->var_count; i++) {
    const struct mk_var* var = &symtab->var_array[i];
    if (strcmp(var->key, name) == 0) {
      return var->val;
    }
  }

  return NULL;
}
