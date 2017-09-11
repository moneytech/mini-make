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

#ifndef MINI_MAKE_INTERPRETER_OBSERVER_H
#define MINI_MAKE_INTERPRETER_OBSERVER_H

#ifdef __cplusplus
extern "C" {
#endif

struct mmk_string;
struct mmk_command;

struct mmk_interpreter_observer {
  /** @brief Data to pass to the observer callbacks. */
  void *data;
  /** @brief Called when a target is determined to not exist. */
  int (*on_target_nonexistent)(void *, const struct mmk_string *target);
  /** @brief Called when a target is discovered to be expired.
   * A target is considered to be expired when a prerequisite is found
   * that is newer than the target. */
  int (*on_target_expired)(void *, const struct mmk_string *target);
};

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_INTERPRETER_OBSERVER_H */
