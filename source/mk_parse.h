#ifndef mk_parse_h
#define mk_parse_h

#include <stddef.h>

struct mk_state;
struct mk_token;

int mk_parse(struct mk_state* state,
             const struct mk_token* token_array,
             size_t token_count);

#endif /* mk_parse_h */
