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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_update(void) {

  int err;
  struct make_string key;
  struct make_string value;
  struct make_assignment_stmt assignment_stmt;
  struct make_table table;

  make_table_init(&table);

  key.data = "CXX";
  key.size = 3;
  value.data = "g++";
  value.size = 3;
  assignment_stmt.operation = MINI_MAKE_OPERATION_RECURSIVE;
  assignment_stmt.key = &key;
  assignment_stmt.value = &value;

  err = make_table_update(&table, &assignment_stmt);
  assert(err == 0);
  assert(table.entry_count == 1);
  assert(table.entry_array[0].key.size == 3);
  assert(memcmp(table.entry_array[0].key.data, "CXX", 3) == 0);
  assert(table.entry_array[0].value.size == 3);
  assert(memcmp(table.entry_array[0].value.data, "g++", 3) == 0);

  make_table_free(&table);
}

static void test_evaluate(void) {

  int err;
  struct make_table table;
  struct make_string key;
  struct make_string value;
  struct make_string target;
  struct make_string in;
  struct make_string out;

  make_table_init(&table);
  make_string_init(&key);
  make_string_init(&value);
  make_string_init(&target);

  err = make_string_set(&key, "CXX", 3);
  assert(err == 0);
  err = make_string_set(&value, "g++", 3);
  assert(err == 0);
  err = make_table_define(&table, &key, &value);
  assert(err == 0);
  err = make_string_set(&target, "test.o", 6);
  assert(err == 0);
  err = make_table_set_target(&table, &target);
  assert(err == 0);
  make_string_free(&key);
  make_string_free(&value);
  make_string_free(&target);

  make_string_init(&in);
  make_string_init(&out);

  err = make_string_set(&in, "${CXX} -c test.c -o $@", 22);
  assert(err == 0);
  err = make_table_evaluate(&table, &in, &out);
  assert(err == 0);
  assert(out.size == 23);
  assert(memcmp(out.data, "g++ -c test.c -o test.o", 23) == 0);

  make_string_free(&in);
  make_string_free(&out);

  make_table_free(&table);
}

int main(void) {
  test_update();
  test_evaluate();
  return EXIT_SUCCESS;
}

