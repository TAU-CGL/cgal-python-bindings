project(sp_boost)
cmake_minimum_required(VERSION 3.17...3.22)

# Generate a shared library
option(CGALPY_BUILD_SHARED_LIBS "Build shared libs instead of static libs" ON)

# Use C++17 for this directory and its sub-directories.
list(FIND CMAKE_CXX_COMPILE_FEATURES cxx_generalized_initializers has_cpp17)
if (has_cpp17 LESS 0)
  message(STATUS "NOTICE: These examples requires a C++17 compiler and will not be compiled.")
  return()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(PY_VERSION 3.8)
find_package(PythonLibs ${PY_VERSION} REQUIRED)

set(BOOST_VERSION 1.78.0)
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS system thread python38)

include_directories(${PYTHON_INCLUDE_DIRS})
add_library(sp_boost SHARED sp_boost.cpp)
target_link_libraries(sp_boost ${Boost_LIBRARIES})

# don't prepend wrapper library name with lib
set_target_properties(sp_boost PROPERTIES PREFIX "")
