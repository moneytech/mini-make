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

#ifndef MAKE_JOB_MANAGER_H
#define MAKE_JOB_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

struct make_job;
struct make_string;

struct make_job_manager {
  struct make_job *job_array;
  unsigned long int job_count;
  unsigned long int job_max;
};

void make_job_manager_init(struct make_job_manager *job_manager);

void make_job_manager_free(struct make_job_manager *job_manager);

void make_job_manager_set_max_jobs(struct make_job_manager *job_manager,
                                   unsigned long int max);

int make_job_manager_queue(struct make_job_manager *job_manager,
                           const struct make_string *cmdline);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MAKE_JOB_MANAGER_H */
