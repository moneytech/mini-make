#include "mk_parse.h"

#include "mk_malloc.h"
#include "mk_node.h"
#include "mk_rule.h"
#include "mk_tree.h"

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

struct mk_parser {
  /** The state, used for allocating memory. */
  struct mk_state* state;
  /** The token array being parsed. */
  const struct mk_token* token_array;
  /** The number of tokens left in the token array. */
  size_t token_count;
  /** The tree being built by the parser. */
  struct mk_tree* tree;
};

static int mk_parser_done(const struct mk_parser* parser) {
  return !parser->token_count;
}

static int mk_parse_node(struct mk_parser* parser) {
  parser->token_count--;
  return 0;
}

int mk_parse(struct mk_state* state,
             const struct mk_token* token_array,
             size_t token_count) {

  struct mk_tree* tree = mk_tree_create(state);
  if (!tree) {
    return -1;
  }

  struct mk_parser parser;
  parser.state = state;
  parser.token_array = token_array;
  parser.token_count = token_count;
  parser.tree = tree;

  int err = 0;

  while (!mk_parser_done(&parser)) {
    err = mk_parse_node(&parser);
    if (err) {
      break;
    }
  }

  if (err) {
    mk_tree_destroy(tree, state);
    return err;
  }

  mk_tree_destroy(tree, state);

  return 0;
}
