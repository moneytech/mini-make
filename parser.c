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

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int make_is_space(char c) {
  if (isspace(c))
    return 1;
  else
    return 0;
}

static int make_is_filechar(char c) {
  if ((isalnum(c))
   || (c == '(') || (c == ')')
   || (c == '{') || (c == '}')
   || (c == '/')
   || (c == '\\')
   || (c == '$')
   || (c == '\"')
   || (c == '\'')
   || (c == '.')
   || (c == '_')
   || (c == '+')
   || (c == '-'))
    return 1;
  else
    return 0;
}

static int make_is_prerequisite_char(char c) {
  if (make_is_filechar(c) || (c == ':'))
    return 1;
  else
    return 0;
}

static int make_is_operator_char(char c) {
  if ((c == '=')
   || (c == '?')
   || (c == '+')
   || (c == ':'))
    return 1;
  return 0;
}

static void get_location(const struct make_parser *parser,
                         unsigned long int i,
                         struct make_location *location) {

  char c;
  unsigned long int j;
  const struct make_string *source;

  source = &parser->source;

  location->path = parser->path;
  location->line = 1;
  location->column = 1;

  for (j = 0; j < i; j++) {
    c = source->data[j];
    if (c == '\n') {
      location->line++;
      location->column = 1;
    } else {
      location->column++;
    }
  }
}

static void unexpected_char(struct make_parser *parser,
                            unsigned long int i) {
  struct make_location location;
  struct make_listener *listener;
  struct make_string *source;
  void *user_data;
  char c;

  get_location(parser, i, &location);

  source = &parser->source;
  c = source->data[i];

  listener = &parser->listener;
  user_data = listener->user_data;
  listener->on_unexpected_char(user_data, c, &location);
}

static void missing_separator(struct make_parser *parser,
                              unsigned long int i) {
  struct make_location location;
  struct make_listener *listener;
  void *user_data;

  get_location(parser, i, &location);

  listener = &parser->listener;
  user_data = listener->user_data;
  listener->on_missing_separator(user_data, &location);
}

