#ifndef MAKE_COMMAND_H
#define MAKE_COMMAND_H

struct make_string;

struct make_command {
  int silent;
  int ignore_error;
  struct make_string *source;
};

#endif /* MAKE_COMMAND_H */
