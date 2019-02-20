#include "mk_node.h"

#include "mk_rule.h"

#include <stdlib.h>

/** This structure contains data related to a node.
 * A node can be either a rule, assignment statement,
 * include statement, or a pattern.
 * */
struct mk_node {
  /** Indicates the type of this node. */
  enum mk_node_type type;
  /** Contains data for the several types of node data. */
  void* data;
};

struct mk_node* mk_node_create(void* data, enum mk_node_type type) {

  struct mk_node* node = malloc(sizeof(*node));
  if (!node) {
    return NULL;
  }

  node->type = type;
  node->data = data;
  return node;
}

void mk_node_destroy(struct mk_node* node) {

  switch (node->type) {
    case MK_NODE_NULL:
      break;
    case MK_NODE_RULE:
      mk_rule_destroy((struct mk_rule*)node->data);
      break;
  }

  node->type = MK_NODE_NULL;
  node->data = NULL;
}
