#ifndef mk_malloc_h
#define mk_malloc_h

#include <stddef.h>

struct mk_state;

void* mk_malloc(struct mk_state* state, size_t size);

void* mk_realloc(struct mk_state* state, void* addr, size_t size);

void mk_free(struct mk_state* state, void* addr);

#endif /* mk_malloc_h */
