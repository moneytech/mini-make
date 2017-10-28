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

#include <mini-make/buffer.h>

#include <mini-make/error.h>

void make_buffer_init(struct make_buffer *buffer) {
  buffer->data = NULL;
  buffer->size = 0;
  buffer->element_size = 1;
}

void make_buffer_free(struct make_buffer *buffer) {

  if (buffer->data != NULL)
    free(buffer->data);

  buffer->data = NULL;
  buffer->size = 0;
}

int make_buffer_resize(struct make_buffer *buffer,
                       size_t size) {
  void *tmp;

  tmp = realloc(buffer->data, size * buffer->element_size);
  if (tmp == NULL)
    return make_failure;

  buffer->data = tmp;
  buffer->size = size;

  return make_success;
}

