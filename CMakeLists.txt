cmake_minimum_required(VERSION 3.14)
if(POLICY CMP0011)
  cmake_policy(SET CMP0011 NEW)
endif()
project(cgalpy VERSION "1.0.0" LANGUAGES CXX)

# General
set(CGALPY_PROJECT_AUTHORS "Nir Goren and Efi Fogel")
set(CGALPY_PROJECT_EMAILS "cgl@tau.ac.il")
set(CGALPY_PROJECT_URL "http://acg.cs.tau.ac.il")
set(CGALPY_MODULES_REL_DIR cmake/modules)
set(CGALPY_MODULE_DIR ${CMAKE_SOURCE_DIR}/${CGALPY_MODULES_REL_DIR})
set(CMAKE_MODULE_PATH ${CGALPY_MODULE_DIR})
include(CGALPY_Macros)

# General options
option(CGALPY_USE_SHARED_LIBS "Use shared libs versus static libs" ON)
option(CGALPY_BUILD_SHARED_LIBS "Build shared libs versus static libs" ON)
option(CGALPY_FIXED_LIBRARY_NAME "The library name is fixed" ON)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
    STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Binding options. Notice that The default value of option is OFF.
option(CGALPY_KERNEL_BINDINGS                   "Compile bindings for 2,3 Kernel" ON)
option(CGALPY_KERNEL_D_BINDINGS                 "Compile bindings for dD Kernel")
option(CGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS "Compile bindings for 2D Arrangements on Surfaces")
option(CGALPY_ALPHA_SHAPE_2_BINDINGS            "Compile bindings for 2D Alpha Shapes")
option(CGALPY_ALPHA_SHAPE_3_BINDINGS            "Compile bindings for 3D Alpha Shapes")
option(CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS "Compile bindings for 2D boolean set operations")
option(CGALPY_BOUNDING_VOLUMES_BINDINGS         "Compile bindings for bounding volumes")
option(CGALPY_CONVEX_HULL_2_BINDINGS            "Compile bindings for 2D convex hull")
option(CGALPY_CONVEX_HULL_3_BINDINGS            "Compile bindings for 3D convex hull")
option(CGALPY_POLYGON_2_BINDINGS                "Compile bindings for 2D polygon")
option(CGALPY_POLYGON_PARTITIONING_BINDINGS     "Compile bindings for polygon partitioning")
option(CGALPY_MINKOWSKI_SUM_2_BINDINGS          "Compile bindings for 2D Minkowski sum")
option(CGALPY_SPATIAL_SEARCHING_BINDINGS        "Compile bindings for spatial searching")
option(CGALPY_TRIANGULATION_2_BINDINGS          "Compile bindings for 2D triangulations")
option(CGALPY_TRIANGULATION_3_BINDINGS          "Compile bindings for 3D triangulations")
option(CGALPY_AOS2_POINT_LOCATION_BINDINGS      "Compile bindings for point location")
option(CGALPY_KERNEL_INTERSECTION_BINDINGS      "Compile bindings for intersections")
option(CGALPY_KERNEL_D_BINDINGS                 "Compile bindings for dD Kernel")

include(alpha_shape_2_options)
include(alpha_shape_3_options)
include(arrangement_on_surface_2_options)
include(boolean_set_operations_2_options)
include(bounding_volumes_options)
include(convex_hull_2_options)
include(kernel_options)
include(kernel_d_options)
include(polygon_2_options)
include(minkowski_sum_2)
include(polygon_partitioning_options)
include(spatial_searching_options)
include(triangulation_2_options)
include(triangulation_3_options)

message(STATUS "VERSION=${CMAKE_PROJECT_VERSION}")
message(STATUS "SOVERSION=${CMAKE_PROJECT_VERSION_MAJOR}")

# Add src subdirectory
add_subdirectory(src)

include(InstallRequiredSystemLibraries)

set(CPACK_GENERATOR TGZ)
set(CPACK_PACKAGE_VENDOR "taucgl")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
include(CPack)
