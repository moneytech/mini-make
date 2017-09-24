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

#ifndef MINI_MAKE_TARGET_H
#define MINI_MAKE_TARGET_H

#include <mini-make/string.h>

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief A makefile target. */
struct make_target {
  /** @brief The path of the target. */
  struct make_string path;
  /** @brief The line in the makefile
   * that this target appeared in. */
  unsigned long int line;
  /** @brief The column in the makefile
   * that this target appeared in. */
  unsigned long int column;
};

/** @brief Initializes the target. */
void make_target_init(struct make_target *target);

/** @brief Releases resource allocated by the target. */
void make_target_free(struct make_target *target);

/** @brief Returns the path of the target. This function
 * never returns NULL, but it may return a string with no content. */
const struct make_string *make_target_get_path(const struct make_target *target);

/** @brief Returns the line of the target. */
unsigned long int make_target_get_line(const struct make_target *target);

/** @brief Returns the column of the target. */
unsigned long int make_target_get_column(const struct make_target *target);

/** @brief Sets the path of the target. */
int make_target_set_path(struct make_target *target, const struct make_string *path);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_TARGET_H */
