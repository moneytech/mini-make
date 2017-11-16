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

#include <windows.h>

#include "buffer-io.h"

#include <mini-make/error.h>

int make_buffer_send(HANDLE handle, const struct make_buffer *buffer) {
  (void) handle;
  (void) buffer;
  return make_failure;
}

int make_buffer_recv(HANDLE handle, struct make_buffer *buffer) {
  (void) handle;
  (void) buffer;
  return make_failure;
}

