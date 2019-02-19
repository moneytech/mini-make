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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void test_resize(void) {

  struct make_buffer buffer;

  make_buffer_init(&buffer);

  int err = make_buffer_resize(&buffer, 3);
  assert(err == make_success);

  char element;

  element = 'a';
  make_buffer_set_element(&buffer, 0, &element);

  element = 'b';
  make_buffer_set_element(&buffer, 1, &element);

  element = 'c';
  make_buffer_set_element(&buffer, 2, &element);

  assert(buffer.size == 3);
  assert(memcmp(buffer.data, "abc", 3) == 0);

  make_buffer_free(&buffer);
}

int main(void) {
  test_resize();
  return EXIT_SUCCESS;
}
