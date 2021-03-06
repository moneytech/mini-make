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

#include <mini-make/parser.h>

#include <mini-make/assignment-stmt.h>
#include <mini-make/command.h>
#include <mini-make/include-stmt.h>
#include <mini-make/listener.h>
#include <mini-make/location.h>
#include <mini-make/string.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_data {
  const char *makefile_path;
  unsigned long int targets_found;
  unsigned long int prerequisites_found;
  unsigned long int commands_found;
  unsigned long int includes_found;
  unsigned long int assignments_found;
  unsigned long int missing_separator_found;
};

static int on_target(void *user_data, const struct make_string *target) {

  struct test_data *test_data;

  printf("Found target: %.*s\n",
         (int) target->size,
         target->data);

  test_data = (struct test_data *)(user_data);

  assert(test_data->targets_found < 4);

  if (test_data->targets_found == 0) {
    assert(target->size == 2);
    assert(memcmp(target->data, "t1", 2) == 0);
  } else if (test_data->targets_found == 1) {
    assert(target->size == 2);
    assert(memcmp(target->data, "t2", 2) == 0);
  } else if (test_data->targets_found == 2) {
    assert(target->size == 2);
    assert(memcmp(target->data, "t3", 2) == 0);
  } else if (test_data->targets_found == 3) {
    assert(target->size == 2);
    assert(memcmp(target->data, "t4", 2) == 0);
  }

  test_data->targets_found++;

  return 0;
}

static int on_prerequisite(void *user_data, const struct make_string *prerequisite) {

  struct test_data *test_data;

  printf("Found prerequisite: %.*s\n",
         (int) prerequisite->size,
         prerequisite->data);

  test_data = (struct test_data *)(user_data);

  assert(test_data->prerequisites_found < 2);

  if (test_data->prerequisites_found == 0) {
    assert(prerequisite->size == 2);
    assert(memcmp(prerequisite->data, "p1", 2) == 0);
  } else if (test_data->prerequisites_found == 1) {
    assert(prerequisite->size == 2);
    assert(memcmp(prerequisite->data, "p2", 2) == 0);
  }

  test_data->prerequisites_found++;

  return 0;
}

static int on_command(void *user_data, const struct make_command *command) {

  struct test_data *test_data;

  printf("Found command: %.*s\n",
         (int) command->source->size,
         command->source->data);

  test_data = (struct test_data *) user_data;

  if (test_data->commands_found == 0) {
    assert(command->ignore_error == 0);
    assert(command->silent == 0);
    assert(command->source->size == 4);
    assert(memcmp(command->source->data, "cmd1", 4) == 0);
  } else if (test_data->commands_found == 1) {
    assert(command->ignore_error == 1);
    assert(command->silent == 0);
    assert(command->source->size == 4);
    assert(memcmp(command->source->data, "cmd2", 4) == 0);
  } else if (test_data->commands_found == 2) {
    assert(command->ignore_error == 0);
    assert(command->silent == 1);
    assert(command->source->size == 4);
    assert(memcmp(command->source->data, "cmd3", 4) == 0);
  } else if (test_data->commands_found == 3) {
    assert(command->ignore_error == 1);
    assert(command->silent == 1);
    assert(command->source->size == 4);
    assert(memcmp(command->source->data, "cmd4", 4) == 0);
  } else if (test_data->commands_found == 4) {
    assert(command->ignore_error == 1);
    assert(command->silent == 1);
    assert(command->source->size == 4);
    assert(memcmp(command->source->data, "cmd5", 4) == 0);
  } else if (test_data->commands_found == 5) {
    assert(command->ignore_error == 0);
    assert(command->silent == 0);
    assert(command->source->size == 4);
    assert(memcmp(command->source->data, "cmd6", 4) == 0);
  }

  test_data->commands_found++;

  return 0;
}

static int on_include_stmt(void *user_data, const struct make_include_stmt *include_stmt) {

  struct test_data *test_data;

  printf("Found include statement: %.*s\n",
         (int) include_stmt->path->size,
         include_stmt->path->data);

  test_data = (struct test_data *) user_data;

  if (test_data->includes_found == 0) {
    assert(include_stmt->ignore_error == 0);
    assert(include_stmt->path->size == (sizeof("rules.mk") - 1));
    assert(memcmp(include_stmt->path->data, "rules.mk", sizeof("rules.mk") - 1) == 0);
  } else if (test_data->includes_found == 1) {
    assert(include_stmt->ignore_error == 0);
    assert(include_stmt->path->size == (sizeof("vars.mk") - 1));
    assert(memcmp(include_stmt->path->data, "vars.mk", sizeof("vars.mk") - 1) == 0);
  } else if (test_data->includes_found == 2) {
    assert(include_stmt->ignore_error == 1);
    assert(include_stmt->path->size == (sizeof("overrides.mk") - 1));
    assert(memcmp(include_stmt->path->data, "overrides.mk", sizeof("overrides.mk") - 1) == 0);

  }

  test_data->includes_found++;

  return 0;
}

