project(square)
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

# include_directories(${PYTHON_INCLUDE_DIRS})
# add_library(square_nanobind SHARED square.cpp)
# include_directories($ENV{NANOBIND_DIR})

# add_subdirectory($ENV{NANOBIND_DIR})

find_package(nanobind CONFIG REQUIRED)

include_directories($ENV{ROBIN_MAP_DIR})
function(dump_cmake_variables)
  get_cmake_property(_variableNames VARIABLES)
  list (SORT _variableNames)
  foreach (_variableName ${_variableNames})
    if (ARGV0)
      unset(MATCHED)
      string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})
      if (NOT MATCHED)
        continue()
      endif()
    endif()
    message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
endfunction()
dump_cmake_variables()
nanobind_add_module(square_nanobind
  NB_STATIC # Build static libnanobind (the extension module itself remains a shared library)
  square.cpp)

# don't prepend wrapper library name with lib
set_target_properties(square_nanobind PROPERTIES PREFIX "")

# install targets
# install(TARGETS square
#  RUNTIME DESTINATION ${BIN_DIR}
#  LIBRARY DESTINATION ${LIB_DIR}
#  ARCHIVE DESTINATION ${LIB_DIR})
