cmake_minimum_required(VERSION 2.6)

project("mini-make")

include_directories("include")

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  set (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -ansi -pedantic")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -pedantic")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
  set (CMAKE_C_FLAGS "/W3 /WX /D_CRT_SECURE_NO_WARNINGS")
endif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")

add_library("make" STATIC
  "chdir.c"
  "interpreter.c"
  "job.c"
  "job-manager.c"
  "listener.c"
  "parser.c"
  "string.c"
  "table.c"
  "var.c")

add_executable("mini-make" "make.c")

target_link_libraries("mini-make" "make")

install(TARGETS "mini-make"
  RUNTIME DESTINATION "bin")

function(ADD_MAKE_TEST TEST_NAME)
  add_executable(${TEST_NAME} ${ARGN})
  target_link_libraries(${TEST_NAME} "make")
  add_test(${TEST_NAME} ${TEST_NAME})
endfunction(ADD_MAKE_TEST TEST_NAME)

add_make_test("table-test" "table-test.c")
add_make_test("parser-test" "parser-test.c")

enable_testing()

