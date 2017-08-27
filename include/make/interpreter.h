#ifndef MAKE_INTERPRETER_H
#define MAKE_INTERPRETER_H

#include <make/job-manager.h>
#include <make/parser.h>
#include <make/string.h>
#include <make/table.h>

#include <stdio.h>

struct make_interpreter {
  /** Contains processes started from
   * recipes. */
  struct make_job_manager job_manager;
  struct make_string target;
  struct make_parser parser;
  /** @brief Stores all the variables
   * from the assignment statements. */
  struct make_table table;
  /** @brief Set to one if the last
   * target parsed was .PHONY */
  int phony_found;
  /** @brief Set to one if the target
   * was found when parsing a rule. */
  int target_found;
  /** @brief Set to one if the target
   * was found when parsing a makefile. */
  int target_found_once;
  /** @brief Set to one if the target
   * exists. */
  int target_exists;
  /** @brief Set to one if the target is
   * older than it's prerequisites. */
  int target_expired;
  /** @brief Target modification time. */
  long int target_mtime;
  /** @brief Logs error messages. */
  FILE *errlog;
  /** @brief Logs non-error messages. */
  FILE *outlog;
};

void make_interpreter_init(struct make_interpreter *interpreter);

void make_interpreter_free(struct make_interpreter *interpreter);

int make_interpreter_define(struct make_interpreter *interpreter,
                            const struct make_string *key,
                            const struct make_string *value);

int make_interpreter_has_target(const struct make_interpreter *interpreter);

int make_interpreter_read(struct make_interpreter *interpreter,
                          const char *filename);

int make_interpreter_run(struct make_interpreter *interpreter);

int make_interpreter_set_target(struct make_interpreter *interpreter,
                                const struct make_string *target);

#endif /* MAKE_INTERPRETER_H */
