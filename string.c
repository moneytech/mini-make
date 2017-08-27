#include <make/string.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

void make_string_init(struct make_string *str) {
  str->data = NULL;
  str->size = 0;
}

void make_string_free(struct make_string *str) {
  free(str->data);
  str->data = NULL;
  str->size = 0;
}

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

int make_string_res(struct make_string *dst,
                    unsigned long int res) {

  char *tmp;

  if (res < dst->res)
    /* The string already has at
     * least this much space */
    return 0;

  tmp = realloc(dst->data, res);
  if (tmp == NULL)
    return -ENOMEM;

  dst->data = tmp;
  dst->res = res;

  return 0;
}

int make_string_set(struct make_string *dst,
                    const char *src,
                    unsigned long int src_size) {

  int err;

  err = make_string_res(dst, src_size);
  if (err)
    return err;

  memcpy(dst->data, src, src_size);
  dst->data[src_size] = 0;
  dst->size = src_size;
  return 0;
}