static int assignment_stmt(struct make_parser *parser,
                           unsigned long int i,
                           unsigned long int *j) {

  int err;
  char c;
  int escape_found;
  struct make_string *source;
  struct make_listener *listener;
  struct make_string key;
  struct make_string value;
  struct make_assignment_stmt assignment_stmt;

  listener = &parser->listener;

  key.data = NULL;
  key.size = 0;

  value.data = NULL;
  value.size = 0;

  assignment_stmt.operation = MINI_MAKE_OPERATION_RECURSIVE;
  assignment_stmt.key = &key;
  assignment_stmt.value = &value;

  source = &parser->source;

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
    } else if (make_is_operator_char(c)) {
        break;
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
      assignment_stmt.operation = MINI_MAKE_OPERATION_RECURSIVE;
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
        assignment_stmt.operation = MINI_MAKE_OPERATION_APPEND;
        i++;
        break;
      } else if (c == '?') {
        assignment_stmt.operation = MINI_MAKE_OPERATION_CONDITIONAL;
        i++;
        break;
      } else if (c == ':') {
        assignment_stmt.operation = MINI_MAKE_OPERATION_STATIC;
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
  escape_found = 0;
  while (i < source->size) {
    c = source->data[i];
    if (c == '\\') {
      if (escape_found) {
        if (value.size == 0)
          value.data = &source->data[i];
        value.size++;
        escape_found = 0;
      } else {
        escape_found = 1;
        if (value.size > 0)
          value.size++;
      }
      i++;
      continue;
    } else if (c == '\n') {
      if (escape_found) {
        escape_found = 0;
        if (value.size > 0)
          value.size++;
        i++;
        continue;
      } else
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

static int comment(struct make_parser *parser,
                   unsigned long int i,
                   unsigned long int *j) {
  char c;
  struct make_string *source;
  source = &parser->source;
  while (i < source->size) {
    c = source->data[i];
    if (make_is_space(c)) {
      i++;
      continue;
    } else if (c != '#') {
      break;
    }
    /* Found a comment.
     * Move the parser passed
     * the comment. */
    while (i < source->size) {
      c = source->data[i];
      if (c == '\n')
        break;
      i++;
    }
    i++;
  }
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

  listener = &parser->listener;

  source = &parser->source;

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
  int escape_found;
  struct make_listener *listener;
  struct make_string *source;
  char c;
  struct make_string target;
  struct make_string prerequisite;
  struct make_string command_source;
  struct make_command command;

  listener = &parser->listener;

  source = &parser->source;

  err = make_listener_notify_rule_start(listener);
  if (err)
    return err;

  /* This loop parses all of the rule's
   * targets */
  while (i < source->size) {
    c = source->data[i];
    if (c == '\n') {
      missing_separator(parser, i);
      return -EINVAL;
    } else if (make_is_space(c)) {
      i++;
      continue;
    } else if (c == ':') {
      i++;
      break;
    } else if (make_is_filechar(c)) {
      target.data = &source->data[i];
      target.size = 0;
      while (i < source->size) {
        c = source->data[i];
        if (!make_is_filechar(c))
          break;
        target.size++;
        i++;
      }
      err = listener->on_target(listener->user_data, &target);
      if (err)
        return err;
    } else {
      unexpected_char(parser, i);
      return -EINVAL;
    }
  }

  /* This loop parses all of the rule's prerequisites */
  escape_found = 0;
  while (i < source->size) {
    c = source->data[i];
    if (c == '\\') {
      if (escape_found) {
        escape_found = 0;
      } else {
        escape_found = 1;
        i++;
        continue;
      }
    }
    if (c == '\n') {
      if (escape_found) {
        escape_found = 0;
        i++;
        continue;
      } else {
        i++;
        break;
      }
    } else if (make_is_space(c)) {
      i++;
      continue;
    } else if (make_is_prerequisite_char(c)) {
      prerequisite.data = &source->data[i];
      prerequisite.size = 0;
      escape_found = 0;
      while (i < source->size) {
        c = source->data[i];
        if (c == '\n') {
          if (escape_found) {
            escape_found = 0;
            i++;
          }
          break;
        } else if (c == '\\') {
          if (escape_found)
            escape_found = 0;
          else {
            escape_found = 1;
            i++;
            continue;
          }
        }
        if (!make_is_prerequisite_char(c))
          break;
        prerequisite.size++;
        i++;
      }
      escape_found = 0;
      err = listener->on_prerequisite(listener->user_data, &prerequisite);
      if (err)
        return err;
    } else {
      unexpected_char(parser, i);
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

  err = make_listener_notify_rule_finish(listener);
  if (err)
    return err;

  *j = i;

  return 0;
}

void make_parser_init(struct make_parser *parser) {
  make_string_init(&parser->path);
  make_string_init(&parser->source);
  make_listener_init(&parser->listener);
}

void make_parser_free(struct make_parser *parser) {
  make_string_free(&parser->path);
  make_string_free(&parser->source);
}

int make_parser_read(struct make_parser *parser,
                     const char *filename) {

  int err;
  FILE *file;
  long int file_pos;

  err = make_string_set_asciiz(&parser->path, filename);
  if (err) {
    return err;
  }

  file = fopen(filename, "r");
  if (file == NULL)
    return -ENOENT;

  err = fseek(file, 0, SEEK_END);
  if (err < 0) {
    err = -errno;
    fclose(file);
    return err;
  }

  file_pos = ftell(file);
  if (file_pos < 0) {
    err = -errno;
    fclose(file);
    return err;
  }

  err = fseek(file, 0, SEEK_SET);
  if (err < 0) {
    err = -errno;
    fclose(file);
    return err;
  }

  parser->source.data = malloc(file_pos + 1);
  if (parser->source.data == NULL) {
    fclose(file);
    return -ENOMEM;
  }

  parser->source.size = fread(parser->source.data,
                              1, file_pos, file);

  parser->source.data[parser->source.size] = 0;

  fclose(file);

  return 0;
}

int make_parser_run(struct make_parser *parser) {

  int err;
  unsigned long int i;
  unsigned long int j;

  i = 0;
  j = 0;

  while (i < parser->source.size) {

    err = comment(parser, i, &j);
    if (err)
      return err;
    else if (j > i) {
      i = j;
      continue;
    }

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

