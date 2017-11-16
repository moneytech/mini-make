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

#include <mini-make/command.h>
#include <mini-make/error.h>
#include <mini-make/include-stmt.h>
#include <mini-make/phooks.h>
#include <mini-make/location.h>
#include <mini-make/parser.h>
#include <mini-make/string.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#if defined(__unix__)
#include <unistd.h>
#elif defined(_WIN32)
#define stat _stat
#endif

static const struct make_string * get_target(const struct make_interpreter *interpreter) {
  return &interpreter->table.target;
}

static int build_prerequisite(struct make_interpreter *interpreter,
                              const struct make_string *prerequisite) {

  int err;
  const struct make_string *old_target;
  struct make_string target_copy;
  int target_found;
  int target_expired;

  old_target = get_target(interpreter);
  target_found = interpreter->target_found;
  target_expired = interpreter->target_expired;

  make_string_init(&target_copy);

  err = make_string_copy(old_target, &target_copy);
  if (err) {
    make_string_free(&target_copy);
    return err;
  }

  err = make_interpreter_set_target(interpreter, prerequisite);
  if (err) {
    make_string_free(&target_copy);
    return err;
  }

  err = make_interpreter_run(interpreter);
  if (err) {
    make_string_free(&target_copy);
    return err;
  }

  err = make_interpreter_set_target(interpreter, &target_copy);
  if (err) {
    make_string_free(&target_copy);
    return err;
  }

  interpreter->target_found = target_found;
  interpreter->target_expired = target_expired;

  make_string_free(&target_copy);

  return 0;
}

static int on_rule_start(void *data) {

  struct make_interpreter *interpreter;

  interpreter = (struct make_interpreter *) data;
  interpreter->phony_found = 0;
  interpreter->target_found = 0;
  interpreter->target_expired = 0;

  return 0;
}

static int on_rule_finish(void *data) {
  (void) data;
  return 0;
}

static int on_target(void *data, const struct make_string *target) {

  int err;
  struct make_interpreter *interpreter;
  struct make_string phony;
  struct make_string silent;
  struct make_string evaluated_target;

  interpreter = (struct make_interpreter *) data;

  make_string_init(&evaluated_target);

  err = make_table_evaluate(&interpreter->table,
                            target, &evaluated_target);
  if (err) {
    make_string_free(&evaluated_target);
    return err;
  }

  phony.data = ".PHONY";
  phony.size = sizeof(".PHONY") - 1;
  phony.res = 0;
  if (make_string_equal(&phony, &evaluated_target)) {
    make_string_free(&evaluated_target);
    interpreter->phony_found = 1;
    return 0;
  }

  silent.data = ".SILENT";
  silent.size = sizeof(".SILENT") - 1;
  silent.res = 0;
  if (make_string_equal(&silent, &evaluated_target)) {
    make_string_free(&evaluated_target);
    interpreter->silent = 1;
    return 0;
  }

  if (!make_interpreter_has_target(interpreter)) {
    err = make_interpreter_set_target(interpreter, &evaluated_target);
    if (err) {
      make_string_free(&evaluated_target);
      return err;
    }
  }

  if (make_string_equal(&interpreter->target.path, &evaluated_target)) {
    interpreter->target_found = 1;
    interpreter->target_found_once = 1;
  }

  make_string_free(&evaluated_target);

  return 0;
}

