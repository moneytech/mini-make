#include "mk_state.h"

#include "mk_run.h"

#include <stdlib.h>

int main(int argc, char **argv) {

  struct mk_state* state = mk_state_create();
  if (!state) {
    return EXIT_FAILURE;
  }

  int err = mk_run(state, argc, argv);

  mk_state_destroy(state);

  if (err)
    return EXIT_FAILURE;
  else
    return EXIT_SUCCESS;
}
