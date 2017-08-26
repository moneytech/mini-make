#include <make/interpreter.h>

#include <make/table.h>

#include <stdio.h>
#include <stdlib.h>

int main(void) {

  struct make_table table;

  make_table_init(&table);

  make_table_free(&table);

  return EXIT_SUCCESS;
}

