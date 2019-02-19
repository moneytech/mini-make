#include "mk_open.h"

#include "mk_malloc.h"
#include "mk_report.h"
#include "mk_scan.h"
#include "mk_state.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

int mk_open(struct mk_state* state) {

  const char* filename = mk_state_find(state, "__file__");
  if (!filename) {
    filename = "Makefile";
  }

  FILE* file = fopen(filename, "rb");
  if (!file) {
    mk_report(state, "failed to open file (%s)", strerror(errno));
    return -1;
  }

  int err = fseek(file, 0L, SEEK_END);
  if (err != 0) {
    mk_report(state, "failed to seek file position (%s)", strerror(errno));
    fclose(file);
    return -1;
  }

  long int file_size = ftell(file);
  if (file_size == -1L) {
    mk_report(state, "failed to get file size (%s)", strerror(errno));
    fclose(file);
    return -1;
  }

  err = fseek(file, 0L, SEEK_SET);
  if (err != 0) {
    mk_report(state, "failed to seek file position (%s)", strerror(errno));
    fclose(file);
    return -1;
  }

  char* data = mk_malloc(state, file_size);
  if (!data) {
    fclose(file);
    return -1;
  }

  size_t read_count = fread(data, 1, (size_t) file_size, file);

  fclose(file);

  err = mk_scan(state, data, read_count);

  mk_free(state, data);

  return err;
}
