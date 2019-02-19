#ifndef mk_tree_h
#define mk_tree_h

#include <stddef.h>

struct mk_node;

/** A tree is a fully parsed Makefile.
 * @ingroup mk_internal
 * */
struct mk_tree
{
  /** The array of nodes that make up the tree. */
  struct mk_node* node_array;
  /** The number of nodes in the tree. */
  size_t node_count;
  /** The number of slots allocated in the node array. */
  size_t node_max;
};

#endif /* mk_tree_h */
