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

#include <make/chdir.h>
#include <make/listener.h>
#include <make/interpreter.h>
#include <make/options.h>
#include <make/parser.h>
#include <make/string.h>
#include <make/table.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

char *argv0 = "mini-make";

static void print_help(void) {
  printf("Usage: %s [options] [target] ...\n", argv0);
  printf("\n");
  printf("Options:\n");
  printf("  -s, --silent          : Does not print any of the commands it runs.\n");
  printf("  -n, --just-print      : Prints the commands but does not run them.\n");
  printf("  -f, --file <FILE>     : Read FILE instead of default 'Makefile'.\n");
  printf("  -C, --directory <DIR> : Change directory to DIR before running makefile.\n");
  printf("  -j, --jobs <N>        : Use up to 'N' number of jobs.\n");
  printf("  -h, --help            : Print this help message.\n");
  printf("  -v, --version         : Print version information.\n");
  printf("\n");
  printf("Submit bug reports at https://github.com/tay10r/mini-make/issues/new\n");
}

static void print_version(void) {
  printf("Mini Make 0.1.1\n");
}

int main(int argc, char **argv) {

  int err;
  int i;
  struct make_interpreter interpreter;
  struct make_string target;
  struct make_options options;
  struct make_string make_key;
  struct make_string make_value;

  argv0 = argv[0];

  options.filename = "Makefile";
  options.working_dir = ".";
  options.jobs = 1;
  options.just_print = 0;
  options.silent = 0;

  for (i = 1; i < argc; i++) {
    if ((strcmp(argv[i], "-n") == 0)
     || (strcmp(argv[i], "--just-print") == 0)
     || (strcmp(argv[i], "--dry-run") == 0)
     || (strcmp(argv[i], "--recon") == 0)) {
      options.just_print = 1;
    } else if ((strcmp(argv[i], "-C") == 0)
            || (strcmp(argv[i], "--directory") == 0)) {
      options.working_dir = argv[i + 1];
      if (options.filename == NULL) {
        fprintf(stderr, "No directory given for '%s'\n", argv[i]);
        return EXIT_FAILURE;
      }
      i++;
    } else if ((strcmp(argv[i], "-f") == 0)
            || (strcmp(argv[i], "--file") == 0)
            || (strcmp(argv[i], "--makefile") == 0)) {
      options.filename = argv[i + 1];
      if (options.filename == NULL) {
        fprintf(stderr, "No file name given for '%s'\n", argv[i]);
        return EXIT_FAILURE;
      }
      i++;
    } else if ((strcmp(argv[i], "-j") == 0)
            || (strcmp(argv[i], "--jobs") == 0)) {
      if (i + 1 >= argc) {
          options.jobs = INT_MAX;
      } else if (sscanf(argv[i + 1], "%u", &options.jobs) != 0) {
        fprintf(stderr, "Incorrect numerical value given for '%s'\n", argv[i]);
        return EXIT_FAILURE;
      }
    } else if ((strcmp(argv[i], "-s") == 0)
            || (strcmp(argv[i], "--silent") == 0)
            || (strcmp(argv[i], "--quiet") == 0)) {
      options.silent = 1;
    } else if ((strcmp(argv[i], "-h") == 0)
            || (strcmp(argv[i], "--help") == 0)) {
      print_help();
      return EXIT_FAILURE;
    } else if ((strcmp(argv[i], "-v") == 0)
            || (strcmp(argv[i], "--version") == 0)) {
      print_version();
      return EXIT_FAILURE;
    } else if (argv[i][0] != '-') {
      /* The non-options will be
       * parsed in the next loop */
      break;
    } else {
      fprintf(stderr, "Unknown option '%s'\n", argv[i]);
      return EXIT_FAILURE;
    }
  }

  make_interpreter_init(&interpreter);

  if (options.just_print)
    interpreter.just_print = 1;

  if (options.silent)
    interpreter.silent = 1;

  make_key.data = "MAKE";
  make_key.size = 4;
  make_key.res = 0;

  make_string_init(&make_value);
  if ((make_string_set_asciiz(&make_value, argv0) != 0)
   || (make_string_prepend_char(&make_value, '\"') != 0)
   || (make_string_append_char(&make_value, '\"') != 0)) {
    make_string_free(&make_value);
    make_interpreter_free(&interpreter);
    return EXIT_FAILURE;
  }

  err = make_interpreter_define(&interpreter, &make_key, &make_value);
  if (err) {
    make_string_free(&make_value);
    make_interpreter_free(&interpreter);
    return err;
  }

  make_string_free(&make_value);

  err = make_chdir(options.working_dir);
  if (err < 0) {
    fprintf(stderr, "Failed to change directory to '%s'\n", options.working_dir);
    make_interpreter_free(&interpreter);
    return EXIT_FAILURE;
  }

  err = make_interpreter_read(&interpreter, options.filename);
  if (err < 0) {
    fprintf(stderr, "Failed to read '%s'\n", options.filename);
    make_interpreter_free(&interpreter);
    return EXIT_FAILURE;
  }

  if (i >= argc) {
    /* The interpreter will find the first
     * target in the makefile and build it */
    err = make_interpreter_run(&interpreter);
  }

  /* Parses the target names and assignment
   * statements given on the command line. */

  make_string_init(&target);

  while (i < argc) {

    err = make_string_set(&target, argv[i], strlen(argv[i]));
    if (err)
      break;

    err = make_interpreter_set_target(&interpreter, &target);
    if (err)
      break;

    err = make_interpreter_run(&interpreter);
    if (err)
      break;

    i++;
  }

  make_string_free(&target);

  make_interpreter_free(&interpreter);

  if (err)
    return EXIT_FAILURE;
  else
    return EXIT_SUCCESS;
}

