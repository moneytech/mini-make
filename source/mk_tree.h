#ifndef mk_tree_h
#define mk_tree_h

struct mk_rule;
struct mk_tree;

/** Creates a new tree instance.
 * @returns A pointer to a new tree.
 * @ingroup mk_internals
 * */
struct mk_tree* mk_tree_create();

/** Releases memory allocated by a tree.
 * @param tree The tree to release the memory for.
 * @ingroup mk_internals
 * */
void mk_tree_destroy(struct mk_tree* tree);

/** Adds a rule to the tree.
 * @param tree The tree to add the rule to.
 * @param rule The rule to add to the tree.
 * After calling this function, the tree has
 * ownership of the rule.
 * @returns Zero on success, non-zero on failure.
 * @ingroup mk_internals
 * */
int mk_tree_push_rule(struct mk_tree* tree, struct mk_rule* rule);

#endif /* mk_tree_h */
