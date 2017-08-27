#include <make/table.h>

#include <make/assignment-stmt.h>
#include <make/string.h>
#include <make/var.h>

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
  assignment_stmt.operation = MAKE_OPERATION_RECURSIVE;
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

