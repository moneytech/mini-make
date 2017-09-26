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

#include <mini-make/protocol.h>

#include <mini-make/error.h>

#include <stdlib.h>

#include "protocol-0.h"

void make_protocol_init(struct make_protocol *protocol) {
  protocol->data = NULL;
  protocol->done = NULL;
  protocol->request = NULL;
  protocol->reply = NULL;
}

void make_protocol_done(struct make_protocol *protocol) {
  if (protocol->done != NULL)
    protocol->done(protocol->data);
}

int make_protocol_request(struct make_protocol *protocol,
                          const struct make_buffer *request) {

  if (protocol->request == NULL)
    return make_failure;
  else
    return protocol->request(protocol->data, request);
}

int make_protocol_reply(struct make_protocol *protocol,
                        struct make_buffer *reply) {

  if (protocol->reply == NULL)
    return make_failure;
  else
    return protocol->reply(protocol->data, reply);
}

int make_protocol_set_version(struct make_protocol *protocol,
                              unsigned int version) {
  switch (version) {
    case 0:
      return make_protocol_0(protocol);
    default:
      return make_failure;
  }
  return make_success;
}

