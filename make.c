#include <make/options.h>
#include <make/string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

  int i;
  struct make_options options;
  struct make_string target;

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
      fprintf(stdout, "Building target: %s\n", argv[i]);
      target.data = argv[i];
      target.size = strlen(argv[i]);
      (void) target;
    }
  }

  return EXIT_SUCCESS;
}

