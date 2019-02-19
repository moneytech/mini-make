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

#include <mini-make/interpreter.h>

#include <mini-make/chdir.h>
#include <mini-make/error.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TESTING_DIR
#define TESTING_DIR "testing"
#endif

struct test_data {
  int targets_found;
};

static int on_target(void *data_ptr, const struct make_target *target) {

  struct test_data *data;

  data = (struct test_data *) data_ptr;

  if (data->targets_found == 0) {
    /* t1 */
    assert(target->path.size == 2);
    assert(memcmp(target->path.data, "t1", 2) == 0);
  } else if (data->targets_found == 1) {
    /* p1 */
    assert(target->path.size == 2);
    assert(memcmp(target->path.data, "p1", 2) == 0);
  } else if (data->targets_found == 2) {
    /* t1 */
    assert(target->path.size == 2);
    assert(memcmp(target->path.data, "t1", 2) == 0);
  } else if (data->targets_found == 3) {
    /* p2 */
    assert(target->path.size == 2);
    printf("%.*s\n", (int) target->path.size, target->path.data);
    assert(memcmp(target->path.data, "p2", 2) == 0);
  } else if (data->targets_found == 4) {
    /* t1 */
    assert(target->path.size == 2);
    assert(memcmp(target->path.data, "t1", 2) == 0);
  } else {
    return make_failure;
  }

  data->targets_found++;

  return make_success;
}

int main(void) {

  int err;
  const char *makefile_name = "Makefile";
  const char *makefile_dir = TESTING_DIR "/makefiles/test2";
  struct test_data data;
  struct make_ihooks hooks;
  struct make_interpreter interpreter;

  err = make_chdir(makefile_dir);
  if (err) {
    fprintf(stderr, "Failed to change directory to '%s': %s\n", makefile_dir, strerror(-err));
    return EXIT_FAILURE;
  }

  data.targets_found = 0;

  make_ihooks_init(&hooks);
  hooks.data = &data;
  hooks.on_target = on_target;

  make_interpreter_init(&interpreter);

  interpreter.just_print = 1;

  make_interpreter_set_hooks(&interpreter, &hooks);

  err = make_interpreter_read(&interpreter, makefile_name);
  assert(err == make_success);

  err = make_interpreter_run(&interpreter);
  assert(err == make_success);

  assert(data.targets_found == 5);

  make_interpreter_free(&interpreter);

  return EXIT_SUCCESS;
}

