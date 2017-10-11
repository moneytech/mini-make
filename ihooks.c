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

#include <mini-make/ihooks.h>

#include <mini-make/error.h>

#include <stdlib.h>

void make_ihooks_init(struct make_ihooks *ihooks) {
  ihooks->data = NULL;
  ihooks->on_target = NULL;
  ihooks->on_target_expired = NULL;
  ihooks->on_command = NULL;
}

int make_ihooks_notify_target(struct make_ihooks *ihooks,
                              const struct make_target *target) {
  if (ihooks->on_target != NULL)
    return ihooks->on_target(ihooks->data, target);
  else
    return make_success;
}

int make_ihooks_notify_target_expired(struct make_ihooks *ihooks,
                                      const struct make_target *target) {
  if (ihooks->on_target != NULL)
    return ihooks->on_target_expired(ihooks->data, target);
  else
    return make_success;
}

int make_ihooks_notify_command(struct make_ihooks *ihooks,
                               const struct make_target *target,
                               const struct make_command *command) {
  if (ihooks->on_command != NULL)
    return ihooks->on_command(ihooks->data, target, command);
  else
    return make_success;
}

