#include <make/chdir.h>

#include <errno.h>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__unix__)
#include <unistd.h>
#endif

int make_chdir(const char *path) {
#if defined(_WIN32)
  if (!SetWorkingDirectory(path))
    /* TODO get appropriate error code */
    return -EINVAL;
#elif defined(__unix__)
  if (chdir(path) < 0)
    return -errno;
#endif
  return 0;
}

