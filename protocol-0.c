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

#include "protocol-0.h"

#include <mini-make/error.h>
#include <mini-make/protocol.h>
#include <mini-make/job-manager.h>

#include <stdlib.h>

struct protocol_0_data {
  struct make_job_manager job_manager;
};

static void protocol_0_done(void *data_ptr) {

  struct protocol_0_data *data;

  data = (struct protocol_0_data *) data_ptr;

  make_job_manager_free(&data->job_manager);

  free(data);
}

static int protocol_0_request(void *data_ptr,
                              const struct make_buffer *request) {
  (void) data_ptr;
  (void) request;
  return make_success;
}

static int protocol_0_reply(void *data_ptr,
                            struct make_buffer *reply) {
  (void) data_ptr;
  (void) reply;
  return make_success;
}

int make_protocol_0(struct make_protocol *protocol) {

  struct protocol_0_data *data;

  data = malloc(sizeof(*data));
  if (data == NULL)
    return make_failure;

  protocol->data = data;
  protocol->done = protocol_0_done;
  protocol->request = protocol_0_request;
  protocol->reply = protocol_0_reply;

  return make_success;
}

