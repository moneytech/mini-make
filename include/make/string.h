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
};

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

#endif /* MAKE_STRING_H */
