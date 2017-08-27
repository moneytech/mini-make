#ifndef MAKE_JOB_H
#define MAKE_JOB_H

#if defined(_WIN32)
#include <windows.h>
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

#endif /* MAKE_JOB_H */
