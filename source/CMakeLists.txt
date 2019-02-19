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
  "mk_malloc.h"
  "mk_malloc.c"
  "mk_open.h"
  "mk_open.c"
  "mk_parse.h"
  "mk_parse.c"
  "mk_report.h"
  "mk_report.c"
  "mk_run.h"
  "mk_run.c"
  "mk_scan.h"
  "mk_scan.c"
  "mk_state.h"
  "mk_state.c"
  "mk_strdup.h"
  "mk_strdup.c")

target_include_directories("make"
	PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	       $<INSTALL_INTERFACE:include>)

add_executable("mini-make" "mini-make.c")
target_link_libraries("mini-make" "make")
set_target_properties("mini-make" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

install(TARGETS "mini-make" "make"
  EXPORT "MiniMakeTargets"
  RUNTIME DESTINATION "bin"
  LIBRARY DESTINATION "lib"
  ARCHIVE DESTINATION "lib"
  PUBLIC_HEADER DESTINATION "include/mini-make")

install(EXPORT "MiniMakeTargets"
  DESTINATION "lib/mini-make")
