cmake_minimum_required(VERSION 3.7)
project(square_boost)

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

set(BOOST_VERSION 1.64.0)
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS system thread python35)

include_directories(${PYTHON_INCLUDE_DIRS})
add_library(square_boost SHARED square_boost.cpp)
target_link_libraries(square_boost ${Boost_LIBRARIES})

# don't prepend wrapper library name with lib
set_target_properties(square_boost PROPERTIES PREFIX "")

# install targets
# install(TARGETS square
#  RUNTIME DESTINATION ${BIN_DIR}
#  LIBRARY DESTINATION ${LIB_DIR}
#  ARCHIVE DESTINATION ${LIB_DIR})