static int on_prerequisite(void *data, const struct make_string *prerequisite) {

  int err;
  struct stat prerequisite_stat;
  struct make_interpreter *interpreter;
  struct make_string path;
  const struct make_string *target;

  interpreter = (struct make_interpreter *) data;

  if (interpreter->phony_found) {
    /* If the target is found in a .PHONY rule, then
     * it is always expired. */
    target = get_target(interpreter);
    if (make_string_equal(prerequisite, target)) {
      interpreter->target_expired = 1;
      return 0;
    }
  }

  if (!interpreter->target_found) {
    return 0;
  }

  make_string_init(&path);

  err = make_table_evaluate(&interpreter->table,
                            prerequisite, &path);
  if (err) {
    make_string_free(&path);
    return err;
  }

  err = build_prerequisite(interpreter, &path);
  if (err) {
    make_string_free(&path);
    return err;
  }

  err = stat(path.data, &prerequisite_stat);
  if (err) {
    if (errno == ENOENT) {
      /* Prerequisite does not exist. If it
       * does not exist, that means it's going
       * to be created and newer than the target.
       * So, build the prerequisite and mark the
       * target as expired */
      interpreter->target_expired = 1;
    } else {
      fprintf(interpreter->errlog,
              "Failed to stat '%s': %s\n",
              path.data, strerror(errno));
      make_string_free(&path);
      return -errno;
    }
  } else if (prerequisite_stat.st_mtime > interpreter->target_mtime) {
    /* Prerequisite is newer than the target. */
    interpreter->target_expired = 1;
  }

  make_string_free(&path);

  return 0;
}

static int on_command(void *data, const struct make_command *command_in) {

  int err;
  struct make_command command;
  struct make_string command_str;
  struct make_interpreter *interpreter;

  interpreter = (struct make_interpreter *) data;

  if (!interpreter->target_found)
    /* The target is not in this rule. */
    return 0;
  else if (interpreter->target_exists
        && !interpreter->target_expired)
    /* The target exists and it is up to date. */
    return 0;

  make_string_init(&command_str);

  err = make_table_evaluate(&interpreter->table,
                            command_in->source,
                            &command_str);
  if (err) {
    make_string_free(&command_str);
    return err;
  }

  command = *command_in;
  command.source = &command_str;

  err = make_ihooks_notify_command(&interpreter->hooks, &interpreter->target, &command);
  if (err != make_success) {
    make_string_free(&command_str);
    return err;
  }

  if (!command.silent && !interpreter->silent) {
    fprintf(interpreter->outlog, "%.*s\n",
            (int) command_str.size,
            command_str.data);
  }

  if (!interpreter->just_print) {
    err = make_job_manager_queue(&interpreter->job_manager,
                                 &command_str);
    if (err && !command.ignore_error) {
      fprintf(interpreter->errlog,
              "Recipe failed for target '%.*s'\n",
              (int) interpreter->table.target.size,
              interpreter->table.target.data);
      make_string_free(&command_str);
      return err;
    }
  }

  make_string_free(&command_str);

  return 0;
}

static int on_assignment_stmt(void *data, const struct make_assignment_stmt *assignment_stmt) {

  int err;
  struct make_table *table;
  struct make_interpreter *interpreter;

  interpreter = (struct make_interpreter *) data;

  table = &interpreter->table;

  err = make_table_update(table, assignment_stmt);
  if (err)
    return err;

  return 0;
}

static int on_include_stmt(void *data, const struct make_include_stmt *include_stmt) {

  int err;
  struct make_parser new_parser;
  struct make_string path;
  struct make_interpreter *interpreter;

  interpreter = (struct make_interpreter *) data;

  make_string_init(&path);
  err = make_string_copy(include_stmt->path, &path);
  if (err) {
    make_string_free(&path);
    return err;
  }
  make_parser_init(&new_parser);
  err = make_parser_read(&new_parser, path.data);
  if (err) {
    if (include_stmt->ignore_error) {
      /* This include file may be skipped if it doesn't
       * exist. */
      make_string_free(&path);
      make_parser_free(&new_parser);
      return make_success;
    } else {
      fprintf(interpreter->errlog,
              "Failed to open '%s'\n",
              path.data);
      make_string_free(&path);
      make_parser_free(&new_parser);
      return err;
    }
  }
  make_string_free(&path);

  new_parser.hooks = interpreter->parser.hooks;
  err = make_parser_run(&new_parser);
  if (err) {
    make_parser_free(&new_parser);
    return err;
  }
  make_parser_free(&new_parser);

  return 0;
}

static void on_unexpected_char(void *data, char c, const struct make_location *location) {

  struct make_interpreter *interpreter;

  interpreter = (struct make_interpreter *) data;

  fprintf(interpreter->errlog,
          "%.*s:%lu:%lu: Unexpected character '%c'\n",
          (int) location->path.size, location->path.data,
          location->line, location->column, c);
}

