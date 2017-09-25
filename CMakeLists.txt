cmake_minimum_required(VERSION 2.6)

project("mini-make" C)

set (MINI_MAKE_VERSION "0.2.0")
set (MINI_MAKE_VERISON_MAJOR 0)
set (MINI_MAKE_VERSION_MINOR 2)
set (MINI_MAKE_VERSION_PATCH 0)

add_subdirectory("examples")

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
  "${SRCDIR}/buffer.c"
  "${INCDIR}/buffer.h"
  "${SRCDIR}/chdir.c"
  "${INCDIR}/chdir.h"
  "${SRCDIR}/error.c"
  "${INCDIR}/error.h"
  "${SRCDIR}/interpreter.c"
  "${INCDIR}/interpreter.h"
  "${SRCDIR}/ihooks.c"
  "${INCDIR}/ihooks.h"
  "${SRCDIR}/job.c"
  "${INCDIR}/job.h"
  "${SRCDIR}/job-manager.c"
  "${INCDIR}/job-manager.h"
  "${SRCDIR}/phooks.c"
  "${INCDIR}/phooks.h"
  "${SRCDIR}/parser.c"
  "${INCDIR}/parser.h"
  "${SRCDIR}/protocol.c"
  "${INCDIR}/protocol.h"
  "${SRCDIR}/string.c"
  "${INCDIR}/string.h"
  "${SRCDIR}/table.c"
  "${INCDIR}/table.h"
  "${SRCDIR}/target.c"
  "${INCDIR}/target.h"
  "${EXTDIR}/thread.c"
  "${INCDIR}/thread.h"
  "${INCDIR}/thread-callback.h"
  "${SRCDIR}/var.c"
  "${INCDIR}/var.h")
target_include_directories("make" INTERFACE "${CMAKE_INSTALL_PREFIX}/include")
target_include_directories("make" PRIVATE "include")

add_executable("mini-make" "mini-make.c")
target_include_directories("mini-make" PRIVATE "include")
target_link_libraries("mini-make" "make")

install(DIRECTORY "include/mini-make"
  DESTINATION "include"
  FILES_MATCHING PATTERN "*.h")

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

add_make_test("table-test" "table-test.c")
add_make_test("parser-test" "parser-test.c")
add_make_test("interpreter-test" "interpreter-test.c")

add_subdirectory("testing")

enable_testing()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/MiniMakeConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/MiniMakeConfig.cmake" @ONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/MiniMakeConfigVersion.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/MiniMakeConfigVersion.cmake" @ONLY)

if (WIN32)
  install(FILES
      "${CMAKE_CURRENT_BINARY_DIR}/MiniMakeConfig.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/MiniMakeConfigVersion.cmake"
    DESTINATION
      "cmake")
endif (WIN32)

if (UNIX)
  install(FILES
      "${CMAKE_CURRENT_BINARY_DIR}/MiniMakeConfig.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/MiniMakeConfigVersion.cmake"
    DESTINATION
      "share/MiniMake/cmake")
endif (UNIX)

set (CPACK_PACKAGE_NAME "Mini Make")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "A miniature implementation of Make.")
set (CPACK_PACKAGE_VENDOR "Taylor Holberton")
set (CPACK_PACKAGE_VERSION_MAJOR ${MINI_MAKE_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${MINI_MAKE_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${MINI_MAKE_VERSION_PATCH})
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_CONTACT "taylorcholberton@gmail.com")

include(CPack)

