#ifndef MAKE_ASSIGNMENT_STMT_H
#define MAKE_ASSIGNMENT_STMT_H

#include "operation.h"

struct make_string;

struct make_assignment_stmt {
  enum make_operation operation;
  struct make_string *key;
  struct make_string *value;
};

#endif /* MAKE_ASSIGNMENT_STMT_H */

