#ifndef mk_run_h
#define mk_run_h

struct mk_state;

/** Runs the state on a command line invocation.
 * @param state The state to run the command line arguments on.
 * @param argc The argument count.
 * @param argv The argument array.
 * @returns Zero on success, non-zero on failure.
 * */
int mk_run(struct mk_state* state, int argc, char** argv);

#endif /* mk_run_h */
