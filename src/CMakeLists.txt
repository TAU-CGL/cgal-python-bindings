set(PY_MIN_VERSION 3.5)
set(BOOST_MIN_VERSION 1.68.0)

# Find required libraries
find_package(CGAL REQUIRED COMPONENTS Core)
if (NOT CGAL_FOUND)
  message(STATUS "This library cannot be compiled, as it requires CGAL.")
  return()
endif()
include(${CGAL_USE_FILE})

if(BUILD_SHARED_LIBS)
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
  set (CGALPY_BOOST_LIBRARIES ${CGALPY_BOOST_LIBRARIES} python)
else()
  # Set the Boost python library name (e.g., python35)
  set(CGALPY_BOOST_PYTHON_LIB_NAME python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR})
  set(CGALPY_BOOST_LIBRARIES ${CGALPY_BOOST_LIBRARIES} ${CGALPY_BOOST_PYTHON_LIB_NAME})
endif()
include_directories(${PYTHON_INCLUDE_DIRS})
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS ${CGALPY_BOOST_LIBRARIES})

if(NOT Boost_FOUND)
  message (STATUS "This library cannot be compiled, as it requires Boost.")
  return()
endif()

# Add includes and libs
include_directories(${Boost_INCLUDE_DIR})

# Add some compiler options
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_definitions(-GR)
  add_definitions(-vmg)
# elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#
endif()

# Use C++14 for this directory and its sub-directories.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Select kernel
if ("${CGALPY_KERNEL_NAME}" STREQUAL "epec")
  set(CGALPY_KERNEL ${CGALPY_EPEC_KERNEL})
elseif ("${CGALPY_KERNEL_NAME}" STREQUAL "epic")
  set(CGALPY_KERNEL ${CGALPY_EPIC_KERNEL})
endif()
add_definitions(-DCGALPY_KERNEL=${CGALPY_KERNEL})

# Select geometry traits
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

# Select dimension
add_definitions(-DCGALPY_DIMENSION=${CGALPY_DIMENSION})

# 2D Minkowsi sums
if(${CGALPY_MINKOWSKI_SUM_2_BINDINGS})
  add_definitions(-DCGALPY_MINKOWSKI_SUM_2_BINDINGS)
endif()

# Triangulations bindings
if (${CGALPY_TRIANGULATION_2_BINDINGS})
  add_definitions(-DCGALPY_TRIANGULATION_2_BINDINGS)
endif()

# Spatial searching bindings
if(${CGALPY_SPATIAL_SEARCHING_BINDINGS})
  add_definitions(-DCGALPY_SPATIAL_SEARCHING_BINDINGS)
endif()

# Convex hull bindings
if(${CGALPY_CONVEX_HULL_BINDINGS})
  add_definitions(-DCGALPY_CONVEX_HULL_BINDINGS)
endif()

# Bounding volumes bindings
if(${CGALPY_BOUNDING_VOLUMES_BINDINGS})
  add_definitions(-DCGALPY_BOUNDING_VOLUMES_BINDINGS)
endif()

# Boolean set operations bindings
if(${CGALPY_BOOLEAN_SET_OPERATIONS_BINDINGS})
  add_definitions(-DCGALPY_BOOLEAN_SET_OPERATIONS_BINDINGS)
endif()

# Polygon partitioning operations bindings
if(${CGALPY_POLYGON_PARTITIONING_BINDINGS})
  add_definitions(-DCGALPY_POLYGON_PARTITIONING_BINDINGS)
endif()

# Point location bindings
if(${CGALPY_POINT_LOCATION_BINDINGS})
  add_definitions(-DCGALPY_POINT_LOCATION_BINDINGS)
endif()

