#include <mini-make/thread.h>

#include <mini-make/error.h>
#include <mini-make/thread-callback.h>

#include <pthread.h>

#include <stdlib.h>

struct make_thread {
  pthread_t thread;
};

struct make_thread *make_thread_create(struct make_thread_callback *callback) {

  int err;
  struct make_thread *thread;

  thread = malloc(sizeof(*thread));
  if (thread == NULL)
    return NULL;

  err = pthread_create(&thread->thread,
                       NULL /* attributes */,
                       callback->function,
                       callback->data);
  if (err) {
    free(thread);
    return NULL;
  }

  return thread;
}

void make_thread_destroy(struct make_thread *thread) {
  if (thread != NULL) {
    pthread_cancel(thread->thread);
    free(thread);
  }
}

int make_thread_join(struct make_thread *thread, void **result) {

  int err;

  err = pthread_join(thread->thread, result);
  if (err)
    return make_failure;

  free(thread);

  return make_success;
}

