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

#ifndef MINI_MAKE_SERVER_H
#define MINI_MAKE_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

struct make_buffer;
struct make_server;

typedef int (*make_server_callback)(void *data,
                                    const struct make_buffer *request,
                                    struct make_buffer *reply);

struct make_server *make_server_create(const char *socket_name);

void make_server_destroy(struct make_server *server);

void make_server_set_callback(struct make_server *server,
                              make_server_callback calback);

void make_server_set_callback_data(struct make_server *server,
                                   void *callback_data);

int make_server_discover_clients(struct make_server *server);

int make_server_iterate_clients(struct make_server *server);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_SERVER_H */
