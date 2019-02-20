#include "mk_report.h"

#include "mk_state.h"

#include <stdarg.h>
#include <stdio.h>

void mk_report(struct mk_state* state, const char* fmt, ...) {

  const char* makefile = mk_state_find(state, "__file__");
  if (makefile) {
    fprintf(stderr, "%s: ", makefile);
  } else {
    const char* argv0 = mk_state_find(state, "__mini_make__");
    if (argv0) {
      fprintf(stderr, "%s: ", argv0);
    }
  }

  va_list args;

  va_start(args, fmt);

  vfprintf(stderr, fmt, args);

  va_end(args);

  fprintf(stderr, "\n");
}

void mk_syntax_report(struct mk_state* state, size_t line, const char* fmt, ...) {

  const char* makefile = mk_state_find(state, "__file__");
  if (makefile) {
    fprintf(stderr, "%s:%lu: ", makefile, (unsigned long int) line);
  } else {
    const char* argv0 = mk_state_find(state, "__mini_make__");
    if (argv0) {
      fprintf(stderr, "%s: ", argv0);
    }
  }

  va_list args;

  va_start(args, fmt);

  vfprintf(stderr, fmt, args);

  va_end(args);

  fprintf(stderr, "\n");
}
