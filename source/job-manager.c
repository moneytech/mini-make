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

#include <mini-make/job-manager.h>

#include <mini-make/error.h>
#include <mini-make/job.h>

#include <stdlib.h>
#include <string.h>

static int add_job(struct make_job_manager *job_manager, const struct make_job *job) {

  struct make_job *tmp;
  size_t tmp_size;

  tmp_size = job_manager->job_count + 1;
  tmp_size *= sizeof(job_manager->job_array[0]);

  tmp = realloc(job_manager->job_array, tmp_size);
  if (tmp == NULL)
    return make_failure;

  job_manager->job_array = tmp;
  job_manager->job_count++;
  job_manager->job_array[job_manager->job_count - 1] = *job;

  return make_success;
}

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

int make_job_manager_wait_for_one(struct make_job_manager *job_manager) {

  int err;
  int exit_code;

  if (job_manager->job_count == 0)
    return make_success;

  err = make_job_wait(&job_manager->job_array[0], &exit_code);
  if (err != make_success)
    return err;
  else if (exit_code != EXIT_SUCCESS)
    return make_failure;

  make_job_free(&job_manager->job_array[0]);

  memmove(&job_manager->job_array[0],
          &job_manager->job_array[1],
          (job_manager->job_count - 1) * sizeof(job_manager->job_array[0]));

  job_manager->job_count--;

  return make_success;
}

int make_job_manager_wait_for_all(struct make_job_manager *job_manager) {

  int err;

  while (job_manager->job_count > 0) {
    err = make_job_manager_wait_for_one(job_manager);
    if (err != make_success)
      return err;
  }

  return make_success;
}

int make_job_manager_queue(struct make_job_manager *job_manager,
                           const struct make_string *cmdline) {

  int err;
  struct make_job job;

  if (job_manager->job_count >= job_manager->job_max) {
    err = make_job_manager_wait_for_one(job_manager);
    if (err)
      return err;
  }

  make_job_init(&job);

  err = make_job_start(&job, cmdline);
  if (err)
    return err;

  err = add_job(job_manager, &job);
  if (err != make_success)
    return err;

  return 0;
}

