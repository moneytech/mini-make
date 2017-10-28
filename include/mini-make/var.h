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

/** @file */

#ifndef MINI_MAKE_VAR_H
#define MINI_MAKE_VAR_H

#include <mini-make/string.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup make-variable Make Variable
 * @brief A makefile variable.
 * */

/** @brief A makefile variable.
 * Contains a key and a value.
 * @ingroup make-variable
 * */
struct make_var {
  /** @brief The key to the variable. */
  struct make_string key;
  /** @brief The value of the variable. */
  struct make_string value;
};

/** @brief Initializes a variable.
 * @ingroup make-variable
 * */
void make_var_init(struct make_var *var);

/** @brief Releases all resources allocated
 * by the variable.
 * @ingroup make-variable
 * */
void make_var_free(struct make_var *var);

/** @brief Sets the key to the variable.
 * @ingroup make-variable
 * */
int make_var_set_key(struct make_var *var,
                     const struct make_string *key);

/** @brief Appends content to the value of the variable.
 * @ingroup make-variable
 * */
int make_var_append_value(struct make_var *var,
                          const struct make_string *value);

/** @brief Sets the value of a variable.
 * @ingroup make-variable
 * */
int make_var_set_value(struct make_var *var,
                       const struct make_string *value);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_VAR_H */
