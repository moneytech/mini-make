cmake_minimum_required(VERSION 2.6)

if (WIN32)
  set (TEST_SH "powershell" "-NoLogo" "-NonInteractive" "-Command")
elseif (UNIX)
  set (TEST_SH)
endif (WIN32)

add_custom_target("integration-test"
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/basic/main${CMAKE_EXECUTABLE_SUFFIX}")

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/basic")

add_custom_command (OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/basic/main${CMAKE_EXECUTABLE_SUFFIX}"
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/basic/Makefile" "mini-make"
  COMMAND ${TEST_SH} "$<TARGET_FILE:mini-make>"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/basic"
  COMMENT "Running Mini Make on test project 'basic'")

add_custom_command (OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/basic/Makefile"
  COMMAND "${CMAKE_COMMAND}" "-G" "Unix Makefiles" "${CMAKE_CURRENT_SOURCE_DIR}/basic" -DCMAKE_MAKE_PROGRAM=$<TARGET_FILE:mini-make>
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/basic"
  COMMENT "Generating Makefile for 'basic' test project")

