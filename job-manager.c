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

