#ifndef mk_parse_h
#define mk_parse_h

#include <stddef.h>

struct mk_state;
struct mk_token;

/** Parses a set of tokens for assignment statements,
 * include statements, conditional statements, and nodes.
 * @param state The interpreter state.
 * @param token_array The array of tokens to parse.
 * @param token_count The number of tokens in @p token_array.
 * @returns Zero on success, non-zero on failure.
 * @ingroup mk_internal
 * */
int mk_parse(struct mk_state* state,
             const struct mk_token* token_array,
             size_t token_count);

#endif /* mk_parse_h */
