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

#ifndef MINI_MAKE_TABLE_H
#define MINI_MAKE_TABLE_H

#include <mini-make/string.h>

#ifdef __cplusplus
extern "C" {
#endif

struct make_assignment_stmt;
struct make_var;

struct make_table {
  struct make_string target;
  struct make_string first_prerequisite;
  struct make_string all_prerequisites;
  struct make_var *entry_array;
  unsigned long int entry_count;
};

void make_table_init(struct make_table *table);

void make_table_free(struct make_table *table);

int make_table_set_target(struct make_table *table,
                          const struct make_string *target);

int make_table_set_first_prerequisite(struct make_table *table,
                                      const struct make_string *first_preq);

int make_table_define(struct make_table *table,
                      const struct make_string *key,
                      const struct make_string *value);

int make_table_evaluate(struct make_table *table,
                        const struct make_string *in,
                        struct make_string *out);

int make_table_update(struct make_table *table,
    const struct make_assignment_stmt *assignment_stmt);

int make_table_value_of(const struct make_table *table,
                        const struct make_string *key,
                        struct make_string *value);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_TABLE_H */
