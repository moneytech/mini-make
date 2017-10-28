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

/** @file buffer.h */

/** @defgroup make-buffer Make Buffer
 * @brief Used for storing elements of a resizeable
 * array.
 * */

#ifndef MINI_MAKE_BUFFER_H
#define MINI_MAKE_BUFFER_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief A general purpose buffer.
 * @ingroup make-buffer
 * */
struct make_buffer {
  /** @brief The address of the data
   * in the buffer. */
  void *data;
  /** @brief The number of elements
   * in the buffer. */
  size_t size;
  /** @brief The size of each element
   * in the buffer. This is equal to
   * one by default. */
  size_t element_size;
};

/** @brief Initializes the buffer.
 * @ingroup make-buffer
 * */
void make_buffer_init(struct make_buffer *buffer);

/** @brief Releases resources allocated by the buffer.
 * @ingroup make-buffer
 * */
void make_buffer_free(struct make_buffer *buffer);

/** @brief Gets the pointer of an element at a
 * specific index.
 *@ingroup make-buffer
 * */
void *make_buffer_get_element(struct make_buffer *buffer,
                              size_t element_index);

/** @brief Sets element data at a specific index.
 * @ingroup make-buffer
 * */
void make_buffer_set_element(struct make_buffer *buffer,
                             size_t element_index,
                             const void *element_data);

/** @brief Sets the size of each element in the buffer.
 * This function does not handle the case where elements
 * of a different size are already in the buffer. This
 * function is meant to be called before the buffer is
 * filled.
 * @param buffer An initialized buffer.
 * @param element_size The size of the elements within
 * the buffer. If the element is a structure, using the
 * sizeof operator would be appropriate.
 * @ingroup make-buffer
 * */
void make_buffer_set_element_size(struct make_buffer *buffer,
                                  size_t element_size);

/** @brief Resizes the buffer.
 * @ingroup make-buffer
 * */
int make_buffer_resize(struct make_buffer *buffer,
                       size_t size);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_BUFFER_H */
