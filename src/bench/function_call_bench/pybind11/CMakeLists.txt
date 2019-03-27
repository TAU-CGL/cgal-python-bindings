cmake_minimum_required(VERSION 3.7)
project(square)

# Generate a shared library
option(BUILD_SHARED_LIBS "Build shared libs instead of static libs" ON)

# Use C++11 for this directory and its sub-directories.
list(FIND CMAKE_CXX_COMPILE_FEATURES cxx_generalized_initializers has_cpp11)
if (has_cpp11 LESS 0)
  message(STATUS "NOTICE: These examples requires a C++11 compiler and will not be compiled.")
  return()
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(PY_VERSION 3.5)
find_package(PythonLibs ${PY_VERSION} REQUIRED)

include_directories(${PYTHON_INCLUDE_DIRS})
add_library(square SHARED square.cpp)
include_directories($ENV{PYBIND11_DIR})

# don't prepend wrapper library name with lib
set_target_properties(square PROPERTIES PREFIX "")

# install targets
# install(TARGETS square
#  RUNTIME DESTINATION ${BIN_DIR}
#  LIBRARY DESTINATION ${LIB_DIR}
#  ARCHIVE DESTINATION ${LIB_DIR})
