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

#include <mini-make/client.h>

#include <mini-make/buffer.h>
#include <mini-make/error.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include "buffer-io.h"

struct make_client {
  int socket_fd;
};

struct make_client *make_client_create(const char *socket_path) {

  int err;
  struct make_client *client;
  struct sockaddr_un address;

  client = malloc(sizeof(*client));
  if (client == NULL)
    return NULL;

  client->socket_fd = - 1;

  client->socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
  if (client->socket_fd < 0) {
    make_client_destroy(client);
    return NULL;
  }

  memset(&address, 0, sizeof(address));

  address.sun_family = AF_UNIX;
  snprintf(address.sun_path, sizeof(address.sun_path) - 1, "%s", socket_path);

  err = connect(client->socket_fd,
                (struct sockaddr *) &address,
                sizeof(address));
  if (err == -1) {
    make_client_destroy(client);
    return NULL;
  }

  return client;
}

void make_client_destroy(struct make_client *client) {
  if (client != NULL) {
    close(client->socket_fd);
    free(client);
  }
}

struct make_buffer *make_client_send_request(struct make_client *client,
                                             const struct make_buffer *request) {

  int err = make_buffer_send(client->socket_fd, request);
  if (err != make_success)
    return NULL;

  struct make_buffer *reply;

  reply = malloc(sizeof(*reply));
  if (reply == NULL)
    return NULL;

  make_buffer_init(reply);

  return reply;
}

