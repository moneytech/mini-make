cmake_minimum_required(VERSION 2.6)

find_package(Doxygen)
if (DOXYGEN_FOUND)
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
                 "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
  add_custom_target("documentation"
    ${DOXYGEN_EXECUTABLE}
    COMMENT "Generating doxygen documentation"
    DEPENDS "mini-make" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endif (DOXYGEN_FOUND)
