#include <make/parser.h>

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
   || (c == '_')
   || (c == '-'))
    return 1;
  else
    return 0;
}

void make_parser_init(struct make_parser *parser) {
  parser->source = NULL;
  parser->listener = NULL;
}

int make_parser_run(struct make_parser *parser) {

  int err;
  struct make_listener *listener;
  struct make_string *source;
  char c;
  /* Index of the parser within
   * the source code. */
  unsigned long int i;
  /* Used as a temporary index */
  unsigned long int j;
  struct make_string target;
  struct make_string prerequisite;
  struct make_string command_source;
  struct make_command command;
  struct make_string include_path;
  struct make_include_stmt include_stmt;

  if (parser == NULL)
    return -EFAULT;

  listener = parser->listener;
  if (listener == NULL)
    return -EFAULT;
  else if (listener->on_target == NULL)
    return -EFAULT;

  source = parser->source;
  if (source == NULL)
    return -EFAULT;
  else if (source->data == NULL)
    return -EFAULT;

  /* Setup the variable assignments that
   * aren't going to change throughout the
   * parse loop. */
  include_stmt.path = &include_path;
  command.source = &command_source;

  /* Initialize loop indices. */
  i = 0;
  j = 0;

  /* At this point the parser will start
   * parsing the source.
   *
   * Instead of writing the code in a 'for'
   * or 'while' loop, a goto statement was
   * sed to reduce indentation. */
parse_loop:

  /* Check to see if we can find a include
   * statement. */
  include_stmt.ignore_error = 0;
  /* Save a temporary index, incase this isn't
   * an include statement */
  j = i;
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
        /* Restore the main index, since this wasn't
         * an actual include statement.  */
        i = j;
        break;
      }
      /* A 'include' keyword was found. Move passed
       * it and start parsing include paths */
      i += sizeof("include");
    } else {
      /* Restore the main index, this is not
       * an include statement. */
      i = j;
      break;
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

    /* Done parsing include paths. Go
     * back to the top of the parse loop. */
    goto parse_loop;
  }

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

  /* If there's more makefile source code leftover,
   * keep parsing the source. */
  if (i < source->size)
    goto parse_loop;

  return 0;
}

