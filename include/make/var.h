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

#ifndef MAKE_VAR_H
#define MAKE_VAR_H

#include <make/string.h>

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MAKE_VAR_H */
