#include <stdio.h>
#include <stdlib.h>

#include <mini-make/error.h>
#include <mini-make/parser.h>

static int on_target(void *data, const struct make_string *target) {
  (void) data;
  (void) target;
  fprintf(stdout, "Found target: %.*s\n", (int) target->size, target->data);
  return make_success;
}

static int list_targets_of(const char *path) {

  int err;
  struct make_parser parser;

  make_parser_init(&parser);

  parser.hooks.on_target = on_target;

  err = make_parser_read(&parser, path);
  if (err) {
    fprintf(stderr, "Failed to read '%s'\n", path);
    make_parser_free(&parser);
    return EXIT_FAILURE;
  }

  err = make_parser_run(&parser);
  if (err) {
    fprintf(stderr, "Failed to parse '%s'\n", path);
    make_parser_free(&parser);
    return EXIT_FAILURE;
  }

  make_parser_free(&parser);

  return 0;
}

int main(int argc, const char **argv) {

  int i;
  int err;

  for (i = 1; i < argc; i++) {
    err = list_targets_of(argv[i]);
    if (err)
      return err;
  }

  return 0;
}

