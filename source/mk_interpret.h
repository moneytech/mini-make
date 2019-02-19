#ifndef mk_interpret_h
#define mk_interpret_h

struct mk_state;
struct mk_tree;

/** Interprets a makefile tree.
 * @param state The state to interpret the makefile.
 * @param tree The makefile tree to interpret.
 * @returns Zero on success, non-zero on failure.
 * @ingroup mk_internal
 * */
int mk_interpret(struct mk_state* state, struct mk_tree* tree);

#endif /* mk_interpret_h */
