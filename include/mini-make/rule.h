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

#ifndef MINI_MAKE_RULE_H
#define MINI_MAKE_RULE_H

/** @file */

#include <mini-make/buffer.h>

#ifdef __cplusplus
extern "C" {
#endif

struct make_target;
struct make_prerequisite;
struct make_command;

/** @defgroup make-rule Make Rule
 * @brief A makefile rule.
 * */

/** @brief A makefile rule.
 * Makefile rules contain targets,
 * prerequisites and commands (in that order).
 * @ingroup make-rule
 * */
struct make_rule {
  /** @brief The target buffer. */
  struct make_buffer targets;
  /** @brief The prerequisite buffer. */
  struct make_buffer prerequisites;
  /** @brief The command buffer. */
  struct make_buffer commands;
  /** @brief The number of valid targets in the rule. */
  size_t target_count;
  /** @brief The number of valid prerequisites in the rule. */
  size_t prerequisite_count;
  /** @brief The number of valid commands in the rule. */
  size_t command_count;
};

/** @brief Initializes a rule for use.
 * @ingroup make-rule
 * */
void make_rule_init(struct make_rule *rule);

/** @brief Releases all resources allocated by
 * the rule.
 * @ingroup make-rule
 * */
void make_rule_free(struct make_rule *rule);

/** @brief Adds a target to the rule.
 * @ingroup make-rule
 * */
int make_rule_add_target(struct make_rule *rule,
                         const struct make_target *target);

/** @brief Adds a command to the rule.
 * @ingroup make-rule
 * */
int make_rule_add_command(struct make_rule *rule,
                          const struct make_command *command);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_RULE_H */
