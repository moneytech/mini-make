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

#ifndef MINI_MAKE_IHOOKS_H
#define MINI_MAKE_IHOOKS_H

#ifdef __cplusplus
extern "C" {
#endif

struct make_command;
struct make_target;

/** @brief Makefile interpreter hooks. */
struct make_ihooks {
  /** @brief Used for passing custom data to the
   * callback functions. */
  void *data;
  /** @brief Called when the interpreter determines a command should run. */
  int (*on_command)(void *data,
                    const struct make_target *target,
                    const struct make_command *command);
  /** @brief Called when the interpreter begins building a target. */
  int (*on_target)(void *data, const struct make_target *target);
  /** @brief Called when a target has been determined to be expired. */
  int (*on_target_expired)(void *data, const struct make_target *target);
};

/** Initializes all hooks. */
void make_ihooks_init(struct make_ihooks *ihooks);

/** Notifies hooks that the interpreter is building a new target. */
int make_ihooks_notify_target(struct make_ihooks *ihooks,
                              const struct make_target *target);

/** Notifies hooks that the target is expired. */
int make_ihooks_notify_target_expired(struct make_ihooks *ihooks,
                                      const struct make_target *target);

/** Notifies hooks that a command should be run. */
int make_ihooks_notify_command(struct make_ihooks *ihooks,
                               const struct make_target *target,
                               const struct make_command *command);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MINI_MAKE_IHOOKS_H */
