#ifndef MAKE_STAT_H
#define MAKE_STAT_H

struct make_string;

/** @brief Check if a file exists or not.
 * @returns One if the file exists, zero if
 * it does not.
 * */
int make_exists(const struct make_string *path);

/** @brief Checks if @p path_a is newer than
 * @p path_b by comparing modification times.
 * */
int make_newer_than(const struct make_string *path_a,
                    const struct make_string *path_b);

#endif /* MAKE_STAT_H */
