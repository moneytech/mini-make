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

#include <stdio.h>
#include <stdlib.h>

#include <mini-make/error.h>
#include <mini-make/parser.h>

static int on_target(void *data, const struct make_string *target) {
  (void) data;
  (void) target;
  fprintf(stdout, "Found target: %.*s\n", (int) target->size, target->data);
  return make_success;
}

static int list_targets_of(const char *path) {

  int err;
  struct make_parser parser;

  make_parser_init(&parser);

  parser.hooks.on_target = on_target;

  err = make_parser_read(&parser, path);
  if (err) {
    fprintf(stderr, "Failed to read '%s'\n", path);
    make_parser_free(&parser);
    return EXIT_FAILURE;
  }

  err = make_parser_run(&parser);
  if (err) {
    fprintf(stderr, "Failed to parse '%s'\n", path);
    make_parser_free(&parser);
    return EXIT_FAILURE;
  }

  make_parser_free(&parser);

  return 0;
}

int main(int argc, const char **argv) {

  int i;
  int err;

  if (argc == 1) {
    fprintf(stderr, "No files specified.\n");
    return EXIT_FAILURE;
  }

  for (i = 1; i < argc; i++) {
    err = list_targets_of(argv[i]);
    if (err)
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

