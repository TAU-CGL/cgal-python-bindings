set(PY_MIN_VERSION 3.5)
set(BOOST_MIN_VERSION 1.68.0)

# Find required libraries
find_package(CGAL REQUIRED COMPONENTS Core)
if (NOT CGAL_FOUND)
  message(STATUS "This library cannot be compiled, as it requires CGAL.")
  return()
endif()
include(${CGAL_USE_FILE})

if (BUILD_SHARED_LIBS)
  set(Boost_USE_STATIC_LIBS OFF)
else()
  set(Boost_USE_STATIC_LIBS ON)
  set(BOOST_PYTHON_STATIC_LIB 1)
endif()
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
if ("${CGALPY_KERNEL_NAME}" STREQUAL "epec")
  set(CGALPY_KERNEL ${CGALPY_EPEC_KERNEL})
elseif ("${CGALPY_KERNEL_NAME}" STREQUAL "epic")
  set(CGALPY_KERNEL ${CGALPY_EPIC_KERNEL})
endif()
add_definitions(-DCGALPY_KERNEL=${CGALPY_KERNEL})

# Select Geometry Traits
if     ("${CGALPY_GEOMETRY_TRAITS_NAME}" STREQUAL "linear")
  set(CGALPY_GEOMETRY_TRAITS ${CGALPY_ARR_LINEAR_GEOMETRY_TRAITS})
elseif ("${CGALPY_GEOMETRY_TRAITS_NAME}" STREQUAL "segment")
  set(CGALPY_GEOMETRY_TRAITS ${CGALPY_ARR_SEGMENT_GEOMETRY_TRAITS})
elseif ("${CGALPY_GEOMETRY_TRAITS_NAME}" STREQUAL "nonCachingSegment")
  set(CGALPY_GEOMETRY_TRAITS ${CGALPY_ARR_NON_CACHING_SEGMENT_GEOMETRY_TRAITS})
elseif ("${CGALPY_GEOMETRY_TRAITS_NAME}" STREQUAL "conic")
  set(CGALPY_GEOMETRY_TRAITS ${CGALPY_ARR_CONIC_GEOMETRY_TRAITS})
elseif ("${CGALPY_GEOMETRY_TRAITS_NAME}" STREQUAL "algebraic")
  set(CGALPY_GEOMETRY_TRAITS ${CGALPY_ARR_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS})
elseif ("${CGALPY_GEOMETRY_TRAITS_NAME}" STREQUAL "circleSegment")
  set(CGALPY_GEOMETRY_TRAITS ${CGALPY_ARR_CIRCLE_SEGMENT_GEOMETRY_TRAITS})
endif()
add_definitions(-DCGALPY_GEOMETRY_TRAITS=${CGALPY_GEOMETRY_TRAITS})

# Select DCEL
if     ("${CGALPY_DCEL_NAME}" STREQUAL "plain")
  set(CGALPY_DCEL ${CGALPY_ARR_PLAIN_DCEL})
elseif ("${CGALPY_DCEL_NAME}" STREQUAL "faceExtended")
  set(CGALPY_DCEL ${CGALPY_ARR_FACE_EXTENDED_DCEL})
elseif ("${CGALPY_DCEL_NAME}" STREQUAL "halfedgeExtended")
  set(CGALPY_DCEL ${CGALPY_ARR_HALFEDGE_EXTENDED_DCEL})
elseif ("${CGALPY_DCEL_NAME}" STREQUAL "vertexExtended")
  set(CGALPY_DCEL ${CGALPY_ARR_VERTEX_EXTENDED_DCEL})
elseif ("${CGALPY_DCEL_NAME}" STREQUAL "allExtended")
  set(CGALPY_DCEL ${CGALPY_ARR_ALL_EXTENDED_DCEL})
endif()
add_definitions(-DCGALPY_DCEL=${CGALPY_DCEL})

# Select Dimension
add_definitions(-DCGALPY_DIMENSION=${CGALPY_DIMENSION})

# Minkowsi sum
if (${CGALPY_MINKOWSKI_SUM_BINDINGS})
  add_definitions(-DCGALPY_MINKOWSKI_SUM_BINDINGS)
endif()

# Triangulations bindings
if (${CGALPY_TRIANGULATIONS_BINDINGS})
  add_definitions(-DCGALPY_TRIANGULATIONS_BINDINGS)
endif()

# Spatial searching bindings
if (${CGALPY_SPATIAL_SEARCHING_BINDINGS})
  add_definitions(-DCGALPY_SPATIAL_SEARCHING_BINDINGS)
endif()

# Convex hull bindings
if (${CGALPY_CONVEX_HULL_BINDINGS})
  add_definitions(-DCGALPY_CONVEX_HULL_BINDINGS)
endif()

# Bounding volumes bindings
if (${CGALPY_BOUNDING_VOLUMES_BINDINGS})
  add_definitions(-DCGALPY_BOUNDING_VOLUMES_BINDINGS)
endif()

# Boolean set operations bindings
if (${CGALPY_BOOLEAN_SET_OPERATIONS_BINDINGS})
  add_definitions(-DCGALPY_BOOLEAN_SET_OPERATIONS_BINDINGS)
endif()

# Polygon partitioning operations bindings
if (${CGALPY_POLYGON_PARTITIONING_BINDINGS})
  add_definitions(-DCGALPY_POLYGON_PARTITIONING_BINDINGS)
endif()

# Point location bindings
if (${CGALPY_POINT_LOCATION_BINDINGS})
  add_definitions(-DCGALPY_POINT_LOCATION_BINDINGS)
endif()

set (CGALPY_INSTALL_INC_DIR "include" CACHE STRING "The folder where CGALPY header files will be installed, relative to CMAKE_INSTALL_PREFIX")
set (CGALPY_INSTALL_LIB_DIR "lib"     CACHE STRING "The folder where CGALPY libraries will be installed, relative to CMAKE_INSTALL_PREFIX")
set (CGALPY_INSTALL_BIN_DIR "bin"
  CACHE STRING "The folder where CGALPY executables, e.g., binaried and scripts, will be installed, relative to CMAKE_INSTALL_PREFIX")
set (CGALPY_INSTALL_CMAKE_DIR "${CGALPY_INSTALL_LIB_DIR}/CGALPY"
  CACHE STRING "The folder where CGALPY CMake modules will be installed, relative to CMAKE_INSTALL_PREFIX")

# Add subdirectories
add_subdirectory(libs)
