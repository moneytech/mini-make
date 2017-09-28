#include <mini-make/client.h>

#include <mini-make/buffer.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

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

  err = bind(client->socket_fd,
             (struct sockaddr *) &address,
             sizeof(struct sockaddr_un));
  if (err != 0) {
    make_client_destroy(client);
    return NULL;
  }

  err = listen(client->socket_fd, 32);
  if (err != 0) {
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

  struct make_buffer *reply;

  (void) client;
  (void) request;

  reply = malloc(sizeof(*reply));
  if (reply == NULL)
    return NULL;

  make_buffer_init(reply);

  return reply;
}

