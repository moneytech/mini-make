#ifndef MAKE_JOB_MANAGER_H
#define MAKE_JOB_MANAGER_H

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

#endif /* MAKE_JOB_MANAGER_H */
