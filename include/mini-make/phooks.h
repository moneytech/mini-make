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

#ifndef MAKE_PHOOKS_H
#define MAKE_PHOOKS_H

#ifdef __cplusplus
extern "C" {
#endif

struct make_assignment_stmt;
struct make_command;
struct make_include_stmt;
struct make_location;
struct make_string;

/** @brief Makefile parser hooks. */
struct make_phooks {
  /** @brief Used for passing custom data to the
   * callback functions. */
  void *data;
  /** @brief Executed when an assignment statement
   * is found. */
  int (*on_assignment_stmt)(void *user_data,
                            const struct make_assignment_stmt *assignment_stmt);
  /** @brief Executed when a comment is found. */
  int (*on_comment)(void *user_data, const struct make_string *comment);
  /** @brief Executed when an include statement
   * is found. */
  int (*on_include_stmt)(void *user_data,
                         const struct make_include_stmt *include_stmt);
  /** @brief Executed right before a rule is parsed. */
  int (*on_rule_start)(void *user_data);
  /** @brief Executed right after a rule is parsed. */
  int (*on_rule_finish)(void *user_data);
  /** @brief Executed when a target is parsed. */
  int (*on_target)(void *user_data,
                   const struct make_string *target);
  /** @brief Executed when a prerequisite is parsed. */
  int (*on_prerequisite)(void *user_data,
                         const struct make_string *prerequisite);
  /** @brief Executed when a command is parsed. */
  int (*on_command)(void *user_data,
                    const struct make_command *command);
  /** @brief Executed when an unexpected character is found. */
  void (*on_unexpected_char)(void *user_data, char c,
                             const struct make_location *location);
  /** @brief Executed when a rule is missing a ':' separator. */
  void (*on_missing_separator)(void *user_data,
                               const struct make_location *location);
  /** @brief Executed when the end of file is reached unexpectedly. */
  void (*on_unexpected_eof)(void *user_data);
};

/** @brief Initializes the phooks with functions
 * that do not do anything. None of the values (except
 * for the data member) are initialized to NULL. */
void make_phooks_init(struct make_phooks *phooks);

/** @brief Notifies the phooks of a start of a rule. */
int make_phooks_notify_rule_start(struct make_phooks *phooks);

/** @brief Notifies the phooks of the end of a rule. */
int make_phooks_notify_rule_finish(struct make_phooks *phooks);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MAKE_PHOOKS_H */
