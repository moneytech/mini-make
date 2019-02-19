#ifndef mk_scan_h
#define mk_scan_h

#include <stddef.h>

struct mk_state;

/** Scans the source for tokens,
 * passing them to @ref mk_parse.
 * @param state The interpreter state.
 * @param data The character data to scan.
 * @param size The number of characters in @ref data.
 * @returns Zero on success, non-zero on failure.
 * @ingroup mk_internal
 * */
int mk_scan(struct mk_state* state, const char* data, size_t size);

#endif /* mk_scan_h */
