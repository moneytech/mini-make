#include "mk_malloc.h"

#include "mk_report.h"

#include <stdlib.h>

void* mk_malloc(struct mk_state* state, size_t size) {

  void* addr = malloc(size);
  if (!addr && state) {
    mk_report(state, "failed to allocate %lu bytes", (unsigned long int) size);
  }

  return addr;
}

void* mk_realloc(struct mk_state* state, void* addr, size_t size) {

  addr = realloc(addr, size);
  if (!addr && (size > 0) && state) {
    mk_report(state, "failed to reallocate %lu bytes", (unsigned long int) size);
  }

  return addr;
}

void mk_free(struct mk_state* state, void* addr) {
  (void)state;
  free(addr);
}
