#include <make/listener.h>
#include <make/interpreter.h>
#include <make/options.h>
#include <make/parser.h>
#include <make/string.h>
#include <make/table.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_source(const char *path, unsigned long int *source_size) {

  FILE *file;
  char *source;
  long int file_pos;
  size_t read_size;

  file = fopen(path, "r");
  if (file == NULL)
    return NULL;

  if (fseek(file, 0, SEEK_END) < 0) {
    fclose(file);
    return NULL;
  }

  file_pos = ftell(file);
  if (file_pos < 0) {
    fclose(file);
    return NULL;
  }

  if (fseek(file, 0, SEEK_SET) < 0) {
    fclose(file);
    return NULL;
  }

  source = malloc(file_pos + 1);
  if (source == NULL) {
    fclose(file);
    return NULL;
  }

  read_size = fread(source, 1, file_pos, file);
  if (source_size != NULL)
    *source_size = (unsigned long int) read_size;

  source[read_size] = 0;

  fclose(file);

  return source;
}

int main(int argc, char **argv) {

  int err;
  int i;
  struct make_interpreter interpreter;
  struct make_listener listener;
  struct make_options options;
  struct make_parser parser;
  struct make_string source;
  struct make_string target;
  struct make_table table;

  source.data = NULL;
  source.size = 0;

  parser.source = &source;
  parser.listener = &listener;

  interpreter.parser = &parser;
  interpreter.target = NULL;
  interpreter.table = &table;
  interpreter.outlog = stdout;
  interpreter.errlog = stderr;

  make_table_init(&table);

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

  source.data = read_source(options.filename, &source.size);
  if (source.data == NULL) {
    fprintf(stderr, "Failed to read '%s'\n", options.filename);
    make_table_free(&table);
    return EXIT_FAILURE;
  }

  if (i >= argc) {
    /* The interpreter will find the first
     * target in the makefile and build it */
    target.data = NULL;
    target.size = 0;
    interpreter.target = &target;
    err = make_interpreter_run(&interpreter);
  }

  /* Parses the target names and assignment
   * statements given on the command line. */
  while (i < argc) {
    fprintf(stdout, "Building target: %s\n", argv[i]);
    target.data = argv[i];
    target.size = strlen(argv[i]);
    interpreter.target = &target;
    err = make_interpreter_run(&interpreter);
    if (err)
      break;
    else
      i++;
  }

  free(source.data);

  make_table_free(&table);

  if (err)
    return EXIT_FAILURE;
  else
    return EXIT_SUCCESS;
}

