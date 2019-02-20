#include "mk_tree.h"

#include "mk_node.h"

#include <stdlib.h>

/** A tree is a fully parsed Makefile.
 * @ingroup mk_internals
 * */
struct mk_tree
{
  /** The array of nodes that make up the tree. */
  struct mk_node** node_array;
  /** The number of nodes in the tree. */
  size_t node_count;
  /** The number of slots allocated in the node array. */
  size_t node_max;
};

struct mk_tree* mk_tree_create() {

  struct mk_tree* tree = malloc(sizeof(*tree));
  if (!tree) {
    return NULL;
  }

  tree->node_array = NULL;
  tree->node_count = 0;
  tree->node_max = 0;

  return tree;
}

void mk_tree_destroy(struct mk_tree* tree) {

  for (size_t i = 0; i < tree->node_count; i++) {
    mk_node_destroy(tree->node_array[i]);
  }

  free(tree->node_array);
  free(tree);
}

static int mk_tree_expand(struct mk_tree* tree) {

  size_t node_max = tree->node_max + 32;

  struct mk_node** node_array = tree->node_array;

  node_array = realloc(node_array, sizeof(struct mk_node*) * node_max);
  if (!node_array) {
    return -1;
  }

  tree->node_array = node_array;
  tree->node_max = node_max;

  return 0;
}

int mk_tree_push_rule(struct mk_tree* tree, struct mk_rule* rule) {

  struct mk_node* node = mk_node_create(rule, MK_NODE_RULE);
  if (!node) {
    return -1;
  }

  if (tree->node_count >= tree->node_max) {
    int err = mk_tree_expand(tree);
    if (err) {
      return err;
    }
  }

  tree->node_array[tree->node_count++] = node;

  return 0;
}
