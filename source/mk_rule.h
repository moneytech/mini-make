#ifndef mk_rule_h
#define mk_rule_h

struct mk_rule;

/** Creates a new rule instance.
 * @returns A pointer to a new rule.
 * @ingroup mk_internals
 * */
struct mk_rule* mk_rule_create();

/** Releases memory allocated by a rule.
 * @param rule The rule to release the memory for.
 * @ingroup mk_internals
 * */
void mk_rule_destroy(struct mk_rule* rule);

/** Adds a target to the rule.
 * @param rule The rule to add the target for.
 * @param target The target to add to the rule.
 * After calling this function, the rule takes ownership of @p target.
 * @returns Zero on success, non-zero on failure.
 * @ingroup mk_internals
 * */
int mk_rule_add_target(struct mk_rule* rule, char* target);

/** Adds a dependency to the rule.
 * @param rule The rule to add the dependency for.
 * @param dep The dependency to add to the rule.
 * After calling this function, the rule takes ownership of @p dep.
 * @returns Zero on success, non-zero on failure.
 * @ingroup mk_internals
 * */
int mk_rule_add_dep(struct mk_rule* rule, char* dep);

#endif /* mk_rule_h */
