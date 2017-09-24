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

#ifndef MINI_MAKE_INTERPRETER_H
#define MINI_MAKE_INTERPRETER_H

#include <mini-make/ihooks.h>
#include <mini-make/job-manager.h>
#include <mini-make/parser.h>
#include <mini-make/string.h>
#include <mini-make/table.h>
#include <mini-make/target.h>

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

struct make_interpreter {
  /** Interpreter hooks. */
  struct make_ihooks hooks;
  /** Contains processes started from
   * recipes. */
  struct make_job_manager job_manager;
  /** @brief The target that the interpreter
   * is building. */
  struct make_target target;
  struct make_parser parser;
  /** @brief Stores all the variables
   * from the assignment statements. */
  struct make_table table;
  /** @brief If set, the interpreter
   * does not print any of the commands
   * it is running. */
  int silent;
  /** @brief Print the commands as
   * normal but do not run them. */
  int just_print;
  /** @brief Set to one if the last
   * target parsed was .PHONY */
  int phony_found;
  /** @brief Set to one if the target
   * was found when parsing a rule. */
  int target_found;
  /** @brief Set to one if the target
   * was found when parsing a makefile. */
  int target_found_once;
  /** @brief Set to one if the target
   * exists. */
  int target_exists;
  /** @brief Set to one if the target is
   * older than it's prerequisites. */
  int target_expired;
  /** @brief Target modification time. */
  long long int target_mtime;
  /** @brief Logs error messages. */
  FILE *errlog;
  /** @brief Logs non-error messages. */
  FILE *outlog;
};

void make_interpreter_init(struct make_interpreter *interpreter);

void make_interpreter_free(struct make_interpreter *interpreter);

int make_interpreter_define(struct make_interpreter *interpreter,
                            const struct make_string *key,
                            const struct make_string *value);

int make_interpreter_has_target(const struct make_interpreter *interpreter);

int make_interpreter_read(struct make_interpreter *interpreter,
                          const char *filename);

int make_interpreter_run(struct make_interpreter *interpreter);

void make_interpreter_set_hooks(struct make_interpreter *interpreter,
                                const struct make_ihooks *hooks);

int make_interpreter_set_target(struct make_interpreter *interpreter,
                                const struct make_string *target);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MINI_MAKE_INTERPRETER_H */
