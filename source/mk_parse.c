#include "mk_parse.h"

#include "mk_interpret.h"
#include "mk_report.h"
#include "mk_rule.h"
#include "mk_token.h"
#include "mk_tree.h"

#include <stdlib.h>
#include <string.h>

struct mk_token_iterator
{
  const struct mk_token* it;
  const struct mk_token* end;
};

static const struct mk_token* mk_current_token(const struct mk_token_iterator* iterator) {
  return iterator->it;
}

static void mk_next_token(struct mk_token_iterator* iterator) {
  if (iterator->it != iterator->end) {
    iterator->it++;
  }
}

static int mk_at_end(struct mk_token_iterator* iterator) {
  return iterator->it == iterator->end;
}

static char* mk_parse_target(struct mk_token_iterator* iterator) {

  struct mk_token_iterator iterator_copy = *iterator;

  char* target = NULL;

  size_t target_size = 0;

  while (!mk_at_end(iterator)) {

    const struct mk_token* token = mk_current_token(iterator);
    if (!token) {
      break;
    }

    int terminated = 0;

    switch (token->type) {
      case MK_TOKEN_COMMENT:
      case MK_TOKEN_SPACE:
      case MK_TOKEN_COLON:
        terminated = 1;
        break;
      case MK_TOKEN_NEWLINE:
        free(target);
        *iterator = iterator_copy;
        return NULL;
      default:
        break;
    }

    if (terminated) {
      break;
    }

    char* tmp = realloc(target, target_size + token->size + 1);
    if (!tmp) {
      free(target);
      *iterator = iterator_copy;
      return NULL;
    }

    target = tmp;

    memcpy(target + target_size, token->data, token->size);

    target_size += token->size;

    target[target_size] = 0;

    mk_next_token(iterator);
  }

  return target;
}

static struct mk_rule* mk_parse_rule(struct mk_token_iterator* iterator) {

  struct mk_token_iterator iterator_copy = *iterator;

  struct mk_rule* rule = mk_rule_create();
  if (!rule) {
    return NULL;
  }

  for (;;) {

    char* target = mk_parse_target(iterator);
    if (!target) {
      break;
    }

    if (mk_rule_add_target(rule, target) != 0) {
      mk_rule_destroy(rule);
      *iterator = iterator_copy;
      return NULL;
    }
  }

  return rule;
}

static int mk_parse_any(struct mk_tree* tree, struct mk_token_iterator* iterator) {

  const struct mk_token* token = mk_current_token(iterator);
  switch (token->type) {
    case MK_TOKEN_COMMENT:
    case MK_TOKEN_NEWLINE:
      mk_next_token(iterator);
      return 0;
    default:
      break;
  }

  struct mk_rule* rule = mk_parse_rule(iterator);
  if (rule) {
    return mk_tree_push_rule(tree, rule);
  }

  return -1;
}

int mk_parse(struct mk_state* state,
             const struct mk_token* token_array,
             size_t token_count) {

  struct mk_token_iterator iterator;
  iterator.it = token_array;
  iterator.end = token_array + token_count;

  struct mk_tree* tree = mk_tree_create();
  if (!tree) {
    return -1;
  }

  int err = 0;

  while (!mk_at_end(&iterator)) {
    err = mk_parse_any(tree, &iterator);
    if (err) {
      mk_report(state, "missing ':' separator");
      break;
    }
  }

  if (err) {
    mk_tree_destroy(tree);
    return err;
  }

  err = mk_interpret(state, tree);

  mk_tree_destroy(tree);

  return err;
}
