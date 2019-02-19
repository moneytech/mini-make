#include "mk_parse.h"

#include "mk_interpret.h"
#include "mk_malloc.h"
#include "mk_node.h"
#include "mk_report.h"
#include "mk_rule.h"
#include "mk_token.h"
#include "mk_tree.h"

static struct mk_rule* mk_rule_create(struct mk_state* state) {

  struct mk_rule* rule = mk_malloc(state, sizeof(struct mk_rule));
  if (!rule) {
    return NULL;
  }

  rule->targets = mk_malloc(state, sizeof(char*));
  if (!rule->targets) {
    mk_free(state, rule);
    return NULL;
  } else {
    rule->targets[0] = NULL;
  }

  rule->deps = mk_malloc(state, sizeof(char*));
  if (!rule->deps) {
    mk_free(state, rule->targets);
    mk_free(state, rule);
    return NULL;
  } else {
    rule->deps[0] = NULL;
  }

  rule->recipe = NULL;
  if (!rule->recipe) {
    mk_free(state, rule->deps);
    mk_free(state, rule->targets);
    mk_free(state, rule);
    return NULL;
  } else {
    rule->recipe[0] = NULL;
  }

  return rule;
}

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

  mk_free(state, rule);
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
  tree->node_max = 0;

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
  /** The current node being built by the parser. */
  struct mk_node* node;
};

static int mk_parser_done(const struct mk_parser* parser) {
  return !parser->token_count;
}

static int mk_push_node(struct mk_parser* parser) {

  if (!parser->node || (parser->node->type == MK_NODE_NULL)) {
    return 0;
  }

  struct mk_tree* tree = parser->tree;

  struct mk_node* node_array = tree->node_array;

  size_t node_count = tree->node_count;

  if (node_count >= tree->node_max) {

    node_array = mk_realloc(parser->state, node_array, (tree->node_max + 16) * sizeof(struct mk_node));
    if (!node_array) {
      return -1;
    }

    tree->node_array = node_array;
    tree->node_max += 16;
  }

  struct mk_node* node = &tree->node_array[tree->node_count];

  *node = *parser->node;

  parser->node->type = MK_NODE_NULL;

  tree->node_count++;

  return 0;
}

enum mk_parse_match {
  MK_MATCH_NONE,
  MK_MATCH_FAIL,
  MK_MATCH_GOOD
};

static int mk_unexpected_token(struct mk_parser* parser) {

  const struct mk_token* token = parser->token_array;

  const char* type_name = "symbol";

  switch (token->type) {
    case MK_TOKEN_SPACE:
      type_name = "space";
      break;
    case MK_TOKEN_NEWLINE:
      type_name = "newline";
      break;
    case MK_TOKEN_IDENTIFIER:
      type_name = "identifier";
      break;
    case MK_TOKEN_NUMBER:
      type_name = "number";
      break;
    case MK_TOKEN_COMMENT:
      type_name = "comment";
      break;
    default:
      break;
  }

  if ((token->type != MK_TOKEN_SPACE)
   && (token->type != MK_TOKEN_NEWLINE)) {
    mk_report(parser->state, "unexpected %s '%.*s'", type_name, (int) token->size, token->data);
  } else {
    mk_report(parser->state, "unexpected %s", type_name);
  }

  return -1;
}

static enum mk_parse_match mk_parse_rule(struct mk_parser* parser) {

  struct mk_rule* rule = mk_rule_create(parser->state);
  if (!rule) {
    return MK_MATCH_FAIL;
  }

  (void)parser;
  return MK_MATCH_NONE;
}

static enum mk_parse_match mk_parse_unused(struct mk_parser* parser) {

  const struct mk_token* token = parser->token_array;

  switch (token->type) {
    case MK_TOKEN_COMMENT:
    case MK_TOKEN_SPACE:
    case MK_TOKEN_NEWLINE:
      parser->token_array++;
      parser->token_count--;
      return MK_MATCH_GOOD;
    default:
      break;
  }

  return MK_MATCH_NONE;
}

#define MK_TRY_PARSE(parser, func)   \
  do {                               \
    switch (func(parser)) {          \
      case MK_MATCH_NONE:            \
        break;                       \
      case MK_MATCH_FAIL:            \
        return -1;                   \
      case MK_MATCH_GOOD:            \
        return mk_push_node(parser); \
    }                                \
  } while (0)

static int mk_parse_node(struct mk_parser* parser) {

  MK_TRY_PARSE(parser, mk_parse_unused);
  MK_TRY_PARSE(parser, mk_parse_rule);

  return mk_unexpected_token(parser);
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
  parser.node = NULL;

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

  err = mk_interpret(state, tree);

  mk_tree_destroy(tree, state);

  return err;
}
