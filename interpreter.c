#include <make/interpreter.h>

#include <make/command.h>
#include <make/listener.h>
#include <make/parser.h>
#include <make/string.h>

#include <stdio.h>

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

  struct make_interpreter *interpreter;

  interpreter = (struct make_interpreter *) data;

  (void) interpreter;
  (void) target;

  return 0;
}

static int on_prerequisite(void *data, const struct make_string *prerequisite) {

  struct make_interpreter *interpreter;

  interpreter = (struct make_interpreter *) data;

  (void) interpreter;
  (void) prerequisite;

  return 0;
}

static int on_command(void *data, const struct make_command *command) {

  struct make_interpreter *interpreter;

  interpreter = (struct make_interpreter *) data;

  interpreter->target_expired = 1;

  if (interpreter->target_expired) {
    fprintf(stdout, "%.*s\n",
            (int) command->source->size,
            command->source->data);
    /* RUN command */
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

int make_interpreter_run(struct make_interpreter *interpreter) {

  int err;
  struct make_parser *parser;
  struct make_listener *listener;

  parser = interpreter->parser;

  listener = parser->listener;
  listener->user_data = interpreter;
  listener->on_rule_start = on_rule_start;
  listener->on_rule_finish = on_rule_finish;
  listener->on_target = on_target;
  listener->on_prerequisite = on_prerequisite;
  listener->on_command = on_command;
  listener->on_assignment_stmt = on_assignment_stmt;
  listener->on_include_stmt = on_include_stmt;
  listener->on_unexpected_char = on_unexpected_char;

  err = make_parser_run(parser);
  if (err)
    return err;

  return 0;
}

