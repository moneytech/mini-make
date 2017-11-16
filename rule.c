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

#include <mini-make/rule.h>

#include <mini-make/error.h>
#include <mini-make/target.h>
#include <mini-make/prerequisite.h>
#include <mini-make/command.h>

void make_rule_init(struct make_rule *rule) {

  make_buffer_init(&rule->targets);
  make_buffer_set_element_size(&rule->targets, sizeof(struct make_target));
  rule->target_count = 0;

  make_buffer_init(&rule->prerequisites);
  make_buffer_set_element_size(&rule->prerequisites, sizeof(struct make_prerequisite));
  rule->prerequisite_count = 0;

  make_buffer_init(&rule->commands);
  make_buffer_set_element_size(&rule->commands, sizeof(struct make_command));
  rule->command_count = 0;
}

void make_rule_free(struct make_rule *rule) {
  make_buffer_free(&rule->targets);
  make_buffer_free(&rule->prerequisites);
  make_buffer_free(&rule->commands);
  rule->target_count = 0;
  rule->prerequisite_count = 0;
  rule->command_count = 0;
}

int make_rule_add_target(struct make_rule *rule,
                         const struct make_target *target) {
  (void) rule;
  (void) target;
  return make_failure;
}

int make_rule_add_prerequisite(struct make_rule *rule,
                               const struct make_prerequisite *prequisite) {
  (void) rule;
  (void) prerequisite;
  return make_failure;
}

int make_rule_add_command(struct make_rule *rule,
                          const struct make_command *command) {
  (void) rule;
  (void) command;
  return make_failure;
}
