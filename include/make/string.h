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

#ifndef MAKE_STRING_H
#define MAKE_STRING_H

/** @brief A simple string
 * structure.
 * */
struct make_string {
  /** @brief Contains the string characters. */
  char *data;
  /** @brief The number of characters in the
   * string. If the string is null-terminated,
   * then this number should NOT include the
   * null-terminating character. */
  unsigned long int size;
  /** @brief The number of characters reserved
   * for the string. This is used to reduce the
   * amount of allocations made by the string.
   * */
  unsigned long int res;
};

/** @brief Initializes internal members of the
 * string. */
void make_string_init(struct make_string *str);

/** @brief Releases all resources allocated by
 * the string.
 * */
void make_string_free(struct make_string *str);

/** @brief Appends the contents of @p src to @p dst. */
int make_string_append(struct make_string *dst,
                       const struct make_string *src);

/** @brief Appends a single character to the end of @p dst. */
int make_string_append_char(struct make_string *dst, char c);

/** @brief Creates a copy of the string. */
int make_string_copy(const struct make_string *src,
                     struct make_string *dst);

/** @brief Checks to see if two strings are
 * equal to one another.
 * @returns One if the strings are equal,
 * zero if they are not.
 * */
int make_string_equal(const struct make_string *a,
                      const struct make_string *b);

/** @brief Prepends the contents of @p src
 * to @p dst. */
int make_string_prepend(struct make_string *dst,
                        const struct make_string *src);

/** @brief Reserves an amount of data so that the
 * string can grow with fewer allocations. */
int make_string_reserve(struct make_string *str,
                        unsigned long int res);

/** @brief Sets the contents of @p dst
 * to that of @p src. */
int make_string_set(struct make_string *dst,
                    const char *src,
                    unsigned long int src_size);

#endif /* MAKE_STRING_H */
