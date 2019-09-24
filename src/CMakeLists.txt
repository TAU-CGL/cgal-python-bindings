set(PY_MIN_VERSION 3.5)
set(BOOST_MIN_VERSION 1.68.0)

# Find required libraries
find_package(CGAL REQUIRED COMPONENTS Core)
if (NOT CGAL_FOUND)
  message(STATUS "This library cannot be compiled, as it requires CGAL.")
  return()
endif()
include(${CGAL_USE_FILE})

set(Boost_USE_STATIC_LIBS OFF)
set(CGALPY_BOOST_LIBRARIES system thread)

find_package(Python3 COMPONENTS Interpreter Development)

find_package(PythonLibs ${PY_MIN_VERSION} REQUIRED)
## Print all accessible variables
# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

if (PYTHONLIBS_VERSION_STRING VERSION_LESS 3)
  set(CGALPY_BOOST_LIBRARIES ${CGALPY_BOOST_LIBRARIES} python)
else()
  # Set the Boost python library name (e.g., python35)
  set (CGALPY_BOOST_PYTHON_LIB_NAME python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR})
  set(CGALPY_BOOST_LIBRARIES ${CGALPY_BOOST_LIBRARIES} ${CGALPY_BOOST_PYTHON_LIB_NAME})
endif()
include_directories(${PYTHON_INCLUDE_DIRS})
add_definitions(-DCGALPY_DEF_PYBINDINGS)
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS ${CGALPY_BOOST_LIBRARIES})

if (NOT Boost_FOUND)
  message(STATUS "This library cannot be compiled, as it requires Boost.")
  return()
endif()

find_package(OpenGL REQUIRED)

# Add includes and libs
include_directories(${Boost_INCLUDE_DIR})

if (WIN32)
  include_directories(libs/GL/include)
endif(WIN32)

# Add some compiler options
if (WIN32)
  add_definitions(-GR)
  add_definitions(-vmg)
endif()

# Use C++14 for this directory and its sub-directories.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Select Kernel
# set(CGALPY_EPEC_KERNEL 0)
# set(CGALPY_EPIC_KERNEL 1)

# if ("${CGALPY_KERNEL_NAME}" STREQUAL "epec")
#   add_definitions(-DCGALPY_KERNEL=${CGALPY_EPEC_KERNEL})
# elseif ("${CGALPY_KERNEL_NAME}" STREQUAL "epic")
#   add_definitions(-DCGALPY_KERNEL=${CGALPY_EPIC_KERNEL})
# endif()

# Select Geometry Traits
set(CGALPY_ARR_LINEAR_GEOMETRY_TRAITS 0)
set(CGALPY_ARR_SEGMENT_GEOMETRY_TRAITS 1)
set(CGALPY_ARR_NON_CACHING_SEGMENT_GEOMETRY_TRAITS 2)
set(CGALPY_ARR_CONIC_GEOMETRY_TRAITS 3)
set(CGALPY_ARR_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS 4)
set(CGALPY_ARR_CIRCLE_SEGMENT_GEOMETRY_TRAITS 5)

if ("${CGALPY_GEOMETRY_TRAITS_NAME}" STREQUAL "linear")
  add_definitions(-DCGALPY_GEOMETRY_TRAITS=${CGALPY_ARR_LINEAR_GEOMETRY_TRAITS})
elseif ("${CGALPY_GEOMETRY_TRAITS_NAME}" STREQUAL "segment")
  add_definitions(-DCGALPY_GEOMETRY_TRAITS=${CGALPY_ARR_SEGMENT_GEOMETRY_TRAITS})
elseif ("${CGALPY_GEOMETRY_TRAITS_NAME}" STREQUAL "nonCachingSegment")
  add_definitions(-DCGALPY_GEOMETRY_TRAITS=${CGALPY_ARR_NON_CACHING_SEGMENT_GEOMETRY_TRAITS})
elseif ("${CGALPY_GEOMETRY_TRAITS_NAME}" STREQUAL "conic")
  add_definitions(-DCGALPY_GEOMETRY_TRAITS=${CGALPY_ARR_CONIC_GEOMETRY_TRAITS})
elseif ("${CGALPY_GEOMETRY_TRAITS_NAME}" STREQUAL "algebraic")
  add_definitions(-DCGALPY_GEOMETRY_TRAITS=${CGALPY_ARR_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS})
elseif ("${CGALPY_GEOMETRY_TRAITS_NAME}" STREQUAL "circleSegment")
  add_definitions(-DCGALPY_GEOMETRY_TRAITS=${CGALPY_ARR_CIRCLE_SEGMENT_GEOMETRY_TRAITS})
endif()

set (CGALPY_INSTALL_INC_DIR "include" CACHE STRING "The folder where CGALPY header files will be installed, relative to CMAKE_INSTALL_PREFIX")
set (CGALPY_INSTALL_LIB_DIR "lib"     CACHE STRING "The folder where CGALPY libraries will be installed, relative to CMAKE_INSTALL_PREFIX")
set (CGALPY_INSTALL_BIN_DIR "bin"
  CACHE STRING "The folder where CGALPY executables, e.g., binaried and scripts, will be installed, relative to CMAKE_INSTALL_PREFIX")
set (CGALPY_INSTALL_CMAKE_DIR "${CGALPY_INSTALL_LIB_DIR}/CGALPY"
  CACHE STRING "The folder where CGALPY CMake modules will be installed, relative to CMAKE_INSTALL_PREFIX")

# Add subdirectories
add_subdirectory(libs)