static void on_missing_separator(void *data, const struct make_location *location) {

  struct make_interpreter *interpreter;

  interpreter = (struct make_interpreter *) data;

  fprintf(interpreter->errlog, "%.*s:%lu:%lu: Missing ':' separator\n",
          (int) location->path.size, location->path.data,
          location->line, location->column);
}

void make_interpreter_init(struct make_interpreter *interpreter) {

  struct make_parser *parser;
  struct make_phooks *phooks;

  make_ihooks_init(&interpreter->hooks);
  make_target_init(&interpreter->target);
  make_parser_init(&interpreter->parser);
  make_table_init(&interpreter->table);
  make_job_manager_init(&interpreter->job_manager);

  parser = &interpreter->parser;

  phooks = &parser->hooks;
  make_phooks_init(phooks);
  phooks->data = interpreter;
  phooks->on_rule_start = on_rule_start;
  phooks->on_rule_finish = on_rule_finish;
  phooks->on_target = on_target;
  phooks->on_prerequisite = on_prerequisite;
  phooks->on_command = on_command;
  phooks->on_assignment_stmt = on_assignment_stmt;
  phooks->on_include_stmt = on_include_stmt;
  phooks->on_unexpected_char = on_unexpected_char;
  phooks->on_missing_separator = on_missing_separator;

  interpreter->just_print = 0;
  interpreter->silent = 0;

  interpreter->phony_found = 0;

  interpreter->target_mtime = 0;
  interpreter->target_found = 0;
  interpreter->target_found_once = 0;
  interpreter->target_exists = 0;
  interpreter->target_expired = 0;

  interpreter->outlog = stdout;
  interpreter->errlog = stderr;
}

void make_interpreter_free(struct make_interpreter *interpreter) {
  make_target_free(&interpreter->target);
  make_parser_free(&interpreter->parser);
  make_table_free(&interpreter->table);
  make_job_manager_free(&interpreter->job_manager);
}

int make_interpreter_define(struct make_interpreter *interpreter,
                            const struct make_string *key,
                            const struct make_string *value) {
  return make_table_define(&interpreter->table, key, value);
}

int make_interpreter_has_target(const struct make_interpreter *interpreter) {
  if (interpreter->target.path.size == 0)
    return 0;
  else
    return 1;
}

int make_interpreter_read(struct make_interpreter *interpreter,
                          const char *filename) {
  return make_parser_read(&interpreter->parser, filename);
}

int make_interpreter_run(struct make_interpreter *interpreter) {

  int err;

  err = make_parser_run(&interpreter->parser);
  if (err)
    return err;

  err = make_job_manager_wait_for_all(&interpreter->job_manager);
  if (err)
    return err;

  if (!interpreter->target_found_once
   && !interpreter->target_exists) {
    fprintf(interpreter->errlog,
            "No rule to make target '%s'\n",
            interpreter->target.path.data);
    return -EINVAL;
  }

  return 0;
}

void make_interpreter_set_hooks(struct make_interpreter *interpreter,
                                const struct make_ihooks *hooks) {
  interpreter->hooks = *hooks;
}

int make_interpreter_set_target(struct make_interpreter *interpreter,
                                const struct make_string *target_path) {

  int err;
  struct stat target_stat;

  make_target_free(&interpreter->target);
  make_target_init(&interpreter->target);

  err = make_target_set_path(&interpreter->target, target_path);
  if (err)
    return err;

  err = make_table_set_target(&interpreter->table, target_path);
  if (err)
    return err;

  err = stat(interpreter->target.path.data, &target_stat);
  if (err) {
    if (errno == ENOENT)
      interpreter->target_exists = 0;
    else {
      fprintf(interpreter->errlog,
              "Failed to stat '%.*s'\n", 
              (int) interpreter->target.path.size,
              interpreter->target.path.data);
      return -errno;
    }
  } else {
    interpreter->target_exists = 1;
    interpreter->target_mtime = target_stat.st_mtime;
  }

  err = make_ihooks_notify_target(&interpreter->hooks, &interpreter->target);
  if (err)
    return err;

  return 0;
}

