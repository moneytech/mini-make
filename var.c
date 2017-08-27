#include <make/var.h>

void make_var_init(struct make_var *var) {
  make_string_init(&var->key);
  make_string_free(&var->value);
}

void make_var_free(struct make_var *var) {
  make_string_free(&var->key);
  make_string_free(&var->value);
}

int make_var_set_key(struct make_var *var,
                     const struct make_string *key) {

  int err;

  make_string_free(&var->key);

  err = make_string_copy(key, &var->key);
  if (err)
    return err;

  return 0;
}

int make_var_append_value(struct make_var *var,
                          const struct make_string *value) {

  int err;

  err = make_string_append(&var->value, value);
  if (err)
    return err;

  return 0;
}

int make_var_set_value(struct make_var *var,
                       const struct make_string *value) {

  int err;

  make_string_free(&var->value);

  err = make_string_copy(value, &var->value);
  if (err)
    return err;

  return 0;
}

