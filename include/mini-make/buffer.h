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

#ifndef MINI_MAKE_BUFFER_H
#define MINI_MAKE_BUFFER_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief A general purpose buffer. */
struct make_buffer {
  /** @brief The address of the data
   * in the buffer. */
  void *data;
  /** @brief The size of the buffer. */
  size_t size;
};

/** @brief Initializes the buffer. */
void make_buffer_init(struct make_buffer *buffer);

/** @brief Releases resources allocated by the buffer. */
void make_buffer_free(struct make_buffer *buffer);

/** @brief Resizes the buffer. */
int make_buffer_resize(struct make_buffer *buffer,
                       size_t size);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_BUFFER_H */
