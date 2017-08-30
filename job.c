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

static int format_cmdline(const struct make_string *cmdline_in,
                          struct make_string *cmdline_out) {
  int err;
#if defined(_WIN32)
  char cmd_str[] = "cmd /c \"";
  struct make_string cmd;
#endif

  err = make_string_copy(cmdline_in, cmdline_out);
  if (err)
    return err;

#if defined(_WIN32)
  cmd.data = cmd_str;
  cmd.size = sizeof(cmd_str) - 1;
  err = make_string_prepend(cmdline_out, &cmd);
  if (err)
    return err;
  err = make_string_append_char(cmdline_out, '\"');
  if (err)
    return err;
#endif

  return 0;
}

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
                   const struct make_string *cmdline_in) {

  int err;
  struct make_string cmdline_out;
#if defined(__unix__)
  char *argv[4];
#endif

#if defined(__unix__)
  job->fd = fork();
  if (job->fd < 0)
    return -errno;
  else if (job->fd > 0)
    return 0;

  make_string_init(&cmdline_out);

  err = format_cmdline(cmdline_in, &cmdline_out);
  if (err) {
    make_string_free(&cmdline_out);
    return err;
  }

  argv[0] = "bash";
  argv[1] = "-c";
  argv[2] = cmdline_out.data;
  argv[3] = NULL;
  execvp(argv[0], argv);
  exit(EXIT_FAILURE);
#elif defined(_WIN32)
  make_string_init(&cmdline_out);
  err = format_cmdline(cmdline_in, &cmdline_out);
  if (err) {
    make_string_free(&cmdline_out);
    return err;
  }
  if (!CreateProcess(NULL /* application name */,
      cmdline_out.data,
      NULL /* process security attributes */,
      NULL /* thread security attributes */,
      FALSE,
      NORMAL_PRIORITY_CLASS,
      NULL /* environment (NULL means inherit) */,
      NULL /* current directory (NULL means inherit) */,
      &job->startup_info,
      &job->process_info)) {
    /* TODO : get proper error code */
    make_string_free(&cmdline_out);
    return -EINVAL;
  }

  make_string_free(&cmdline_out);

  return 0;
#else
#error "OS cannot be detected or is not supported"
#endif
}

int make_job_wait(struct make_job *job, int *exit_code) {

#if defined(__unix__)
  int status;
#elif defined(_WIN32)
  DWORD exit_code_2;
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
    if (!GetExitCodeProcess(job->process_info.hProcess, &exit_code_2)) {
      /* TODO : get better error code */
      return -EINVAL;
    }
    *exit_code = (int) exit_code_2;
  }
#endif

  return 0;
}

