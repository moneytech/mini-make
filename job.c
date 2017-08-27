#include <make/job.h>

#include <make/string.h>

#include <errno.h>
#include <stdlib.h>

#include <stdio.h>

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
  job->process_info.hProcess = INVALID_HANDLE_VALUE;
  job->process_info.hThread = INVALID_HANDLE_VALUE;
#elif defined(__unix__)
  job->fd = -1;
#endif
}

void make_job_free(struct make_job *job) {
#if defined(_WIN32)
  if (job->process_info.hProcess != INVALID_HANDLE_VALUE) {
    CloseHandle(job->process_info.hProcess);
    job->process_info.hProcess = INVALID_HANDLE_VALUE;
  }
  if (job->process_info.hThread != INVALID_HANDLE_VALUE) {
    CloseHandle(job->process_info.hThread);
    job->process_info.hThread = INVALID_HANDLE_VALUE;
  }
#else
  (void) job;
#endif
}

int make_job_start(struct make_job *job,
                   const struct make_string *cmdline) {

  int err;
  struct make_string cmdline_copy;
#if defined(__unix__)
  char *argv[4];
#elif defined(_WIN32)
  char powershell_data[] = "powershell -NoLogo -NonInteractive -Command ";
  struct make_string powershell;
#endif

  make_string_init(&cmdline_copy);

#if defined(__unix__)
  job->fd = fork();
  if (job->fd < 0)
    return -errno;
  else if (job->fd > 0)
    return 0;

  err = make_string_copy(cmdline, &cmdline_copy);
  if (err) {
    make_string_free(&cmdline_copy);
    return err;
  }

  argv[0] = "bash";
  argv[1] = "-c";
  argv[2] = cmdline_copy.data;
  argv[3] = NULL;
  execvp(argv[0], argv);
  exit(EXIT_FAILURE);
#elif defined(_WIN32)
  err = make_string_copy(cmdline, &cmdline_copy);
  if (err) {
    make_string_free(&cmdline_copy);
    return err;
  }
  powershell.data = powershell_data;
  powershell.size = sizeof(powershell_data) - 1;
  powershell.res = 0;
  err = make_string_prepend(&cmdline_copy, &powershell);
  if (err) {
    make_string_free(&cmdline_copy);
    return err;
  }
  if (!CreateProcess(NULL /* application name */,
      cmdline_copy.data,
      NULL /* process security attributes */,
      NULL /* thread security attributes */,
      FALSE,
      NORMAL_PRIORITY_CLASS,
      NULL /* environment (NULL means inherit) */,
      NULL /* current directory (NULL means inherit) */,
      &job->startup_info,
      &job->process_info)) {
    /* TODO : get proper error code */
    make_string_free(&cmdline_copy);
    return -EINVAL;
  }

  make_string_free(&cmdline_copy);

  return 0;
#else
#error "OS cannot be detected or is not supported"
  (void) err;
  (void) cmdline_copy;
  (void) job;
  (void) cmdline;
  return -EFAULT;
#endif
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
#elif defined(_WIN32)
  WaitForSingleObject(job->process_info.hProcess, INFINITE);
  if (exit_code != NULL) {
    if (!GetExitCodeProcess(job->process_info.hProcess, exit_code)) {
      /* TODO : get better error code */
      return -EINVAL;
    }
  }
#endif

  return 0;
}

