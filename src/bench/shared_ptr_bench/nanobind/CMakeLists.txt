project(sp_nanobind)
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
find_package(Python ${PY_VERSION} COMPONENTS Interpreter Development REQUIRED)

# find_package(tsl-robin-map REQUIRED)
find_package(nanobind CONFIG REQUIRED)

include_directories(${tsl-robin-map_INCLUDE_DIRS})
nanobind_add_module(sp_nanobind
  NB_STATIC # Build static libnanobind (the extension module itself remains a shared library)
  sp_nanobind.cpp)

# don't prepend wrapper library name with lib
set_target_properties(sp_nanobind PROPERTIES PREFIX "")
