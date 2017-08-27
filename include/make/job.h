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

#ifndef MAKE_JOB_H
#define MAKE_JOB_H

#if defined(_WIN32)
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct make_string;

struct make_job {
#if defined(_WIN32)
  PROCESS_INFORMATION process_info;
  STARTUPINFO startup_info;
#elif defined(__unix__)
  int fd;
#endif
};

void make_job_init(struct make_job *job);

void make_job_free(struct make_job *job);

int make_job_start(struct make_job *job,
                   const struct make_string *cmdline);

int make_job_wait(struct make_job *job,
                  int *exit_code);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MAKE_JOB_H */
