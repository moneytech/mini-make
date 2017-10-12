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

#include "buffer-io.h"

#include <mini-make/buffer.h>
#include <mini-make/error.h>

#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>

#include <errno.h>
#include <stdint.h>

int make_buffer_send(int fd, const struct make_buffer *buffer) {

  if (buffer->size > INT32_MAX)
    return make_failure;

  uint32_t buffer_size = buffer->size;

  buffer_size = htonl(buffer_size);

  fcntl(fd, F_SETFL, O_NONBLOCK);

  ssize_t write_count = write(fd, &buffer_size, sizeof(buffer_size));
  if (write_count < 0)
    return make_failure;
  else if (((uint32_t) write_count) != sizeof(buffer_size))
    return make_failure;

  fcntl(fd, F_SETFL, 0);

  write_count = write(fd, buffer->data, buffer->size);
  if (write_count < 0)
    return make_failure;
  else if (((size_t) write_count) != buffer->size)
    return make_failure;

  return make_success;
}

int make_buffer_recv(int fd, struct make_buffer *buffer) {

  uint32_t buffer_size;

  ssize_t read_count = read(fd, &buffer_size, sizeof(buffer_size));
  if (read_count < 0) {
    if (errno == EAGAIN)
      return make_success;
  } else if (read_count != sizeof(buffer_size)) {
    return make_failure;
  }

  buffer_size = ntohl(buffer_size);

  int err = make_buffer_resize(buffer, buffer_size);
  if (err != make_success)
    return err;

  fcntl(fd, F_SETFL, 0);

  read_count = read(fd, buffer->data, buffer->size);
  if (read_count != ((ssize_t) buffer->size))
    return make_failure;

  fcntl(fd, F_SETFL, O_NONBLOCK);

  return make_success;
}

