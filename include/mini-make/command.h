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

#ifndef MINI_MAKE_COMMAND_H
#define MINI_MAKE_COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

struct make_string;

/** @defgroup make-command Make Command
 * @brief A command that appears in a makefile rule.
 * */

/** @brief A command in a makefile rule.
 * @ingroup make-command
 * */
struct make_command {
  /** @brief If non-zero, the command should not
   * be printed when it is executed. */
  int silent;
  /** @brief If non-zero, the command should not
   * generate an error if it fails. */
  int ignore_error;
  /** @brief The command string. */
  struct make_string *source;
};

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_COMMAND_H */
