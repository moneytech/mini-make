cmake_minimum_required(VERSION 3.0)

if (NOT EXTDIR AND UNIX)
  set (EXTDIR "posix")
endif (NOT EXTDIR AND UNIX)

if (NOT EXTDIR AND WIN32)
  set (EXTDIR "windows")
endif (NOT EXTDIR AND WIN32)

if (NOT EXTDIR)
  message(FATAL_ERROR "Operating system not supported")
endif (NOT EXTDIR)

set (SRCDIR "${CMAKE_CURRENT_SOURCE_DIR}")
set (INCDIR "${PROJECT_SOURCE_DIR}/include/mini-make")

add_library("make" STATIC
  "${INCDIR}/buffer.h"
  "${SRCDIR}/buffer.c"
  "${INCDIR}/chdir.h"
  "${SRCDIR}/chdir.c"
  "${INCDIR}/error.h"
  "${SRCDIR}/error.c"
  "${INCDIR}/ihooks.h"
  "${SRCDIR}/ihooks.c"
  "${INCDIR}/interpreter.h"
  "${SRCDIR}/interpreter.c"
  "${INCDIR}/job.h"
  "${SRCDIR}/job.c"
  "${INCDIR}/job-manager.h"
  "${SRCDIR}/job-manager.c"
  "${INCDIR}/node.h"
  "${SRCDIR}/node.c"
  "${INCDIR}/phooks.h"
  "${SRCDIR}/phooks.c"
  "${INCDIR}/parser.h"
  "${SRCDIR}/parser.c"
  "${INCDIR}/rule.h"
  "${SRCDIR}/rule.c"
  "${INCDIR}/string.h"
  "${SRCDIR}/string.c"
  "${INCDIR}/table.h"
  "${SRCDIR}/table.c"
  "${INCDIR}/target.h"
  "${SRCDIR}/target.c"
  "${INCDIR}/tree.h"
  "${SRCDIR}/tree.c"
  "${INCDIR}/var.h"
  "${SRCDIR}/var.c")

target_include_directories("make"
	PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	       $<INSTALL_INTERFACE:include>)

add_executable("mini-make" "mini-make.c")
target_link_libraries("mini-make" "make")

install(TARGETS "mini-make" "make"
  EXPORT "MiniMakeTargets"
  RUNTIME DESTINATION "bin"
  LIBRARY DESTINATION "lib"
  ARCHIVE DESTINATION "lib"
  PUBLIC_HEADER DESTINATION "include/mini-make")

install(EXPORT "MiniMakeTargets"
  DESTINATION "lib/mini-make")

function(ADD_MAKE_TEST TEST_NAME)
  add_executable(${TEST_NAME} ${ARGN})
  target_link_libraries(${TEST_NAME} "make")
  target_include_directories(${TEST_NAME} PRIVATE "include")
  target_compile_definitions(${TEST_NAME} PRIVATE
    "-DTESTING_DIR=\"${PROJECT_SOURCE_DIR}/testing\"")
  add_test(${TEST_NAME} ${TEST_NAME})
endfunction(ADD_MAKE_TEST TEST_NAME)

add_make_test("buffer-test" "buffer-test.c")
add_make_test("table-test" "table-test.c")
add_make_test("parser-test" "parser-test.c")
add_make_test("interpreter-test" "interpreter-test.c")

enable_testing()
