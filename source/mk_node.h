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

/** This structure contains data related to a node.
 * A node can be either a rule, assignment statement,
 * include statement, or a pattern.
 * */
struct mk_node {
  /** Indicates the type of this node. */
  enum mk_node_type type;
  /** Contains data for the several types of node data. */
  union {
    /** Valid if the node is a rule type. */
    struct mk_rule* rule;
  } data;
};

#endif /* mk_node_h */
