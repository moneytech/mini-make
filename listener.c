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

#include <mini-make/listener.h>

#include <mini-make/error.h>
#include <mini-make/location.h>
#include <mini-make/string.h>

#include <stdio.h>
#include <string.h>

static void on_unexpected_char(void *data, char c,
                               const struct make_location *location) {
  (void) data;
  fprintf(stderr, "%.*s:%lu:%lu: Unexpected character '%c'\n",
          (int) location->path.size, location->path.data,
          location->line, location->column, c);
}

void make_listener_init(struct make_listener *listener) {
  memset(listener, 0, sizeof(*listener));
  listener->on_unexpected_char = on_unexpected_char;
}

int make_listener_notify_rule_start(struct make_listener *listener) {
  if (listener->on_rule_start != NULL)
    return listener->on_rule_start(listener->user_data);
  else
    return make_success;
}

int make_listener_notify_rule_finish(struct make_listener *listener) {
  if (listener->on_rule_finish)
    return listener->on_rule_finish(listener->user_data);
  else
    return make_success;
}

