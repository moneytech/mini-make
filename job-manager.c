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

#include <make/job-manager.h>

#include <make/job.h>

#include <stdlib.h>

void make_job_manager_init(struct make_job_manager *job_manager) {
  job_manager->job_array = NULL;
  job_manager->job_count = 0;
  job_manager->job_max = 1;
}

void make_job_manager_free(struct make_job_manager *job_manager) {
  unsigned long int i;
  struct make_job *job;
  for (i = 0; i < job_manager->job_count; i++) {
    job = &job_manager->job_array[i];
    make_job_free(job);
  }
  free(job_manager->job_array);
  job_manager->job_array = NULL;
  job_manager->job_count = 0;
}

int make_job_manager_queue(struct make_job_manager *job_manager,
                           const struct make_string *cmdline) {

  int err;
  int exit_code;
  struct make_job job;

  if (job_manager->job_count >= job_manager->job_max) {
    /* TODO */
  }

  make_job_init(&job);

  err = make_job_start(&job, cmdline);
  if (err)
    return err;

  err = make_job_wait(&job, &exit_code);
  if (err)
    return err;
  else if (exit_code != 0)
    return exit_code;

  make_job_free(&job);

  return 0;
}

