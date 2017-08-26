#include <make/string.h>

int make_string_equal(const struct make_string *a,
                      const struct make_string *b) {
  if (a->size != b->size)
    return 0;
  else if (memcmp(a->data, b->data, a->size) != 0)
    return 0;
  else
    return 1;
}

