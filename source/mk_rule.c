#include "mk_rule.h"

#include "mk_strlist.h"

#include <stdlib.h>

/** This structure contains data related to a rule.
 * @ingroup mk_internal
 * */
struct mk_rule
{
  /** The targets part of this rule. */
  struct mk_strlist* targets;
  /** The dependencies required by this rule. */
  struct mk_strlist* deps;
  /** The list of commands required to build the targets. */
  struct mk_strlist* recipe;
};

struct mk_rule* mk_rule_create() {

  struct mk_rule* rule = malloc(sizeof(struct mk_rule));
  if (!rule) {
    return NULL;
  }

  rule->targets = mk_strlist_create();
  rule->deps = mk_strlist_create();
  rule->recipe = mk_strlist_create();

  if (!rule->targets || !rule->deps || !rule->recipe) {
    mk_rule_destroy(rule);
    return NULL;
  }

  return rule;
}

void mk_rule_destroy(struct mk_rule* rule) {
  mk_strlist_destroy(rule->targets);
  mk_strlist_destroy(rule->deps);
  mk_strlist_destroy(rule->recipe);
  free(rule);
}

int mk_rule_add_target(struct mk_rule* rule, char* target) {
  return mk_strlist_push(rule->targets, target);
}
