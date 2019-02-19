cmake_minimum_required(VERSION 3.0)

project("mini_make" LANGUAGES C VERSION 0.2.0)

list (APPEND CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
  set (mini_make_CFLAGS -Wall -Wextra -Werror -Wfatal-errors -std=c99)
elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang")
  set (mini_make_CFLAGS -Wall -Wextra -Werror -Wfatal-errors -std=c99 -Wdocumentation)
elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  set (mini_make_CFLAGS /W3 /WX /D_CRT_SECURE_NO_WARNINGS)
endif (CMAKE_C_COMPILER_ID STREQUAL "GNU")

add_subdirectory("documentation")
# add_subdirectory("examples")
add_subdirectory("source")
add_subdirectory("testing")

install(DIRECTORY "include/mini-make"
  DESTINATION "include"
  FILES_MATCHING PATTERN "*.h")

enable_testing()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/MiniMakeConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/MiniMakeConfig.cmake" @ONLY)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/MiniMakeConfigVersion.cmake.in"
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
set (CPACK_PACKAGE_VERSION_MAJOR ${mini_make_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${mini_make_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${mini_make_VERSION_PATCH})
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_CONTACT "taylorcholberton@gmail.com")

include(CPack)
