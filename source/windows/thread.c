#include <mini-make/thread.h>

#include <mini-make/error.h>
#include <mini-make/thread-callback.h>

#include <windows.h>

#include <stdlib.h>

struct make_thread_data {
  void *data;
  void *result;
  void *(*function)(void *data);
};

struct make_thread {
  HANDLE handle;
  struct make_thread_data data;
};

static DWORD WINAPI thread_function(void *data) {

  struct make_thread_data *thread_data;

  thread_data = (struct make_thread_data *) data;

  thread_data->result = thread_data->function(thread_data->data);

  return 0;
}

struct make_thread *make_thread_create(struct make_thread_callback *callback) {

  struct make_thread *thread;

  thread = malloc(sizeof(*thread));
  if (thread == NULL)
    return NULL;

  thread->data.data = callback->data;
  thread->data.function = callback->function;
  thread->data.result = NULL;

  thread->handle = CreateThread(NULL /* security attributes */,
                                0 /* stack size (0 == default) */,
                                thread_function,
                                &thread->data,
                                0 /* creation flags */,
                                NULL /* thread ID */);
  if (thread->handle == NULL) {
    free(thread);
    return NULL;
  }

  return thread;
}

void make_thread_destroy(struct make_thread *thread) {
  if (thread != NULL) {
    if (thread->handle != NULL) {
      CloseHandle(thread->handle);
      thread->handle = NULL;
    }
    free(thread);
  }
}

int make_thread_join(struct make_thread *thread, void **result) {

  if (thread == NULL) {
    return make_failure;
  } else if (thread->handle == NULL) {
    free(thread);
    return make_failure;
  }

  WaitForSingleObject(thread->handle, INFINITE);

  CloseHandle(thread->handle);
  thread->handle = NULL;

  free(thread);

  if (result != NULL)
    *result = thread->data.result;

  return make_success;
}

