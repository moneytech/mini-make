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

#include <mini-make/target.h>

#include <mini-make/error.h>

void make_target_init(struct make_target *target) {
  make_string_init(&target->path);
}

void make_target_free(struct make_target *target) {
  make_string_free(&target->path);
}

const struct make_string *make_target_get_path(const struct make_target *target) {
  return &target->path;
}

int make_string_set_path(struct make_target *target, const struct make_string *path) {

  int err;

  err = make_string_copy(path, &target->path);
  if (err != make_success)
    return err;

  return make_success;
}
