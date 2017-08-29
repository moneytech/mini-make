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

#ifndef MAKE_OPTIONS_H
#define MAKE_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

struct make_options {
  char *working_dir;
  char *filename;
  unsigned int jobs;
  int just_print;
  int silent;
};

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MAKE_OPTIONS_H */
