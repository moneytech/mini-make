#ifndef MAKE_OPTIONS_H
#define MAKE_OPTIONS_H

struct make_options {
  char *working_dir;
  char *filename;
  unsigned int jobs;
};

#endif /* MAKE_OPTIONS_H */
