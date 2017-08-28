/* Copyright (C) 2017 Taylor Holberton
 *
 * This file is part of Mini Make.
 *
 * Mini Make is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mini Make is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mini Make.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <make/string.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

void make_string_init(struct make_string *str) {
  str->data = NULL;
  str->size = 0;
  str->res = 0;
}

void make_string_free(struct make_string *str) {
  free(str->data);
  str->data = NULL;
  str->size = 0;
}

int make_string_append_char(struct make_string *dst, char c) {
  struct make_string src;
  src.data = &c;
  src.size = 1;
  src.res = 0;
  return make_string_append(dst, &src);
}

int make_string_append(struct make_string *dst,
                       const struct make_string *src) {

  int err;

  err = make_string_reserve(dst, dst->size + src->size + 1);
  if (err)
    return err;

  memcpy(&dst->data[dst->size], src->data, src->size);
  dst->size += src->size;
  dst->data[dst->size] = 0;

  return 0;
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

int make_string_prepend(struct make_string *dst,
                        const struct make_string *src) {
  int err;

  err = make_string_reserve(dst, dst->size + src->size + 1);
  if (err)
    return err;

  memmove(dst->data + src->size,
          dst->data,
          dst->size);

  memcpy(dst->data,
         src->data,
         src->size);

  dst->size += src->size;

  dst->data[dst->size] = 0;

  return 0;
}

int make_string_prepend_char(struct make_string *dst, char c) {
  struct make_string src;
  src.data = &c;
  src.size = 1;
  src.res = 0;
  return make_string_prepend(dst, &src);
}

int make_string_reserve(struct make_string *dst,
                    unsigned long int res) {

  char *tmp;

  if (res < dst->res)
    /* The string already has at
     * least this much space */
    return 0;

  tmp = realloc(dst->data, res);
  if ((tmp == NULL) && (res > 0))
    return -ENOMEM;

  dst->data = tmp;
  dst->res = res;

  return 0;
}

int make_string_set(struct make_string *dst,
                    const char *src,
                    unsigned long int src_size) {

  int err;

  err = make_string_reserve(dst, src_size + 1);
  if (err)
    return err;

  memcpy(dst->data, src, src_size);
  dst->data[src_size] = 0;
  dst->size = src_size;

  return 0;
}

int make_string_set_asciiz(struct make_string *dst,
                           const char *src) {

  return make_string_set(dst, src, strlen(src));
}

