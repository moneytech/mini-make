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

#ifndef MINI_MAKE_ASSIGNMENT_STMT_H
#define MINI_MAKE_ASSIGNMENT_STMT_H

#include "operation.h"

#ifdef __cplusplus
extern "C" {
#endif

struct make_string;

/** @defgroup make-assignment-stmt Make Assignment Statement
 * @brief Assignment statements that appear within makefiles.
 * */

/** @brief An assignment statement.
 * @ingroup make-assignment-stmt
 * */
struct make_assignment_stmt {
  /** @brief The operation of the assignment statement. */
  enum make_operation operation;
  /** @brief The key of the variable to the assign
   * the new value to. */
  struct make_string *key;
  /** @brief The new value to assign to the variable. */
  struct make_string *value;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MINI_MAKE_ASSIGNMENT_STMT_H */

