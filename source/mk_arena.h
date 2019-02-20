#ifndef mk_arena_h
#define mk_arena_h

#include <stddef.h>

struct mk_arena;

struct mk_arena* mk_arena_create(struct mk_arena* parent);

void mk_arena_destroy(struct mk_arena* arena);

void* mk_arena_malloc(struct mk_arena* arena, size_t size);

void* mk_arena_realloc(struct mk_arena* arena, void* addr, size_t size);

void mk_arena_free(struct mk_arena* arena, void* addr);

#endif /* mk_arena_h */
