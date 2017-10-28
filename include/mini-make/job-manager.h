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

#ifndef MINI_MAKE_JOB_MANAGER_H
#define MINI_MAKE_JOB_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

struct make_job;
struct make_string;

/** @defgroup make-job-manager Make Job Manager
 * @brief A job manager for commands executed in
 * a makefile. */

/** @brief A job manager for command executed in
 * a makefile.
 * @ingroup make-job-manager
 * */
struct make_job_manager {
  /** @brief An array of active jobs. */
  struct make_job *job_array;
  /** @brief The number of jobs in the
   * job array. */
  unsigned long int job_count;
  /** @brief The maximum jobs to start
   * before waiting for one to exit. */
  unsigned long int job_max;
};

/** @brief Initializes a job manager.
 * @ingroup make-job-manager
 * */
void make_job_manager_init(struct make_job_manager *job_manager);

/** @brief Releases all resources allocated
 * by the job manager. If there are active jobs
 * in the job array, they are killed. Call @ref make_job_manager_wait_for_all
 * if jobs must finish before calling this function.
 * @ingroup make-job-manager
 * */
void make_job_manager_free(struct make_job_manager *job_manager);

/** @brief Sets the maximum number of jobs to start
 * before waiting for one to exit.
 * @ingroup make-job-manager
 * */
void make_job_manager_set_max_jobs(struct make_job_manager *job_manager,
                                   unsigned long int max);

/** @brief Waits for one job to finish.
 * If there are no jobs running, function exits successfully.
 * @ingroup make-job-manager
 * */
int make_job_manager_wait_for_one(struct make_job_manager *job_manager);

/** @brief Waits for all jobs to finish.
 * If there are no jobs running, function exits successfully.
 * @ingroup make-job-manager
 * */
int make_job_manager_wait_for_all(struct make_job_manager *job_manager);

/** @brief Starts another job within the job manager. If the
 * maximum number of jobs has been reached, the job manager will
 * wait for one to exit before adding the new one to the queue.
 * @ingroup make-job-manager
 * */
int make_job_manager_queue(struct make_job_manager *job_manager,
                           const struct make_string *cmdline);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MINI_MAKE_JOB_MANAGER_H */
