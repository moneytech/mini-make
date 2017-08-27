cmake_minimum_required(VERSION 2.6)

project("mini-make")

include_directories("include")

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  set (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -ansi -pedantic")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -ansi -pedantic")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
  set (CMAKE_C_FLAGS "/W3 /WX")
endif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")

add_library("make-library"
  "chdir.c"
  "interpreter.c"
  "interpreter-test.c"
  "job.c"
  "job-manager.c"
  "listener.c"
  "make.c"
  "parser.c"
  "parser-test.c"
  "string.c"
  "table.c"
  "table-test.c"
  "var.c")

set_target_properties("make-library" PROPERTIES OUTPUT_NAME "make")

add_executable("make-program" "make.c")

set_target_properties("make-program" PROPERTIES OUTPUT_NAME "make")

target_link_libraries("make-program" "make-library")

function(ADD_MAKE_TEST TEST_NAME)
  add_executable(${TEST_NAME} ${ARGN})
  target_link_libraries(${TEST_NAME} "make-library")
  add_test(${TEST_NAME} ${TEST_NAME})
endfunction(ADD_MAKE_TEST TEST_NAME)

add_make_test("table-test" "table-test.c")
add_make_test("parser-test" "parser-test.c")
