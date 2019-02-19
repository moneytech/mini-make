#ifndef mk_scan_h
#define mk_scan_h

#include <stddef.h>

struct mk_state;

int mk_scan(struct mk_state* state, const char* data, size_t size);

#endif /* mk_scan_h */
