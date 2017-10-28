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

#ifndef MINI_MAKE_THREAD_H
#define MINI_MAKE_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup make-thread Make Thread
 * @brief A cross-platform thread wrapper.
 * */

/** @brief A cross-platform thread wrapper.
 * @ingroup make-thread
 * */
struct make_thread;

struct make_thread_callback;

/** @brief Creates a thread, using the function pointer
 * and data in @p callback.
 * @returns If this function fails, a null pointer is returned.
 * @ingroup make-thread
 * */
struct make_thread *make_thread_create(struct make_thread_callback *callback);

/** @brief Destroys a thread. If the thread is still running,
 * it is canceled and the memory is deallocated.
 * @ingroup make-thread
 * */
void make_thread_destroy(struct make_thread *thread);

/** @brief Waits for the thread to exit. If this function
 * exits successfully, @ref make_thread_destroy does not
 * need to be called.
 * @ingroup make-thread
 * */
int make_thread_join(struct make_thread *thread, void **result);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_THREAD_H */
