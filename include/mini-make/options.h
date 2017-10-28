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

#ifndef MINI_MAKE_OPTIONS_H
#define MINI_MAKE_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup make-options Make Options
 * @brief Options passed to the make program.
 * */

/** @brief Options passed to the make program.
 * @ingroup make-options
 * */
struct make_options {
  /** @brief The directory that the makefile will
   * be executed in.*/
  char *working_dir;
  /** @brief The name of the makefile to read. */
  char *filename;
  /** @brief The maximum number of jobs to have
   * running at once. */
  unsigned int jobs;
  /** @brief Set to non-zero if the commands should
   * just be printed and not executed. */
  int just_print;
  /** @brief Set to non-zero if the commands should
   * not be echoed as they are executed. */
  int silent;
};

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_OPTIONS_H */
