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

#ifndef MINI_MAKE_INCLUDE_STMT_H
#define MINI_MAKE_INCLUDE_STMT_H

#ifdef __cplusplus
extern "C" {
#endif

struct make_string;

/** @defgroup make-include-stmt Make Include Statement
 * @brief An include statement within a makefile.
 * */

/** @brief An include statement within a makefile.
 * @ingroup make-include-stmt
 * */
struct make_include_stmt {
  /** @brief If non-zero, the include statement should
   * not generate an error if the file does not exist. */
  int ignore_error;
  /** @brief The path to the file in the include statement. */
  struct make_string *path;
};

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_INCLUDE_STMT_H */
