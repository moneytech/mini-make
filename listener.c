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

#include <make/listener.h>

#include <make/location.h>
#include <make/string.h>

#include <stdio.h>

static void on_unexpected_char(void *data, char c,
                               const struct make_location *location) {
  (void) data;
  fprintf(stderr, "%.*s:%lu:%lu: Unexpected character '%c'\n",
          (int) location->path.size, location->path.data,
          location->line, location->column, c);
}

void make_listener_init(struct make_listener *listener) {
  listener->on_unexpected_char = on_unexpected_char;
}