# 3D Alpha Shapes
if(${CGALPY_ALPHA_SHAPES_3_BINDINGS})
  # Check validity of vertex-base settings
  if((${CGALPY_TRI3_VERTEX_BASE_NAME} STREQUAL "plain") OR
     (${CGALPY_TRI3_VERTEX_BASE_NAME} STREQUAL "plainWithInfo") OR
     (${CGALPY_TRI3_VERTEX_BASE_NAME} STREQUAL "regular")  OR
     (${CGALPY_TRI3_VERTEX_BASE_NAME} STREQUAL "regularWithInfo"))
    message("Error: Invalid vertex base (${CGALPY_TRI3_VERTEX_BASE_NAME})")
    return()
  endif()
  # Check validity of cell-base settings
  if((${CGALPY_TRI3_CELL_BASE_NAME} STREQUAL "plain") OR
     (${CGALPY_TRI3_CELL_BASE_NAME} STREQUAL "plainWithInfo") OR
     (${CGALPY_TRI3_CELL_BASE_NAME} STREQUAL "regular")  OR
     (${CGALPY_TRI3_CELL_BASE_NAME} STREQUAL "regularWithInfo"))
    message("Error: Invalid vertex base (${CGALPY_TRI3_VERTEX_BASE_NAME})")
    return()
  endif()

  # Check validity of triangulation
  if((${CGALPY_TRI3_NAME} STREQUAL "[Rr]egular") AND
     (NOT ${CGALPY_TRI3_VERTEX_BASE_NAME} MATCHES "regular"))
    message("Error: Triangulation (${CGALPY_TRI3_NAME}) does not match vertex base (${CGALPY_TRI3_VERTEX_BASE_NAME})")
    return()
  endif()

  # Check validity of triangulation
  if((${CGALPY_TRI3_NAME} STREQUAL "[Rr]egular") AND
     (NOT ${CGALPY_TRI3_CELL_BASE_NAME} MATCHES "regular"))
    message("Error: Triangulation (${CGALPY_TRI3_NAME}) does not match cell base (${CGALPY_TRI3_CELL_BASE_NAME})")
    return()
  endif()

  add_definitions(-DCGALPY_ALPHA_SHAPES_3_BINDINGS)
endif()

# Select Alpha shape exact comparison tag
if(${CGALPY_EXACT_COMPARISON})
  set(CGALPY_EXACT_COMPARISON 1)
else()
  set(CGALPY_EXACT_COMPARISON 0)
endif()
add_definitions(-DCGALPY_EXACT_COMPARISON=${CGALPY_EXACT_COMPARISON})

# Select triangulation vertex base
if     ("${CGALPY_TRI3_VERTEX_BASE_NAME}" STREQUAL "plain")
  set(CGALPY_TRI3_VERTEX_BASE ${CGALPY_TRI3_VERTEX_BASE_PLAIN})
elseif ("${CGALPY_TRI3_VERTEX_BASE_NAME}" STREQUAL "plainWithInfo")
  set(CGALPY_TRI3_VERTEX_BASE ${CGALPY_TRI3_VERTEX_BASE_PLAIN_WITH_INFO})
elseif ("${CGALPY_TRI3_VERTEX_BASE_NAME}" STREQUAL "regular")
  set(CGALPY_TRI3_VERTEX_BASE ${CGALPY_TRI3_VERTEX_BASE_REGULAR})
elseif ("${CGALPY_TRI3_VERTEX_BASE_NAME}" STREQUAL "regualrWithInfo")
  set(CGALPY_TRI3_VERTEX_BASE ${CGALPY_TRI3_VERTEX_BASE_REGULAR_WITH_INFO})
elseif ("${CGALPY_TRI3_VERTEX_BASE_NAME}" STREQUAL "alphaShape")
  set(CGALPY_TRI3_VERTEX_BASE ${CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE})
elseif ("${CGALPY_TRI3_VERTEX_BASE_NAME}" STREQUAL "alphaShapeWithInfo")
  set(CGALPY_TRI3_VERTEX_BASE ${CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_WITH_INFO})
elseif ("${CGALPY_TRI3_VERTEX_BASE_NAME}" STREQUAL "alphaShapeRegular")
  set(CGALPY_TRI3_VERTEX_BASE ${CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR})
elseif ("${CGALPY_TRI3_VERTEX_BASE_NAME}" STREQUAL "alphaShapeRegularWithInfo")
  set(CGALPY_TRI3_VERTEX_BASE ${CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO})
