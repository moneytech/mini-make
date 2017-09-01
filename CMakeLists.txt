cmake_minimum_required(VERSION 2.6)

project("mini-make")

include_directories("include")

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  set (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
  set (CMAKE_C_FLAGS "/W3 /WX /D_CRT_SECURE_NO_WARNINGS")
endif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")

if (NOT EXTDIR AND UNIX)
  set (EXTDIR "${CMAKE_CURRENT_SOURCE_DIR}/posix")
endif (NOT EXTDIR AND UNIX)

if (NOT EXTDIR AND WIN32)
  set (EXTDIR "${CMAKE_CURRENT_SOURCE_DIR}/windows")
endif (NOT EXTDIR AND WIN32)

if (NOT EXTDIR)
  message(FATAL_ERROR "Operating system not supported")
endif (NOT EXTDIR)

set (SRCDIR "${CMAKE_CURRENT_SOURCE_DIR}")
set (INCDIR "${CMAKE_CURRENT_SOURCE_DIR}/include/mini-make")

add_library("make" STATIC
  "${SRCDIR}/chdir.c"
  "${INCDIR}/chdir.h"
  "${SRCDIR}/error.c"
  "${INCDIR}/error.h"
  "${SRCDIR}/interpreter.c"
  "${INCDIR}/interpreter.h"
  "${SRCDIR}/job.c"
  "${INCDIR}/job.h"
  "${SRCDIR}/job-manager.c"
  "${INCDIR}/job-manager.h"
  "${SRCDIR}/listener.c"
  "${INCDIR}/listener.h"
  "${SRCDIR}/parser.c"
  "${INCDIR}/parser.h"
  "${SRCDIR}/string.c"
  "${INCDIR}/string.h"
  "${SRCDIR}/table.c"
  "${INCDIR}/table.h"
  "${EXTDIR}/thread.c"
  "${INCDIR}/thread.h"
  "${INCDIR}/thread-callback.h"
  "${SRCDIR}/var.c"
  "${INCDIR}/var.h")

add_executable("mini-make" "mini-make.c")

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
add_make_test("interpreter-test" "interpreter-test.c")

add_subdirectory("testing")

enable_testing()

set (CPACK_PACKAGE_NAME "Mini Make")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "A miniature implementation of Make.")
set (CPACK_PACKAGE_VENDOR "Taylor Holberton")
set (CPACK_PACKAGE_VERSION_MAJOR 0)
set (CPACK_PACKAGE_VERSION_MINOR 1)
set (CPACK_PACKAGE_VERSION_PATCH 1)
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_CONTACT "taylorcholberton@gmail.com")

include(CPack)

