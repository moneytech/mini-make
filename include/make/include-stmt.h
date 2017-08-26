#ifndef MAKE_INCLUDE_STMT_H
#define MAKE_INCLUDE_STMT_H

struct make_string;

struct make_include_stmt {
  int ignore_error;
  struct make_string *path;
};

#endif /* MAKE_INCLUDE_STMT_H */
