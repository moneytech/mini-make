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

#include <mini-make/phooks.h>

#include <mini-make/error.h>
#include <mini-make/location.h>
#include <mini-make/string.h>

#include <stdio.h>
#include <string.h>

static int on_assignment_stmt(void *data, const struct make_assignment_stmt *assignment_stmt) {
  (void) data;
  (void) assignment_stmt;
  return make_success;
}

static int on_comment(void *data, const struct make_string *comment) {
  (void) data;
  (void) comment;
  return make_success;
}

static int on_include_stmt(void *data, const struct make_include_stmt *include_stmt) {
  (void) data;
  (void) include_stmt;
  return make_success;
}

static int on_rule_start(void *data) {
  (void) data;
  return make_success;
}

static int on_rule_finish(void *data) {
  (void) data;
  return make_success;
}

static int on_target(void *data, const struct make_string *target) {
  (void) data;
  (void) target;
  return make_success;
}

static int on_prerequisite(void *data, const struct make_string *prerequisite) {
  (void) data;
  (void) prerequisite;
  return make_success;
}

static int on_command(void *data, const struct make_command *command) {
  (void) data;
  (void) command;
  return make_success;
}

static void on_unexpected_char(void *data, char c,
                               const struct make_location *location) {
  (void) data;
  fprintf(stderr, "%.*s:%lu:%lu: Unexpected character '%c'\n",
          (int) location->path.size, location->path.data,
          location->line, location->column, c);
}

static void on_unexpected_eof(void *data) {
  fprintf(stderr, "Unexpected end-of-file reached\n");
  (void) data;
}

static void on_missing_separator(void *data, const struct make_location *location) {
  (void) data;
  fprintf(stderr, "%.*s:%lu:%lu: Missing ':' separator\n",
          (int) location->path.size, location->path.data,
          location->line, location->column);
}

void make_phooks_init(struct make_phooks *phooks) {
  memset(phooks, 0, sizeof(*phooks));
  phooks->on_assignment_stmt = on_assignment_stmt;
  phooks->on_include_stmt = on_include_stmt;
  phooks->on_rule_start = on_rule_start;
  phooks->on_rule_finish = on_rule_finish;
  phooks->on_target = on_target;
  phooks->on_prerequisite = on_prerequisite;
  phooks->on_command = on_command;
  phooks->on_unexpected_char = on_unexpected_char;
  phooks->on_unexpected_eof = on_unexpected_eof;
  phooks->on_missing_separator = on_missing_separator;
}

int make_phooks_notify_rule_start(struct make_phooks *phooks) {
  if (phooks->on_rule_start != NULL)
    return phooks->on_rule_start(phooks->data);
  else
    return make_success;
}

int make_phooks_notify_rule_finish(struct make_phooks *phooks) {
  if (phooks->on_rule_finish)
    return phooks->on_rule_finish(phooks->data);
  else
    return make_success;
}

