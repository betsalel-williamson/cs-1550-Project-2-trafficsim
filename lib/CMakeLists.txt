cmake_minimum_required(VERSION 2.8)

set(CMAKE_MACOSX_RPATH 1)

# Define helper functions and macros used by Google Test.
#include(../test/lib/googletest/cmake/internal_utils.cmake)

project(SysCallLibs CXX C)

#config_compiler_and_linker()  # Defined in internal_utils.cmake.

# Where Project_1's .h files can be found.
include_directories(
        ${SysCallLibs_SOURCE_DIR}/include
        ${SysCallLibs_SOURCE_DIR})

# Where Project_1's libraries can be found.
link_directories(${SysCallLibs_BINARY_DIR}/src)

########################################################################
#
# Defines the lib libraries.  User tests should link
# with one of them.

# Project_1 libraries.  We build them using more strict warnings than what
# are used for other targets, to ensure that gtest can be compiled by a user
# aggressive about warnings.
add_library(SysCallLibs STATIC
        src/library.c
        include/library.h)

SET_TARGET_PROPERTIES(SysCallLibs PROPERTIES LINKER_LANGUAGE C)

#cxx_library(Project_1_lib "${cxx_strict}" src/syscalls.c)
#target_link_libraries(Project_1_lib)