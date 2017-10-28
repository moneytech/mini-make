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

/** @file */

#ifndef MINI_MAKE_PROTOCOL_H
#define MINI_MAKE_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

struct make_buffer;

/** @defgroup make-protocol Make Protocol
 * @brief Protocol used to communicate between client
 * and server.
 * */

/** @brief Protocal used to communicate between client
 * and server.
 * @ingroup make protocol
 * */
struct make_protocol {
  /** @brief Protocol implementation data. */
  void *data;
  /** @brief Releases resources allocated by protocol. */
  void (*done)(void *protocol_data);
  /** @brief The callback function that receives the request. */
  int (*request)(void *protocol_data,
                 const struct make_buffer *buffer);
  /** @brief The callback function the replies to the last request. */
  int (*reply)(void *protocol_data,
               struct make_buffer *buffer);
};

/** @brief Initializes a protocol.
 * @ingroup make-protocol
 * */
void make_protocol_init(struct make_protocol *protocol);

/** @brief Releases all resources allocated by
 * the protocol.
 * @ingroup make-protocol
 * */
void make_protocol_done(struct make_protocol *protocol);

/** @brief Handles a request.
 * @ingroup make-protocol
 * */
int make_protocol_request(struct make_protocol *protocol,
                          const struct make_buffer *buffer);

/** @brief Sends a reply for the last received request.
 * @ingroup make-protocol
 * */
int make_protocol_reply(struct make_protocol *protocol,
                        struct make_buffer *buffer);

/** @brief Sets the version of the protocol
 * to use.
 * @ingroup make-protocol
 * */
int make_protocol_set_version(struct make_protocol *protocol,
                              unsigned int version);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_PROTOCOL_H */
