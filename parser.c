#include <make/parser.h>

#include <make/assignment-stmt.h>
#include <make/command.h>
#include <make/include-stmt.h>
#include <make/listener.h>
#include <make/string.h>

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static int make_is_space(char c) {
  if (isspace(c))
    return 1;
  else
    return 0;
}

static int make_is_target(char c) {
  if ((isalnum(c))
   || (c == '.')
   || (c == '_')
   || (c == '-'))
    return 1;
  else
    return 0;
}

static int assignment_stmt(struct make_parser *parser,
                           unsigned long int i,
                           unsigned long int *j) {

  int err;
  char c;
  struct make_string *source;
  struct make_listener *listener;
  struct make_string key;
  struct make_string value;
  struct make_assignment_stmt assignment_stmt;

  listener = parser->listener;

  key.data = NULL;
  key.size = 0;

  value.data = NULL;
  value.size = 0;

  assignment_stmt.operation = MAKE_OPERATION_RECURSIVE;
  assignment_stmt.key = &key;
  assignment_stmt.value = &value;

  source = parser->source;

  /* This loop parses the assignment
   * statement's key */
  while (i < source->size) {
    c = source->data[i];
    if (make_is_space(c)) {
      if (key.size > 0)
        break;
      else {
        i++;
        continue;
      }
    } else {
      if (key.size == 0)
        key.data = &source->data[i];
      key.size++;
    }
    i++;
  }

  if (key.size == 0)
    /* A key was not found */
    return 0;

  /* This loop parses the assignment
   * statement's opertion */
  while (i < source->size) {
    c = source->data[i];
    if (c == '\n') {
      /* If a newline is found here, then
       * this is not an assignment statement. */
      return 0;
    } else if (make_is_space(c)) {
      i++;
      continue;
    } else if (c == '=') {
      assignment_stmt.operation = MAKE_OPERATION_RECURSIVE;
      i++;
      break;
    } else if ((c == '+')
            || (c == '?')
            || (c == ':')) {
      i++;
      if (i >= source->size) {
        /* Unexpected end of file. */
        listener->on_unexpected_eof(listener->user_data);
        return -EINVAL;
      } else if (source->data[i] != '=') {
        /* If a '=' wasn't following then this
         * is not an operation (and therefore,
         * not an assignment statement). */
        return 0;
      } else if (c == '+') {
        assignment_stmt.operation = MAKE_OPERATION_APPEND;
        i++;
        break;
      } else if (c == '?') {
        assignment_stmt.operation = MAKE_OPERATION_CONDITIONAL;
        i++;
        break;
      } else if (c == ':') {
        assignment_stmt.operation = MAKE_OPERATION_STATIC;
        i++;
        break;
      } else {
        /* Unreachable */
        return -EINVAL;
      }

    } else {
      /* Unexpected character, this is
       * must not be an assignment
       * statement. */
      return 0;
    }
  }

  /* This loop parses the assignment
   * statement's value */
  while (i < source->size) {
    c = source->data[i];
    if (c == '\n') {
      break;
    } else if (make_is_space(c) && (value.size == 0)) {
      i++;
      continue;
    } else {
      if (value.size == 0)
        value.data = &source->data[i];
      value.size++;
    }
    i++;
  }

  /* TODO : Get rid of trailing space */

  err = listener->on_assignment_stmt(listener->user_data, &assignment_stmt);
  if (err)
    return err;

  *j = i;

  return 0;
}

static int include_stmt(struct make_parser *parser,
                        unsigned long int i,
                        unsigned long int *j) {

  int err;
  char c;
  struct make_string *source;
  struct make_string include_path;
  struct make_include_stmt include_stmt;
  struct make_listener *listener;

  listener = parser->listener;

  source = parser->source;

  include_path.data = NULL;
  include_path.size = 0;

  include_stmt.ignore_error = 0;
  include_stmt.path = &include_path;

  /* Check to see if we can find a include
   * statement. */
  while (i < source->size) {
    c = source->data[i];
    if (make_is_space(c)) {
      i++;
      continue;
    } else if (c == '-') {
      include_stmt.ignore_error = 1;
      i++;
      continue;
    } else if ((i + sizeof("include")) < source->size) {
      /* If there isn't an 'include' keyword here,
       * break the loop */
      if ((memcmp(&source->data[i], "include", 7) != 0)
       || (!make_is_space(source->data[i + 7]))) {
        /* This was not an include statement. */
        return 0;
      }
      /* A 'include' keyword was found. Move passed
       * it and start parsing include paths */
      i += sizeof("include");
    } else {
      /* This was not an include statement */
      return 0;
    }

    /* Parse actual include paths */
    include_path.data = NULL;
    include_path.size = 0;
    while (i < source->size) {
      c = source->data[i];
      if (c == '\n') {
        if (include_path.size > 0) {
          err = listener->on_include_stmt(listener->user_data, &include_stmt);
          if (err)
            return err;
          /* This may not be necessary,
           * but it's a good safety measure. */
          include_path.data = NULL;
          include_path.size = 0;
        }
        break;
      } else if (make_is_space(c)) {
        if (include_path.size > 0) {
          err = listener->on_include_stmt(listener->user_data, &include_stmt);
          if (err)
            return err;
          include_path.data = NULL;
          include_path.size = 0;
        }
      } else {
        if (include_path.size == 0)
          include_path.data = &source->data[i];
        include_path.size++;
      }
      i++;
    }

    /* Done parsing include paths. */
    break;
  }

  /* Update the index. This tells the
   * caller that it should continue parsing
   * at a new index.  */
  *j = i;

  return 0;
}

