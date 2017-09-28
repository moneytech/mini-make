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

#ifndef MINI_MAKE_CLIENT_H
#define MINI_MAKE_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

struct make_buffer;
struct make_client;

struct make_client *make_client_create(const char *socket_name);

void make_client_destroy(struct make_client *client);

struct make_buffer *make_client_send_request(struct make_client *client,
                                             const struct make_buffer *buffer);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_CLIENT_H */
