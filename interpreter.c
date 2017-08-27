#include <make/interpreter.h>

#include <make/command.h>
#include <make/include-stmt.h>
#include <make/listener.h>
#include <make/parser.h>
#include <make/string.h>

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

  if (make_string_equal(&interpreter->target, &evaluated_target)) {
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

  err = build_prerequisite(interpreter, prerequisite);
  if (err) {
    return err;
  }

  err = make_string_copy(prerequisite, &path);
  if (err)
    return err;

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
      free(path.data);
      return -errno;
    }
  } else if (prerequisite_stat.st_mtime > interpreter->target_mtime) {
    /* Prerequisite is newer than the target. */
    interpreter->target_expired = 1;
  }

  free(path.data);

  return 0;
}

static int on_command(void *data, const struct make_command *command) {

  int err;
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
                            command->source,
                            &command_str);
  if (err) {
    make_string_free(&command_str);
    return err;
  }

  if (!command->silent && !interpreter->silent) {
    fprintf(interpreter->outlog, "%.*s\n",
            (int) command_str.size,
            command_str.data);
  }

  if (!interpreter->just_print) {
    err = make_job_manager_queue(&interpreter->job_manager,
                                 &command_str);
    if (err && !command->ignore_error) {
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
    fprintf(interpreter->errlog,
            "Failed to open '%s'\n",
            path.data);
    make_string_free(&path);
    make_parser_free(&new_parser);
    return err;
  }
  make_string_free(&path);

  new_parser.listener = interpreter->parser.listener;
  err = make_parser_run(&new_parser);
  if (err) {
    make_parser_free(&new_parser);
    return err;
  }
  make_parser_free(&new_parser);

  return 0;
}

static void on_unexpected_char(void *data, char c) {

  struct make_interpreter *interpreter;

  interpreter = (struct make_interpreter *) data;

  fprintf(interpreter->errlog, "Unexpected character '%c'\n", c);
}

static void on_missing_separator(void *data) {

  struct make_interpreter *interpreter;

  interpreter = (struct make_interpreter *) data;

  fprintf(interpreter->errlog, "Missing separator\n");
}

void make_interpreter_init(struct make_interpreter *interpreter) {

  struct make_parser *parser;
  struct make_listener *listener;

  make_string_init(&interpreter->target);
  make_parser_init(&interpreter->parser);
  make_table_init(&interpreter->table);
  make_job_manager_init(&interpreter->job_manager);

  parser = &interpreter->parser;

  listener = &parser->listener;
  listener->user_data = interpreter;
  listener->on_rule_start = on_rule_start;
  listener->on_rule_finish = on_rule_finish;
  listener->on_target = on_target;
  listener->on_prerequisite = on_prerequisite;
  listener->on_command = on_command;
  listener->on_assignment_stmt = on_assignment_stmt;
  listener->on_include_stmt = on_include_stmt;
  listener->on_unexpected_char = on_unexpected_char;
  listener->on_missing_separator = on_missing_separator;

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
  make_string_free(&interpreter->target);
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
  if (interpreter->target.size == 0)
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

  if (!interpreter->target_found_once
   && !interpreter->target_exists) {
    fprintf(interpreter->errlog,
            "No rule to make target '%s'\n",
            interpreter->target.data);
    return -EINVAL;
  }

  return 0;
}

int make_interpreter_set_target(struct make_interpreter *interpreter,
                                const struct make_string *target) {

  int err;
  struct stat target_stat;

  make_string_free(&interpreter->target);

  err = make_string_copy(target, &interpreter->target);
  if (err)
    return err;

  err = make_table_set_target(&interpreter->table, target);
  if (err)
    return err;

  err = stat(interpreter->table.target.data, &target_stat);
  if (err) {
    if (errno == ENOENT)
      interpreter->target_exists = 0;
    else {
      fprintf(interpreter->errlog,
              "Failed to stat '%.*s'\n", 
              (int) target->size,
              target->data);
      return -errno;
    }
  } else {
    interpreter->target_exists = 1;
    interpreter->target_mtime = target_stat.st_mtime;
  }

  return 0;
}

