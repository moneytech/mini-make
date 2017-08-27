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
