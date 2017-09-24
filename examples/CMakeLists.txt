cmake_minimum_required(VERSION 2.6)

macro(ADD_MINI_MAKE_EXAMPLE EXAMPLE_NAME)
  add_executable("${EXAMPLE_NAME}" ${ARGN})
  target_link_libraries("${EXAMPLE_NAME}" "make")
  target_include_directories(${EXAMPLE_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")
endmacro(ADD_MINI_MAKE_EXAMPLE EXAMPLE_NAME)

add_mini_make_example("list-targets" "list-targets.c")

