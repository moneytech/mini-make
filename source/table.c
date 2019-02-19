/* Copyright (C) 2017 Taylor Holberton
 *
 * This file is part of Mini Make.
 *
 * Mini Make is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mini Make is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mini Make.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <mini-make/table.h>

#include <mini-make/assignment-stmt.h>
#include <mini-make/string.h>
#include <mini-make/var.h>

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
  make_string_init(&table->target);
  make_string_init(&table->first_prerequisite);
  make_string_init(&table->all_prerequisites);
}

void make_table_free(struct make_table *table) {
  unsigned long int i;
  struct make_var *entry;
  for (i = 0; i < table->entry_count; i++) {
    entry = &table->entry_array[i];
    make_var_free(entry);
  }
  free(table->entry_array);
  table->entry_array = 0;
  table->entry_count = 0;
  make_string_free(&table->target);
  make_string_free(&table->first_prerequisite);
  make_string_free(&table->all_prerequisites);
}

int make_table_set_target(struct make_table *table,
                          const struct make_string *target) {
  make_string_free(&table->target);
  return make_string_copy(target, &table->target);
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

int make_table_evaluate(struct make_table *table,
                        const struct make_string *in,
                        struct make_string *out) {

  int err;
  char c;
  char c2;
  unsigned long int i;
  struct make_string key;
  struct make_string value;

  i = 0;
  while (i < in->size) {
    c = in->data[i];
    if (c == '$') {
      i++;
      if (i >= in->size)
        break;
    } else {
      err = make_string_append_char(out, c);
      if (err)
        return err;
      i++;
      continue;
    }
    c = in->data[i];
    if ((c == '{') || (c == '(')) {
      i++;
      if (i >= in->size)
        break;
      key.data = &in->data[i];
      key.size = 0;
      key.res = 0;
      while (i < in->size) {
        c2 = in->data[i];
        if (((c == '(') && (c2 == ')'))
         || ((c == '{') && (c2 == '}'))) {
          i++;
          break;
        }
        key.size++;
        i++;
      }
      make_string_init(&value);
      err = make_table_value_of(table, &key, &value);
      if (err) {
        make_string_free(&value);
        return err;
      }
      err = make_string_append(out, &value);
      if (err) {
        make_string_free(&value);
        return err;
      }
      make_string_free(&value);
      continue;
    } else if (c == '@') {
      err = make_string_append(out, &table->target);
      if (err)
        return err;
    } else if (c == '<') {
      err = make_string_append(out, &table->first_prerequisite);
      if (err)
        return err;
    } else if (c == '^') {
      err = make_string_append(out, &table->all_prerequisites);
      if (err)
        return err;
    }
    i++;
  }

  return 0;
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

int make_table_value_of(const struct make_table *table,
                        const struct make_string *key,
                        struct make_string *value) {

  int err;
  unsigned long int i;
  struct make_var *entry;

  for (i = 0; i < table->entry_count; i++) {
    entry = &table->entry_array[i];
    if (make_string_equal(&entry->key, key)) {
      err = make_string_append(value, &entry->value);
      if (err)
        return err;
      break;
    }
  }

  /* TODO : evaluate nested references */

  return 0;
}

