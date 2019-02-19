#include "mk_parse.h"

#include "mk_malloc.h"
#include "mk_node.h"
#include "mk_rule.h"

static void mk_rule_destroy(struct mk_rule* rule, struct mk_state* state) {

  for (size_t i = 0; rule->targets[i]; i++) {
    mk_free(state, rule->targets[i]);
  }

  for (size_t i = 0; rule->deps[i]; i++) {
    mk_free(state, rule->deps[i]);
  }

  for (size_t i = 0; rule->recipe[i]; i++) {
    mk_free(state, rule->recipe[i]);
  }
}

static void mk_node_destroy(struct mk_node* node, struct mk_state* state) {
  switch (node->type) {
    case MK_NODE_NULL:
      break;
    case MK_NODE_RULE:
      mk_rule_destroy(node->data.rule, state);
      break;
  }
}

struct mk_tree
{
  struct mk_node* node_array;
  size_t node_count;
};

static struct mk_tree* mk_tree_create(struct mk_state* state) {

  struct mk_tree* tree = mk_malloc(state, sizeof(*tree));
  if (!tree) {
    return NULL;
  }

  tree->node_array = NULL;
  tree->node_count = 0;

  return tree;
}

static void mk_tree_destroy(struct mk_tree* tree, struct mk_state* state) {

  for (size_t i = 0; i < tree->node_count; i++) {
    mk_node_destroy(&tree->node_array[i], state);
  }

  mk_free(state, tree);
}

int mk_parse(struct mk_state* state,
             const struct mk_token* token_array,
             size_t token_count) {

  struct mk_tree* tree = mk_tree_create(state);
  if (!tree) {
    return -1;
  }

  mk_tree_destroy(tree, state);

  (void)token_array;
  (void)token_count;

  return 0;
}
