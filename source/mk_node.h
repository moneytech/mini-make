#ifndef mk_node_h
#define mk_node_h

struct mk_rule;

/** Enumerates the several types of nodes.
 * @ingroup mk_internals
 * */
enum mk_node_type {
  /** The node has no type. */
  MK_NODE_NULL,
  /** The node is a rule. */
  MK_NODE_RULE
};

struct mk_node;

/** Creates a new node.
 * @param data The data associated with the node.
 * @param type The type associated with the node.
 * @returns A new node instance.
 * @ingroup mk_internals
 * */
struct mk_node* mk_node_create(void* data, enum mk_node_type type);

/** Releases memory allocated by a node.
 * @param node The node to release the memory for.
 * @ingroup mk_internals
 * */
void mk_node_destroy(struct mk_node* node);

#endif /* mk_node_h */
