set(PY_MIN_VERSION 3.5)
set(BOOST_MIN_VERSION 1.68.0)

# Find required libraries
find_package(CGAL REQUIRED COMPONENTS Core)
if (NOT CGAL_FOUND)
  message(STATUS "This library cannot be compiled, as it requires CGAL.")
  return()
endif()
include(${CGAL_USE_FILE})

if(CGALPY_USE_SHARED_LIBS)
  set(Boost_USE_STATIC_LIBS OFF)
else()
  set(Boost_USE_STATIC_LIBS ON)
  add_definitions(-DBOOST_PYTHON_STATIC_LIB)
endif()
set(CGALPY_BOOST_LIBRARIES)

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

# Use C++17 for this directory and its sub-directories.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CGALPY_INSTALL_INC_DIR "include"
    CACHE STRING "The folder where CGALPY header files will be installed, relative to CMAKE_INSTALL_PREFIX")
set(CGALPY_INSTALL_LIB_DIR "lib"
    CACHE STRING "The folder where CGALPY libraries will be installed, relative to CMAKE_INSTALL_PREFIX")
set(CGALPY_INSTALL_BIN_DIR "bin"
    CACHE STRING "The folder where CGALPY executables, e.g., binaries and scripts, will be installed, relative to CMAKE_INSTALL_PREFIX")
set(CGALPY_INSTALL_CMAKE_DIR "${CGALPY_INSTALL_LIB_DIR}/CGALPY"
    CACHE STRING "The folder where CGALPY CMake modules will be installed, relative to CMAKE_INSTALL_PREFIX")

select_kernel()
select_kernel_d()
select_arrangement_2()
select_polygon_2()
select_minkowski_sum_2()
select_triangulation_2()
select_alpha_shape_2()
select_spatial_searching()
select_convex_hull_2()
select_bounding_volumes()
select_boolean_set_operations_2()
select_polygon_partitioning()
select_triangulation_3()
select_alpha_shape_3()


if (${CGALPY_KERNEL_D_BINDINGS} AND
    ((${CGALPY_KERNEL_D} EQUAL ${CGALPY_KERNEL_D_EPIC_D}) OR
    (${CGALPY_KERNEL_D} EQUAL ${CGALPY_KERNEL_D_EPEC_D})))
  find_package(Eigen3 3.3.5)
  if (NOT EIGEN3_FOUND)
    SET(CGALPY_KERNEL_D_BINDINGS OFF)
    SET(CGALPY_SPATIAL_SEARCHING_BINDINGS OFF)
    message(WARNING "Eigen3 not found!\nTurning of the generations of\n(i) dD Kernel, and\n(ii) spatial searching bindings")
  endif()
  include(CGAL_Eigen3_support)
endif()

# Add subdirectories
add_subdirectory(libs)
