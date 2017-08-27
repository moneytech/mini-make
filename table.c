#include <make/table.h>

#include <make/assignment-stmt.h>
#include <make/string.h>
#include <make/var.h>

#include <errno.h>
#include <stdlib.h>

static int make_table_new(struct make_table *table,
                          const struct make_string *key,
                          const struct make_string *value) {

  int err;
  struct make_var *entry_array;
  unsigned long int entry_count;
  unsigned long int size;
  struct make_var *n_entry;

  entry_count = table->entry_count;
  entry_count++;

  size = entry_count;
  size *= sizeof(entry_array[0]);

  entry_array = table->entry_array;
  entry_array = realloc(entry_array, size);
  if (entry_array == NULL)
    return -ENOMEM;
  else
    table->entry_array = entry_array;

  /* new entry */
  n_entry = &entry_array[entry_count - 1];

  err = make_string_copy(key, &n_entry->key);
  if (err)
    return -ENOMEM;

  err = make_string_copy(value, &n_entry->value);
  if (err) {
    free(n_entry->key.data);
    return -ENOMEM;
  }

  table->entry_count = entry_count;

  return 0;
}

void make_table_init(struct make_table *table) {
  table->entry_array = NULL;
  table->entry_count = 0;
}

void make_table_free(struct make_table *table) {
  unsigned long int i;
  struct make_var *entry;
  for (i = 0; i < table->entry_count; i++) {
    entry = &table->entry_array[i];
    make_string_free(&entry->key);
    make_string_free(&entry->value);
  }
  free(table->entry_array);
  table->entry_array = 0;
  table->entry_count = 0;
}

int make_table_define(struct make_table *table,
                      const struct make_string *key,
                      const struct make_string *value) {

  unsigned long int i;
  struct make_var *entry;

  for (i = 0; i < table->entry_count; i++) {
    entry = &table->entry_array[i];
    if (!make_string_equal(&entry->key, key))
      continue;
    /* entry found */
  }

  return make_table_new(table, key, value);
}

int make_table_update(struct make_table *table,
                      const struct make_assignment_stmt *assignment_stmt) {
  int err;
  err = make_table_define(table,
                          assignment_stmt->key,
                          assignment_stmt->value);
  if (err)
    return err;

  return 0;
}

