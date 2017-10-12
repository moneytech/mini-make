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

/** @file buffer-io.h Contains functions for
 * sending and receiving buffers over a socket. */

#ifndef MINI_MAKE_POSIX_BUFFER_IO_H
#define MINI_MAKE_POSIX_BUFFER_IO_H

#ifdef __cplusplus
extern "C" {
#endif

struct make_buffer;

/** @brief Sends a buffer over a socket.
 * @param fd The file descriptor for the socket to send
 * the buffer through.
 * @param buffer An initialized buffer. This buffer may
 * be empty, in which case an empty buffer is sent across
 * the socket.
 * @returns @ref make_success if the function succeeds,
 * or @ref make_failure if it does not.
 * */
int make_buffer_send(int fd, const struct make_buffer *buffer);

/** @brief Receives a buffer from a socket.
 * @param fd The file descriptor to receive the buffer
 * from.
 * @param buffer An initialized buffer. Whether or not
 * this function succeeds, this buffer must be freed
 * afterwards.
 * @returns @ref make_success if the function succeeds,
 * or @ref make_failure if it does not.
 * */
int make_buffer_recv(int fd, struct make_buffer *buffer);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_POSIX_BUFFER_IO_H */
