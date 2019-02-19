#include "mk_run.h"

#include "mk_open.h"
#include "mk_report.h"
#include "mk_state.h"

static int mk_define_arg(struct mk_state* state, const char* arg) {
  (void)state;
  (void)arg;
  return 0;
}

int mk_run(struct mk_state* state, int argc, char** argv) {

  int err = mk_state_define(state, "__mini_make__", argv[0]);
  if (err != 0) {
    return err;
  }

  const char* filename = "Makefile";
  const char* working_dir = ".";

  for (int i = 1; i < argc; i++) {

    const char* arg = argv[i];

    if (arg[0] != '-') {
      /* queue target */
    } else if (arg[1] == 'D') {
      err = mk_define_arg(state, &arg[2]);
      if (err) {
        break;
      }
    } else if (arg[1] == 'C') {
      working_dir = argv[i + 1];
      i++;
    } else if (arg[1] == 'f') {
      filename = argv[i + 1];
      i++;
    } else {
      mk_report(state, "unknown option '%s'", arg);
      return -1;
    }
  }

  if (err) {
    return err;
  }

  if (!filename) {
    mk_report(state, "-f requires filename");
    return -1;
  }

  err = mk_state_define(state, "__file__", filename);
  if (err != 0) {
    return err;
  }

  if (!working_dir) {
    mk_report(state, "-C requires directory path");
    return -1;
  }

  err = mk_state_define(state, "CURDIR", working_dir);
  if (err) {
    return err;
  }

  return mk_open(state);
}
