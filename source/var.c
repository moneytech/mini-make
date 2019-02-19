/* Copyright (C) 2017 Taylor Holberton
 *
 * This file is part of Mini Make.
 *
 * Mini Make is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mini Make is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mini Make.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <mini-make/var.h>

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