elseif ("${CGALPY_TRI3_VERTEX_BASE_NAME}" STREQUAL "fixedAlphaShape")
  set(CGALPY_TRI3_VERTEX_BASE ${CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE})
elseif ("${CGALPY_TRI3_VERTEX_BASE_NAME}" STREQUAL "fixedAlphaShapeWithInfo")
  set(CGALPY_TRI3_VERTEX_BASE ${CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_WITH_INFO})
elseif ("${CGALPY_TRI3_VERTEX_BASE_NAME}" STREQUAL "fixedAlphaShapeRegular")
  set(CGALPY_TRI3_VERTEX_BASE ${CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR})
elseif ("${CGALPY_TRI3_VERTEX_BASE_NAME}" STREQUAL "fixedAlphaShapeRegularWithInfo")
  set(CGALPY_TRI3_VERTEX_BASE ${CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO})
endif()
add_definitions(-DCGALPY_TRI3_VERTEX_BASE=${CGALPY_TRI3_VERTEX_BASE})

# Select triangulation cell base
if     ("${CGALPY_TRI3_CELL_BASE_NAME}" STREQUAL "plain")
  set(CGALPY_TRI3_CELL_BASE ${CGALPY_TRI3_CELL_BASE_PLAIN})
elseif ("${CGALPY_TRI3_CELL_BASE_NAME}" STREQUAL "plainWithInfo")
  set(CGALPY_TRI3_CELL_BASE ${CGALPY_TRI3_CELL_BASE_PLAIN_WITH_INFO})
elseif ("${CGALPY_TRI3_CELL_BASE_NAME}" STREQUAL "regular")
  set(CGALPY_TRI3_CELL_BASE ${CGALPY_TRI3_CELL_BASE_REGULAR})
elseif ("${CGALPY_TRI3_CELL_BASE_NAME}" STREQUAL "regualrWithInfo")
  set(CGALPY_TRI3_CELL_BASE ${CGALPY_TRI3_CELL_BASE_REGULAR_WITH_INFO})
elseif ("${CGALPY_TRI3_CELL_BASE_NAME}" STREQUAL "alphaShape")
  set(CGALPY_TRI3_CELL_BASE ${CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE})
elseif ("${CGALPY_TRI3_CELL_BASE_NAME}" STREQUAL "alphaShapeWithInfo")
  set(CGALPY_TRI3_CELL_BASE ${CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_WITH_INFO})
elseif ("${CGALPY_TRI3_CELL_BASE_NAME}" STREQUAL "alphaShapeRegular")
  set(CGALPY_TRI3_CELL_BASE ${CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR})
elseif ("${CGALPY_TRI3_CELL_BASE_NAME}" STREQUAL "alphaShapeRegularWithInfo")
  set(CGALPY_TRI3_CELL_BASE ${CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO})
elseif ("${CGALPY_TRI3_CELL_BASE_NAME}" STREQUAL "fixedAlphaShape")
  set(CGALPY_TRI3_CELL_BASE ${CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE})
elseif ("${CGALPY_TRI3_CELL_BASE_NAME}" STREQUAL "fixedAlphaShapeWithInfo")
  set(CGALPY_TRI3_CELL_BASE ${CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_WITH_INFO})
elseif ("${CGALPY_TRI3_CELL_BASE_NAME}" STREQUAL "fixedAlphaShapeRegular")
  set(CGALPY_TRI3_CELL_BASE ${CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR})
elseif ("${CGALPY_TRI3_CELL_BASE_NAME}" STREQUAL "fixedAlphaShapeRegularWithInfo")
  set(CGALPY_TRI3_CELL_BASE ${CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO})
endif()
add_definitions(-DCGALPY_TRI3_CELL_BASE=${CGALPY_TRI3_CELL_BASE})

# Select 3D triangulation traits
if     ("${CGALPY_TRI3_TRAITS_NAME}" STREQUAL "kernel")
  set(CGALPY_TRI3_TRAITS ${CGALPY_TRI3_TRAITS_KERNEL})
