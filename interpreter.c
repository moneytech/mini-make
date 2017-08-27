#include <make/interpreter.h>

#include <make/command.h>
#include <make/listener.h>
#include <make/parser.h>
#include <make/string.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#if defined(__unix__)
#include <unistd.h>
#elif defined(_WIN32)
#define stat _stat
#endif

static int on_rule_start(void *data) {

  struct make_interpreter *interpreter;

  interpreter = (struct make_interpreter *) data;
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

  interpreter = (struct make_interpreter *) data;

  if (interpreter->target.size == 0) {
    err = make_interpreter_set_target(interpreter, target);
    if (err)
      return err;
  } else if (make_string_equal(&interpreter->target, target)) {
    interpreter->target_found = 1;
    interpreter->target_found_once = 1;
  }

  return 0;
}

static int on_prerequisite(void *data, const struct make_string *prerequisite) {

  int err;
  struct stat prerequisite_stat;
  struct make_interpreter *interpreter;
  struct make_string path;

  interpreter = (struct make_interpreter *) data;

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
      free(path.data);
      return err;
    }
  } else if (prerequisite_stat.st_mtime > interpreter->target_mtime) {
    /* Prerequisite is newer than the target. */
    interpreter->target_expired = 1;
  }

  free(path.data);

  return 0;
}

static int on_command(void *data, const struct make_command *command) {

  struct make_interpreter *interpreter;

  interpreter = (struct make_interpreter *) data;

  if (!interpreter->target_expired)
    return 0;

  if (!command->silent) {
    fprintf(interpreter->outlog, "%.*s\n",
            (int) command->source->size,
            command->source->data);
  }

  return 0;
}

static int on_assignment_stmt(void *data, const struct make_assignment_stmt *assignment_stmt) {
  (void) data;
  (void) assignment_stmt;
  return 0;
}

static int on_include_stmt(void *data, const struct make_include_stmt *include_stmt) {
  (void) data;
  (void) include_stmt;
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

  err = stat(interpreter->target.data, &target_stat);
  if (err) {
    if (errno == ENOENT)
      interpreter->target_exists = 0;
    else
      return -errno;
  } else {
    interpreter->target_exists = 1;
    interpreter->target_mtime = target_stat.st_mtime;
  }

  return 0;
}

