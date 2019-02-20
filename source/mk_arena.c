#include "mk_arena.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/** This is the number of sections
 * in a single arena.
 * */
#define MK_ARENA_SECTION_COUNT 64

/** The arena is made up of several sections.
 * This is the size, in bytes, of a single section.
 * */
#define MK_ARENA_SECTION_SIZE 64

/** Contains data used by arenas.
 * */
struct mk_arena {
  /** The arena that this one was allocated in. May be null. */
  struct mk_arena* parent;
  /** The bitmask indicating which sections are allocated. */
  uint64_t mask;
  /** The arena data. */
  uint8_t data[MK_ARENA_SECTION_SIZE * MK_ARENA_SECTION_COUNT];
};

struct mk_arena* mk_arena_create(struct mk_arena* parent) {

  struct mk_arena* arena = mk_arena_malloc(parent, sizeof(*arena));
  if (!arena) {
    return NULL;
  }

  arena->parent = parent;
  arena->mask = 0;

  return arena;
}

void mk_arena_destroy(struct mk_arena* arena) {
  mk_arena_free(arena->parent, arena);
}

void* mk_arena_malloc(struct mk_arena* arena, size_t size) {

  if ((arena == NULL) || (size > MK_ARENA_SECTION_SIZE) || (arena->mask == UINT64_MAX)) {
    return malloc(size);
  } else if (size == 0) {
    return NULL;
  }

  for (size_t i = 0; i < MK_ARENA_SECTION_COUNT; i++) {
    if (!((1 << i) & arena->mask)) {
      arena->mask |= (1 << i);
      return &arena->data[i * MK_ARENA_SECTION_SIZE];
    }
  }

  return malloc(size);
}

void* mk_arena_realloc(struct mk_arena* arena, void* addr, size_t size) {

  if (arena == NULL) {
    return realloc(addr, size);
  } else if (addr == NULL) {
    return mk_arena_malloc(arena, size);
  }

  size_t i = 0;

  while (i < MK_ARENA_SECTION_COUNT) {
    if (addr == &arena->data[i * MK_ARENA_SECTION_SIZE]) {
      break;
    }
    i++;
  }

  if (i >= MK_ARENA_SECTION_COUNT) {
    return realloc(addr, size);
  }

  if (size <= MK_ARENA_SECTION_SIZE) {
    /* The memory can still fit in the section. */
    return addr;
  }

  /* At this point, the memory has to be
   * moved to outside the arena. */

  void* tmp = malloc(size);
  if (tmp == NULL) {
    return NULL;
  }

  /* Copy over data */
  memcpy(tmp, addr, MK_ARENA_SECTION_SIZE);

  /* Mark previous entry as free */
  arena->mask &= ~(1 << i);

  return tmp;
}

void mk_arena_free(struct mk_arena* arena, void* addr) {

  if (arena == NULL) {
    free(addr);
    return;
  }

  for (size_t i = 0; i < MK_ARENA_SECTION_COUNT; i++) {
    if (addr == &arena->data[i * MK_ARENA_SECTION_SIZE]) {
      arena->mask &= ~(1 << i);
      return;
    }
  }

  free(addr);
}