elseif ("${CGALPY_TRI3_TRAITS_NAME}" STREQUAL "periodic3Delaunay")
  set(CGALPY_TRI3_TRAITS ${CGALPY_TRI3_TRAITS_PERIODIC3_DELAUNAY})
endif()
add_definitions(-DCGALPY_TRI3_TRAITS=${CGALPY_TRI3_TRAITS})

# Select 3D triangulation concurrency
if     ("${CGALPY_TRI3_CONCURRENCY_NAME}" STREQUAL "sequential")
  set(CGALPY_TRI3_CONCURRENCY ${CGALPY_TRI3_CONCURRENCY_SEQUENTIAL})
elseif ("${CGALPY_TRI3_CONCURRENCY_NAME}" STREQUAL "parallel")
  set(CGALPY_TRI3_CONCURRENCY ${CGALPY_TRI3_CONCURRENCY_PARALLEL})
endif()
add_definitions(-DCGALPY_TRI3_CONCURRENCY=${CGALPY_TRI3_CONCURRENCY})

# Select 3D triangulation location policy
if     ("${CGALPY_TRI3_LOCATION_POLICY_NAME}" STREQUAL "fast")
  set(CGALPY_TRI3_LOCATION_POLICY ${CGALPY_TRI3_LOCATION_POLICY_FAST})
elseif ("${CGALPY_TRI3_LOCATION_POLICY_NAME}" STREQUAL "compact")
  set(CGALPY_TRI3_LOCATION_POLICY ${CGALPY_TRI3_LOCATION_POLICY_COMPACT})
endif()
add_definitions(-DCGALPY_TRI3_LOCATION_POLICY=${CGALPY_TRI3_LOCATION_POLICY})

# Select 3D triangulation
if    ("${CGALPY_TRI3_NAME}" STREQUAL "plain")
  set(CGALPY_TRI3 ${CGALPY_TRI3_PLAIN})
elseif("${CGALPY_TRI3_NAME}" STREQUAL "regular")
  set(CGALPY_TRI3 ${CGALPY_TRI3_REGULAR})
elseif("${CGALPY_TRI3_NAME}" STREQUAL "delaunay")
  set(CGALPY_TRI3 ${CGALPY_TRI3_DELAUNAY})
elseif("${CGALPY_TRI3_NAME}" STREQUAL "periodic3Delaunay")
  set(CGALPY_TRI3 ${CGALPY_TRI3_PERIODIC3_DELAUNAY})
endif()
add_definitions(-DCGALPY_TRI3=${CGALPY_TRI3})

# Select Alpha shape
if    ("${CGALPY_ALPHA_SHAPE_NAME}" STREQUAL "plain")
  set(CGALPY_ALPHA_SHAPE ${CGALPY_ALPHA_SHAPE_PLAIN})
elseif("${CGALPY_ALPHA_SHAPE_NAME}" STREQUAL "fixed")
  set(CGALPY_ALPHA_SHAPE ${CGALPY_ALPHA_SHAPE_FIXED})
endif()
add_definitions(-DCGALPY_ALPHA_SHAPE=${CGALPY_ALPHA_SHAPE})

set(CGALPY_INSTALL_INC_DIR "include" CACHE STRING "The folder where CGALPY header files will be installed, relative to CMAKE_INSTALL_PREFIX")
set(CGALPY_INSTALL_LIB_DIR "lib"     CACHE STRING "The folder where CGALPY libraries will be installed, relative to CMAKE_INSTALL_PREFIX")
set(CGALPY_INSTALL_BIN_DIR "bin"
  CACHE STRING "The folder where CGALPY executables, e.g., binaried and scripts, will be installed, relative to CMAKE_INSTALL_PREFIX")
set(CGALPY_INSTALL_CMAKE_DIR "${CGALPY_INSTALL_LIB_DIR}/CGALPY"
  CACHE STRING "The folder where CGALPY CMake modules will be installed, relative to CMAKE_INSTALL_PREFIX")

# Add subdirectories
add_subdirectory(libs)
