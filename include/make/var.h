#ifndef MAKE_VAR_H
#define MAKE_VAR_H

#include <make/string.h>

struct make_var {
  struct make_string key;
  struct make_string value;
};

void make_var_init(struct make_var *var);

void make_var_free(struct make_var *var);

int make_var_set_key(struct make_var *var,
                     const struct make_string *key);

int make_var_append_value(struct make_var *var,
                          const struct make_string *value);

int make_var_set_value(struct make_var *var,
                       const struct make_string *value);

#endif /* MAKE_VAR_H */
