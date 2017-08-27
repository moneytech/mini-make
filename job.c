#include <make/job.h>

#include <make/string.h>

#include <errno.h>
#include <stdlib.h>

#if defined(__unix__)
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

void make_job_init(struct make_job *job) {
#if defined(_WIN32)
  memset(job, 0, sizeof(*job));
#elif defined(__unix__)
  job->fd = -1;
#endif
}

void make_job_free(struct make_job *job) {
  (void) job;
}

int make_job_start(struct make_job *job,
                   const struct make_string *cmdline) {

  int err;
  struct make_string cmdline_copy;
#ifdef __unix__
  char *argv[4];
#endif

#if defined(__unix__)
  job->fd = fork();
  if (job->fd < 0)
    return -errno;
  else if (job->fd > 0)
    return 0;

  err = make_string_copy(cmdline, &cmdline_copy);
  if (err)
    return err;

  argv[0] = "bash";
  argv[1] = "-c";
  argv[2] = cmdline_copy.data;
  argv[3] = NULL;
  execvp(argv[0], argv);
  exit(EXIT_FAILURE);
#else
  (void) job;
  (void) cmdline;
#endif

  return 0;
}

int make_job_wait(struct make_job *job, int *exit_code) {

#if defined(__unix__)
  int status;
#endif

#if defined(__unix__)
  if (job->fd < 0)
    return -EINVAL;
  else if (waitpid(job->fd, &status, 0) != job->fd)
    return -EINVAL;
  else if (!WIFEXITED(status))
    return -EINVAL;
  else if (exit_code != NULL)
    *exit_code = WEXITSTATUS(status);
#else
  (void) job;
#endif

  return 0;
}