static int rule(struct make_parser *parser,
                unsigned long int i,
                unsigned long int *j) {

  int err;
  struct make_listener *listener;
  struct make_string *source;
  char c;
  struct make_string target;
  struct make_string prerequisite;
  struct make_string command_source;
  struct make_command command;

  listener = parser->listener;

  source = parser->source;

  err = listener->on_rule_start(listener->user_data);
  if (err)
    return err;

  /* This loop parses all of the rule's
   * targets */
  while (i < source->size) {
    c = source->data[i];
    if (make_is_space(c)) {
      i++;
      continue;
    } else if (c == ':') {
      i++;
      break;
    } else if (c == '\n') {
      listener->on_missing_separator(listener->user_data);
      return -EINVAL;
    } else if (make_is_target(c)) {
      target.data = &source->data[i];
      target.size = 0;
      while (i < source->size) {
        c = source->data[i];
        if (!make_is_target(c))
          break;
        target.size++;
        i++;
      }
      err = listener->on_target(listener->user_data, &target);
      if (err)
        return err;
    } else {
      listener->on_unexpected_char(listener->user_data, c);
      return -EINVAL;
    }
  }

  /* This loop parses all of the rule's prerequisites */
  while (i < source->size) {
    c = source->data[i];
    if (c == '\n') {
      i++;
      break;
    } else if (make_is_space(c)) {
      i++;
      continue;
    } else if (make_is_target(c)) {
      prerequisite.data = &source->data[i];
      prerequisite.size = 0;
      while (i < source->size) {
        c = source->data[i];
        if (!make_is_target(c))
          break;
        prerequisite.size++;
        i++;
      }
      err = listener->on_prerequisite(listener->user_data, &prerequisite);
      if (err)
        return err;
    } else {
      listener->on_unexpected_char(listener->user_data, c);
      return -EINVAL;
    }
  }

  /* This loop parses all of the rule's commands */
  for (; i < source->size; i++) {

    c = source->data[i];
    if (c != '\t')
      break;
    else
      i++;

    command.ignore_error = 0;
    command.silent = 0;
    command.source = &command_source;
    command_source.data = NULL;
    command_source.size = 0;

    while (i < source->size) {
      c = source->data[i];
      if (source->data[i] == '-')
        command.ignore_error = 1;
      else if (source->data[i] == '@')
        command.silent = 1;
      else
        break;
      i++;
    }

    command_source.data = &source->data[i];
    command_source.size = 0;

    while (i < source->size) {
      c = source->data[i];
      if (c == '\n')
        break;
      command_source.size++;
      i++;
    }

    err = listener->on_command(listener->user_data, &command);
    if (err)
      return err;
  }

  err = listener->on_rule_finish(listener->user_data);
  if (err)
    return err;

  *j = i;

  return 0;
}

void make_parser_init(struct make_parser *parser) {
  parser->source = NULL;
  parser->listener = NULL;
}

int make_parser_run(struct make_parser *parser) {

  int err;
  unsigned long int i;
  unsigned long int j;

  i = 0;
  j = 0;

  while (i < parser->source->size) {

    err = include_stmt(parser, i, &j);
    if (err)
      return err;
    else if (j > i) {
      i = j;
      continue;
    }

    err = assignment_stmt(parser, i, &j);
    if (err)
      return err;
    else if (j > i) {
      i = j;
      continue;
    }

    err = rule(parser, i, &j);
    if (err)
      return err;
    else if (j > i) {
      i = j;
      continue;
    }
  }

  return 0;
}

