#ifndef mk_rule_h
#define mk_rule_h

/** This structure contains data related to a rule.
 * @ingroup mk_internal
 * */
struct mk_rule
{
  /** The targets part of this rule. */
  char** targets;
  /** The dependencies required by this rule. */
  char** deps;
  /** The list of commands required to build the targets. */
  char** recipe;
};

#endif /* mk_rule_h */
