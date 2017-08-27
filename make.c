#include <make/listener.h>
#include <make/interpreter.h>
#include <make/options.h>
#include <make/parser.h>
#include <make/string.h>
#include <make/table.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

  int err;
  int i;
  struct make_interpreter interpreter;
  struct make_string target;
  struct make_options options;

  make_interpreter_init(&interpreter);

  options.filename = "Makefile";
  options.working_dir = ".";
  options.jobs = 1;

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-C") == 0) {
      options.working_dir = argv[i + 1];
      if (options.filename == NULL) {
        fprintf(stderr, "No directory given for '%s'\n", argv[i]);
        return EXIT_FAILURE;
      }
      i++;
    } else if (strcmp(argv[i], "-f") == 0) {
      options.filename = argv[i + 1];
      if (options.filename == NULL) {
        fprintf(stderr, "No file name given for '%s'\n", argv[i]);
        return EXIT_FAILURE;
      }
      i++;
    } else if (strcmp(argv[i], "-j") == 0) {
      if (sscanf(argv[i + 1], "%u", &options.jobs) != 0) {
        fprintf(stderr, "Incorrect numerical value given for '%s'\n", argv[i]);
        return EXIT_FAILURE;
      }
    } else {
      /* The non-options will be
       * parsed in the next loop */
      break;
    }
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

