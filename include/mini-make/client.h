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

#ifndef MINI_MAKE_CLIENT_H
#define MINI_MAKE_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

struct make_buffer;

/** @defgroup make-client Make Client
 * @brief A client connection to the make server.
 * */

/** @brief A client connection to the make server.
 * @ingroup make-client
 * */
struct make_client;

/** @brief Creates an instance of a make client.
 * @param socket_name On Linux systems, this is the
 * path to the unix domain socket. On Windows systems,
 * this is the name of the pipe.
 * @ingroup make-client
 * */
struct make_client *make_client_create(const char *socket_name);

/** @brief Releases all resources allocated by the
 * client.
 * @ingroup make-client
 * */
void make_client_destroy(struct make_client *client);

/** @brief Sends a request to the make server.
 * @param client A make client.
 * @param request The request to send to the server.
 * @returns The reply from the server.
 * @ingroup make-client
 * */
struct make_buffer *make_client_send_request(struct make_client *client,
                                             const struct make_buffer *request);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_CLIENT_H */
