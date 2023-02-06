set(CGALPY_PY_MIN_VERSION 3.5)
set(CGALPY_BOOST_MIN_VERSION 1.68.0)

if (CGALPY_WITH_VISUAL)
  add_definitions(-DCGALPY_HAS_VISUAL=)
endif()

# Find required libraries
if (NOT SKBUILD)
  if (CGALPY_WITH_VISUAL)
    find_package(CGAL REQUIRED COMPONENTS Core Qt5)
  else()
    find_package(CGAL REQUIRED COMPONENTS Core)
  endif()
endif()

if (NOT CGAL_FOUND)
    message(STATUS "This library cannot be compiled, as it requires CGAL.")
    return()
endif ()


if (NOT SKBUILD)
    include(${CGAL_USE_FILE})

    if (CGALPY_USE_SHARED_LIBS)
        set(Boost_USE_STATIC_LIBS OFF)
    else ()
        set(Boost_USE_STATIC_LIBS ON)
        add_definitions(-DBOOST_PYTHON_STATIC_LIB)
    endif ()

    find_package(Python ${CGALPY_PY_MIN_VERSION} COMPONENTS Interpreter Development)
    # The following is not needed anylonger
    # find_package(PythonLibs ${Python_VERSION_MAJOR}.${Python_VERSION_MINOR} EXACT REQUIRED)
    find_package(Boost ${CGALPY_BOOST_MIN_VERSION} REQUIRED COMPONENTS system thread)
    if (NOT Boost_FOUND)
        message(STATUS "This library cannot be compiled, as it requires Boost.")
        return()
    endif ()

else()
    find_package(Boost REQUIRED)
endif ()

find_package(nanobind CONFIG REQUIRED)

# Add includes and libs
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})

# Add some compiler options
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(-GR)
    add_compile_options(-vmg)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    add_compile_options(-Wno-error=non-pod-varargs)
    message(WARNING "detected Clang")
endif ()

# Use C++17 for this directory and its sub-directories.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (NOT SKBUILD)
    set(CGALPY_INSTALL_INC_DIR "include"
            CACHE STRING "The folder where CGALPY header files will be installed, relative to CMAKE_INSTALL_PREFIX")
    set(CGALPY_INSTALL_LIB_DIR "lib"
            CACHE STRING "The folder where CGALPY libraries will be installed, relative to CMAKE_INSTALL_PREFIX")
    set(CGALPY_INSTALL_BIN_DIR "bin"
            CACHE STRING "The folder where CGALPY executables, e.g., binaries and scripts, will be installed, relative to CMAKE_INSTALL_PREFIX")
    set(CGALPY_INSTALL_CMAKE_DIR "${CGALPY_INSTALL_LIB_DIR}/CGALPY"
            CACHE STRING "The folder where CGALPY CMake modules will be installed, relative to CMAKE_INSTALL_PREFIX")
    set(CGALPY_INSTALL_PYTHON_DIR "${CGALPY_INSTALL_LIB_DIR}/Python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}/dist-packages"
            CACHE STRING "The folder where CGALPY CMake modules will be installed, relative to CMAKE_INSTALL_PREFIX")
endif ()
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
select_surface_mesh()
select_polyhedron_3()
select_polygon_mesh_processing()
select_visibility_2()

if (${CGALPY_KERNEL_D_BINDINGS} AND
((${CGALPY_KERNEL_D} EQUAL ${CGALPY_KERNEL_D_EPIC_D}) OR
(${CGALPY_KERNEL_D} EQUAL ${CGALPY_KERNEL_D_EPEC_D})))
  if (NOT SKBUILD)
    find_package(Eigen3 3.3 REQUIRED)
    if (NOT EIGEN3_FOUND)
        SET(CGALPY_KERNEL_D_BINDINGS OFF)
        SET(CGALPY_SPATIAL_SEARCHING_BINDINGS OFF)
        message(WARNING "Eigen3 not found!\nTurning of the generations of\n(i) dD Kernel, and\n(ii) spatial searching bindings")
    endif()
  endif()
  #  include(CGAL_Eigen3_support)
  include_directories(${EIGEN3_INCLUDE_DIR})
endif ()


# Add subdirectories
add_subdirectory(libs)
