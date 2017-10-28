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

#ifndef MINI_MAKE_TABLE_H
#define MINI_MAKE_TABLE_H

#include <mini-make/string.h>

#ifdef __cplusplus
extern "C" {
#endif

struct make_assignment_stmt;
struct make_var;

/** @defgroup make-table Make Table
 * @brief A key-value table for storing makefile
 * variables.
 * */

/** A key-value table for storing makefile variables.
 * @ingroup make-table
 * */
struct make_table {
  /** @brief The current target being built
   * by the interpreter. */
  struct make_string target;
  /** @brief The first prerequisite of the
   * rule being executed. */
  struct make_string first_prerequisite;
  /** @brief All the prerequisites of the
   * rule being executed. */
  struct make_string all_prerequisites;
  /** @brief An array of all non-special
   * and non-implicit variables. */
  struct make_var *entry_array;
  /** @brief The number of entries in the
   * entry array. */
  unsigned long int entry_count;
};

/** @brief Initializes the table for use.
 * @ingroup make-table
 * */
void make_table_init(struct make_table *table);

/** @brief Releases all resources allocated
 * by the table.
 * @ingroup make-table
 * */
void make_table_free(struct make_table *table);

/** @brief Sets the 'current target' variable. This
 * should be the target that is currently being built.
 * @ingroup make-table
 * */
int make_table_set_target(struct make_table *table,
                          const struct make_string *target);

/** @brief Sets the 'first prerequisite' variable. This
 * should be the first prerequisite of the rule that is
 * currently being executed.
 * @ingroup make-table
 * */
int make_table_set_first_prerequisite(struct make_table *table,
                                      const struct make_string *first_preq);

/** @brief Defines a new variable.
 * @ingroup make-table
 * */
int make_table_define(struct make_table *table,
                      const struct make_string *key,
                      const struct make_string *value);

/** @brief Evaluates a string, resolving all variable
 * references.
 * @ingroup make-table
 * */
int make_table_evaluate(struct make_table *table,
                        const struct make_string *in,
                        struct make_string *out);

/** @brief Updates the table based on the assignment
 * statement.
 * @ingroup make-table
 * */
int make_table_update(struct make_table *table,
                      const struct make_assignment_stmt *assignment_stmt);

/** @brief Gets the value of a variable by its key.
 * @ingroup make-table
 * */
int make_table_value_of(const struct make_table *table,
                        const struct make_string *key,
                        struct make_string *value);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_TABLE_H */
