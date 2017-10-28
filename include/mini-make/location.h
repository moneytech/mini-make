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

#ifndef MINI_MAKE_LOCATION_H
#define MINI_MAKE_LOCATION_H

#include <mini-make/string.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup make-location Make Location
 * @brief A location within a makefile.
 * */

/** @brief A location within a makefile.
 * A location within a makefile. Used for
 * indicating the location of a error or a
 * node within a makefile.
 * @ingroup make-location
 * */
struct make_location {
  /** @brief The name of the file. */
  struct make_string path;
  /** @brief The line number within the
   * file. The first line in the file is
   * equal to one. */
  unsigned long int line;
  /** @brief The column number within the
   * line. The first column in the line is
   * equal to one. */
  unsigned long int column;
};

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_LOCATION_H */
