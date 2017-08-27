#ifndef MAKE_OPTIONS_H
#define MAKE_OPTIONS_H

struct make_options {
  char *working_dir;
  char *filename;
  unsigned int jobs;
  int just_print;
  int silent;
};

#endif /* MAKE_OPTIONS_H */
