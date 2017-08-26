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
  assert(table.entry_array[0].key->size == 3);
  assert(memcmp(table.entry_array[0].key->data, "CXX", 3) == 0);
  assert(table.entry_array[0].value->size == 3);
  assert(memcmp(table.entry_array[0].value->data, "g++", 3) == 0);

  make_table_free(&table);
}

int main(void) {
  test_update();
  return EXIT_SUCCESS;
}

