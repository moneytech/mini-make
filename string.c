#include <make/string.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

int make_string_copy(const struct make_string *src,
                     struct make_string *dst) {
  /* allocate an extra character for
   * a null-terminator */
  dst->data = malloc(src->size + 1);
  if (dst->data == NULL)
    return -ENOMEM;
  memcpy(dst->data, src->data, src->size);
  dst->data[src->size] = 0;
  dst->size = src->size;
  return 0;
}

int make_string_equal(const struct make_string *a,
                      const struct make_string *b) {
  if (a->size != b->size)
    return 0;
  else if (memcmp(a->data, b->data, a->size) != 0)
    return 0;
  else
    return 1;
}

