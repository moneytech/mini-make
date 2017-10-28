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

#include <mini-make/server.h>

#include <mini-make/buffer.h>
#include <mini-make/error.h>
#include <mini-make/string.h>

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "buffer-io.h"

struct client_list {
  int *client_array;
  size_t client_count;
};

static void client_list_init(struct client_list *list) {
  list->client_array = NULL;
  list->client_count = 0;
}

static void client_list_free(struct client_list *list) {
  for (size_t i = 0; i < list->client_count; i++) {
    int fd = list->client_array[i];
    if (fd >= 0)
      close(fd);
  }
  free(list->client_array);
  list->client_array = NULL;
  list->client_count = 0;
}

static int client_list_add(struct client_list *list, int fd) {

  int *tmp;
  size_t tmp_size;

  tmp_size = list->client_count + 1;
  tmp_size *= sizeof(list->client_array[0]);

  tmp = realloc(list->client_array, tmp_size);
  if (tmp == NULL)
    return make_failure;

  list->client_array = tmp;
  list->client_count++;
  list->client_array[list->client_count - 1] = fd;

  return make_success;
}

struct make_server {
  struct make_string socket_path;
  struct client_list client_list;
  int socket_fd;
  void *callback_data;
  make_server_callback callback;
};

struct make_server *make_server_create(const char *socket_path) {

  int err;
  struct make_server *server;
  struct sockaddr_un address;

  server = malloc(sizeof(*server));
  if (server == NULL)
    return NULL;

  client_list_init(&server->client_list);
  make_string_init(&server->socket_path);
  make_string_set_asciiz(&server->socket_path, socket_path);
  server->socket_fd = - 1;
  server->callback = NULL;
  server->callback_data = NULL;

  server->socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (server->socket_fd < 0) {
    make_server_destroy(server);
    return NULL;
  }

  fcntl(server->socket_fd, F_SETFL, O_NONBLOCK);

  memset(&address, 0, sizeof(address));

  address.sun_family = AF_UNIX;
  snprintf(address.sun_path, sizeof(address.sun_path) - 1, "%s", socket_path);

  err = bind(server->socket_fd,
             (struct sockaddr *) &address,
             sizeof(struct sockaddr_un));
  if (err < 0) {
    make_server_destroy(server);
    return NULL;
  }

  err = listen(server->socket_fd, 32);
  if (err < 0) {
    make_server_destroy(server);
    return NULL;
  }

  return server;
}

void make_server_destroy(struct make_server *server) {
  if (server != NULL) {
    client_list_free(&server->client_list);
    unlink(server->socket_path.data);
    make_string_free(&server->socket_path);
    close(server->socket_fd);
    free(server);
  }
}

static int make_server_handle_client(struct make_server *server, int client_fd) {

  struct make_buffer request;
  struct make_buffer reply;

  (void) client_fd;

  if (server->callback == NULL)
    return make_failure;

  make_buffer_init(&request);
  make_buffer_init(&reply);

  int err = make_buffer_recv(client_fd, &request);
  if (err != make_success) {
    make_buffer_free(&request);
    make_buffer_free(&reply);
    return err;
  }

  err = server->callback(server->callback_data,
                         &request, &reply);
  if (err != make_success) {
    make_buffer_free(&request);
    make_buffer_free(&reply);
    return err;
  }

  make_buffer_free(&request);
  make_buffer_free(&reply);
  return make_success;
}

int make_server_discover_clients(struct make_server *server) {

  for (;;) {
    int fd = accept(server->socket_fd, NULL, NULL);
    if (fd < 0)
      break;

    int err = client_list_add(&server->client_list, fd);
    if (err != make_success)
      break;
  }

  return make_success;
}

int make_server_iterate_clients(struct make_server *server) {
  for (size_t i = 0; i < server->client_list.client_count; i++) {
    int fd = server->client_list.client_array[i];
    int err = make_server_handle_client(server, fd);
    if (err != make_success)
      return err;
  }
  return make_success;
}

void make_server_set_callback(struct make_server *server,
                              make_server_callback callback) {
  server->callback = callback;
}

void make_server_set_callback_data(struct make_server *server,
                                   void *callback_data) {
  server->callback_data = callback_data;
}