static int on_assignment_stmt(void *user_data, const struct make_assignment_stmt *assignment_stmt) {

  struct test_data *test_data;

  printf("Found an assignment statement\n");
  printf("  Key: %.*s\n",
         (int) assignment_stmt->key->size,
         assignment_stmt->key->data);
  printf("  Value: %.*s\n",
         (int) assignment_stmt->value->size,
         assignment_stmt->value->data);

  test_data = (struct test_data *) user_data;

  assert(test_data->assignments_found < 4);

  if (test_data->assignments_found == 0) {
    assert(assignment_stmt->key->size == 2);
    assert(memcmp(assignment_stmt->key->data, "k1", 2) == 0);
    assert(assignment_stmt->value->size == 2);
    assert(memcmp(assignment_stmt->value->data, "v1", 2) == 0);
    assert(assignment_stmt->operation == MINI_MAKE_OPERATION_APPEND);
  } else if (test_data->assignments_found == 1) {
    assert(assignment_stmt->key->size == 2);
    assert(memcmp(assignment_stmt->key->data, "k2", 2) == 0);
    assert(assignment_stmt->value->size == 2);
    assert(memcmp(assignment_stmt->value->data, "v2", 2) == 0);
    assert(assignment_stmt->operation == MINI_MAKE_OPERATION_CONDITIONAL);
  } else if (test_data->assignments_found == 2) {
    assert(assignment_stmt->key->size == 2);
    assert(memcmp(assignment_stmt->key->data, "k3", 2) == 0);
    assert(assignment_stmt->value->size == 2);
    assert(memcmp(assignment_stmt->value->data, "v3", 2) == 0);
    assert(assignment_stmt->operation == MINI_MAKE_OPERATION_RECURSIVE);
  } else if (test_data->assignments_found == 3) {
    assert(assignment_stmt->key->size == 2);
    assert(memcmp(assignment_stmt->key->data, "k4", 2) == 0);
    assert(assignment_stmt->value->size == 8);
    assert(memcmp(assignment_stmt->value->data, "v4 \\\n\tv5", 8) == 0);
    assert(assignment_stmt->operation == MINI_MAKE_OPERATION_STATIC);
  }

  test_data->assignments_found++;

  return 0;
}

static int on_rule_start(void *user_data) {
  (void) user_data;
  return 0;
}

static int on_rule_finish(void *user_data) {
  (void) user_data;
  return 0;
}

static void on_missing_separator(void *user_data, const struct make_location *location) {

  struct test_data *test_data;

  test_data = (struct test_data *) user_data;

  if (test_data->missing_separator_found == 0) {
    assert(location->line == 25);
    assert(location->column == 3);
    assert(location->path.size == strlen(test_data->makefile_path));
    assert(strcmp(location->path.data, test_data->makefile_path) == 0);
  }

  test_data->missing_separator_found++;

  (void) location;
  (void) user_data;
}

int main(int argc, const char **argv) {

  int err;
  struct test_data test_data;
  struct make_parser parser;
  const char *makefile_path;
  int i;

  makefile_path = NULL;

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--makefile") == 0) {
      makefile_path = argv[i + 1];
      i++;
    } else if (strcmp(argv[i], "--help") == 0) {
      fprintf(stdout, "Options:\n");
      fprintf(stdout, "  --makefile <FILE> : Specify the path of the makefile used for the test.\n");
    } else {
      fprintf(stderr, "Unknown argument: '%s'\n", argv[i]);
      return EXIT_FAILURE;
    }
  }

  if (makefile_path == NULL)
    makefile_path = "../test.mk";

  test_data.makefile_path = makefile_path;
  test_data.targets_found = 0;
  test_data.prerequisites_found = 0;
  test_data.commands_found = 0;
  test_data.includes_found = 0;
  test_data.assignments_found = 0;
  test_data.missing_separator_found = 0;

  make_parser_init(&parser);

  err = make_parser_read(&parser, makefile_path);
  if (err < 0) {
    fprintf(stderr, "Failed to read '%s'\n", makefile_path);
    make_parser_free(&parser);
    return EXIT_FAILURE;
  }

  parser.listener.user_data = &test_data;
  parser.listener.on_target = on_target;
  parser.listener.on_prerequisite = on_prerequisite;
  parser.listener.on_command = on_command;
  parser.listener.on_include_stmt = on_include_stmt;
  parser.listener.on_assignment_stmt = on_assignment_stmt;
  parser.listener.on_rule_start = on_rule_start;
  parser.listener.on_rule_finish = on_rule_finish;
  parser.listener.on_missing_separator = on_missing_separator;

  err = make_parser_run(&parser);
  assert(err == -EINVAL);

  make_parser_free(&parser);

  assert(test_data.targets_found == 4);
  assert(test_data.prerequisites_found == 2);
  assert(test_data.commands_found == 6);
  assert(test_data.includes_found == 3);
  assert(test_data.assignments_found == 4);
  assert(test_data.missing_separator_found == 1);

  return EXIT_SUCCESS;
}

