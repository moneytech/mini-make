cmake_minimum_required(VERSION 2.6)

macro(ADD_MINI_MAKE_EXAMPLE EXAMPLE_NAME)
  add_executable("${EXAMPLE_NAME}" ${ARGN})
  target_link_libraries("${EXAMPLE_NAME}" "make")
  target_include_directories(${EXAMPLE_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")
endmacro(ADD_MINI_MAKE_EXAMPLE EXAMPLE_NAME)

add_mini_make_example("list-targets" "list-targets.c")

find_package(ASPELL)
if (ASPELL_FOUND)
  add_mini_make_example("comment-spell-checker" "comment-spell-checker.c")
  target_include_directories("comment-spell-checker" PRIVATE ${ASPELL_INCLUDE_DIRS})
  target_link_libraries("comment-spell-checker" ${ASPELL_LIBRARIES})
endif (ASPELL_FOUND)
