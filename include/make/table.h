#ifndef MAKE_TABLE_H
#define MAKE_TABLE_H

#include <make/string.h>

struct make_assignment_stmt;
struct make_var;

struct make_table {
  struct make_string target;
  struct make_string first_prerequisite;
  struct make_string all_prerequisites;
  struct make_var *entry_array;
  unsigned long int entry_count;
};

void make_table_init(struct make_table *table);

void make_table_free(struct make_table *table);

int make_table_set_target(struct make_table *table,
                          const struct make_string *target);

int make_table_set_first_prerequisite(struct make_table *table,
                                      const struct make_string *first_preq);

int make_table_define(struct make_table *table,
                      const struct make_string *key,
                      const struct make_string *value);

int make_table_evaluate(struct make_table *table,
                        const struct make_string *in,
                        struct make_string *out);

int make_table_update(struct make_table *table,
    const struct make_assignment_stmt *assignment_stmt);

int make_table_value_of(const struct make_table *table,
                        const struct make_string *key,
                        struct make_string *value);

#endif /* MAKE_TABLE_H */
