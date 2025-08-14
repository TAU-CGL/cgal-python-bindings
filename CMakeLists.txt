cmake_minimum_required(VERSION 3.29.6)
if (POLICY CMP0011)
  cmake_policy(SET CMP0011 NEW)
endif ()

# Policy CMP0148 is set to old, because FindSphinx still uses FindPythonInterp
if (POLICY CMP0148)
  cmake_policy(SET CMP0148 OLD)
endif ()

# Policy CMP0167 is not set: The FindBoost module is removed.
if (POLICY CMP0167)
  cmake_policy(SET CMP0167 OLD)
endif ()

# The pyproject.toml file also sets the version.
# Ideally, the version would be set in a single place
# Similar to the minimum required version of cmake, which is set in this file,
# I would expect the version to be set here as well.
# Unfortunately, this is not possible.
# Therefore, when bumping up the number, do it in both places!
project(cgalpy VERSION "1.0.10" LANGUAGES CXX)

# Use C++17 for this directory and its sub-directories.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Include windows toolchain path (if available)
if (CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_TOOLCHAIN_FILE})
endif (CMAKE_TOOLCHAIN_FILE)

if (DEFINED ENV{CGAL_DATA_DIR})
    set(CGAL_DATA_DIR $ENV{CGAL_DATA_DIR})
endif ()

# General
set(CGALPY_PROJECT_AUTHORS "Nir Goren and Efi Fogel")
set(CGALPY_PROJECT_EMAILS "cgl@tau.ac.il")
set(CGALPY_PROJECT_URL "http://acg.cs.tau.ac.il")
set(CGALPY_MODULES_REL_DIR cmake/modules)
set(CGALPY_MODULE_DIR ${CMAKE_SOURCE_DIR}/${CGALPY_MODULES_REL_DIR})
set(CGALPY_PYTHOM_BUILD_CONFIG_SETTING CACHE STRING "")
set(CMAKE_MODULE_PATH ${CGALPY_MODULE_DIR})
include(CGALPY_Macros)

# General options
option(CGALPY_USE_SHARED_LIBS "Use shared libs versus static libs" ON)
option(CGALPY_BUILD_SHARED_LIBS "Build shared libs versus static libs" ON)
option(CGALPY_FIXED_LIBRARY_NAME "The library name is fixed" ON)
option(CGALPY_BUILD_DEV_DOC "Build the development documentation" OFF)
option(CGALPY_BUILD_DOC "Build the documentation" ON)
option(CGALPY_WITH_VISUAL "Display" ON)

set(default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
            STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

if (SKBUILD)
    # Get configuration from configuration file (default: config.cmake)
    set(CMAKE_CONFIG_FILE "${CMAKE_SOURCE_DIR}/config.cmake" CACHE STRING "Path to configuration file")
    include(${CMAKE_CONFIG_FILE})
endif()

# Binding options. Notice that The default value of option is OFF.
option(CGALPY_KERNEL_BINDINGS "Compile bindings for 2,3 Kernel" ON)
option(CGALPY_KERNEL_D_BINDINGS "Compile bindings for dD Kernel")
option(CGALPY_2D_RANGE_AND_NEIGHBOR_SEARCH_BINDINGS "Compile bindings for 2D Range and Neighbor Search")
option(CGALPY_3D_POINT_SET_BINDINGS "Compile bindings for 3D Point Set")
option(CGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS "Compile bindings for 2D Arrangements on Surfaces")
option(CGALPY_ALPHA_SHAPE_2_BINDINGS "Compile bindings for 2D Alpha Shapes")
option(CGALPY_ALPHA_SHAPE_3_BINDINGS "Compile bindings for 3D Alpha Shapes")
option(CGALPY_BASIC_VIEWER_BINDINGS "Compile bindings for basic viewers")
option(CGALPY_BGL_BINDINGS "Compile bindings for BGL")
option(CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS "Compile bindings for 2D boolean set operations")
option(CGALPY_BOUNDING_VOLUMES_BINDINGS "Compile bindings for bounding volumes")
option(CGALPY_CONVEX_HULL_2_BINDINGS "Compile bindings for 2D convex hull")
option(CGALPY_CONVEX_HULL_3_BINDINGS "Compile bindings for 3D convex hull")
option(CGALPY_ENVELOPE_2_BINDINGS "Compile bindings for 2D Envelopes")
option(CGALPY_ENVELOPE_3_BINDINGS "Compile bindings for 3D Envelopes")
option(CGALPY_GEOMETRIC_OBJECT_GENERATORS__BINDINGS "Compile bindings for 3D Envelopes")
option(CGALPY_KINETIC_SURFACE_RECONSTRUCTION_BINDINGS "Compile bindings for kinetic surface reconstruction")
option(CGALPY_MINKOWSKI_SUM_2_BINDINGS "Compile bindings for 2D Minkowski sum")
option(CGALPY_NEF_3_BINDINGS "Compile bindings for 3D Nef")
option(CGALPY_POLYHEDRON_3_BINDINGS "Compile bindings for 3D polyhedron")
option(CGALPY_POLYGON_2_BINDINGS "Compile bindings for 2D polygon")
option(CGALPY_POLYGON_MESH_PROCESSING_BINDINGS "Compile bindings for polygon mesh processing")
option(CGALPY_PMP_COMBINATORIAL_REPAIR_BINDINGS "Compile bindings for Polygon Mesh Processing combinatorial repair functions")
option(CGALPY_PMP_CONNECTED_COMPONENTS_BINDINGS "Compile bindings for Polygon Mesh Processing connected components functions")
option(CGALPY_PMP_COREFINEMENT_BINDINGS "Compile bindings for Polygon Mesh Processing corefinement functions")
option(CGALPY_PMP_CORRECTED_CURVATURE_COMPUTATION_BINDINGS "Compile bindings for Polygon Mesh Processing corrected curvature computation functions")
option(CGALPY_PMP_DISTANCE_BINDINGS "Compile bindings for Polygon Mesh Processing distance functions")
option(CGALPY_PMP_FEATURE_DETECTION_BINDINGS "Compile bindings for Polygon Mesh Processing feature detection functions")
option(CGALPY_PMP_GEOMETRIC_MEASURE BINDINGS "Compile bindings for Polygon Mesh Processing geometric measure functions")
option(CGALPY_PMP_GEOMETRIC_REPAIR_BINDINGS "Compile bindings for Polygon Mesh Processing geometric repair functions")
option(CGALPY_PMP_HOLE_FILLING_BINDINGS "Compile bindings for Polygon Mesh Processing hole filling functions")
option(CGALPY_PMP_IO "Compile bindings for Polygon Mesh Processing IO functions")
option(CGALPY_PMP_INTERSECTION_BINDINGS "Compile bindings for Polygon Mesh Processing intersection functions")
option(CGALPY_PMP_LOCATION_BINDINGS "Compile bindings for Polygon Mesh Processing location functions")
option(CGALPY_PMP_NORMAL_COMPUTATION_BINDINGS "Compile bindings for Polygon mesh processing normal computation functions")
option(CGALPY_PMP_ORIENTATION_BINDINGS "Compile bindings for Polygon Mesh Processing orientation functions")
option(CGALPY_PMP_MESHING_BINDINGS "Compile bindings for Polygon Mesh Processing distance functions")
option(CGALPY_POINT_SET_PROCESSING_BINDINGS "Compile bindings for point set processing")
option(CGALPY_TRIANGULATED_SURFACE_MESH_SEGMENTATION_BINDINGS "Compile bindings for surface mesh segmentation")
option(CGALPY_TRIANGULATED_SURFACE_MESH_SIMPLIFICATION_BINDINGS "Compile bindings for surface mesh simplification")
option(CGALPY_TRIANGULATED_SURFACE_MESH_SKELETONIZATION_BINDINGS "Compile bindings for surface mesh skeletonization")
option(CGALPY_POLYGON_PARTITIONING_BINDINGS "Compile bindings for polygon partitioning")
option(CGALPY_SPATIAL_SEARCHING_BINDINGS "Compile bindings for spatial searching")
option(CGALPY_SPATIAL_SORTING_BINDINGS "Compile bindings for spatial sorting")
option(CGALPY_SURFACE_MESH_BINDINGS "Compile bindings for surface mesh")
option(CGALPY_SURFACE_SWEEP_2_BINDINGS "Compile bindings for 2D surface sweep")
option(CGLAPY_SHAPE_DETECTION_BINDINGS "Compile bindings for shape detection")
option(CGALPY_TRIANGULATION_2_BINDINGS "Compile bindings for 2D triangulations")
option(CGALPY_TRIANGULATION_3_BINDINGS "Compile bindings for 3D triangulations")
option(CGALPY_TRIANGULATION_D_BINDINGS "Compile bindings for dD triangulations")
option(CGALPY_AOS2_POINT_LOCATION_BINDINGS "Compile bindings for point location")
option(CGALPY_KERNEL_INTERSECTION_BINDINGS "Compile bindings for intersections")
option(CGALPY_KERNEL_D_BINDINGS "Compile bindings for dD Kernel")
option(CGALPY_VISIBILITY_2_BINDINGS "Compile bindings for 2D Visibility")

include(basic_viewer_options)
include(polygonal_mesh_options) # needs to be before packages using polygonal_mesh
include(kinetic_surface_reconstruction_options) # needs to be before 3D Point Set
include(point_set_processing_options) # needs to be before 3D Point Set
include(three_d_point_set_options)
include(two_d_range_and_neighbor_search_options)
include(alpha_shape_2_options)
include(alpha_shape_3_options)
include(arrangement_on_surface_2_options)
include(bgl_options)
include(boolean_set_operations_2_options)
include(bounding_volumes_options)
include(convex_hull_2_options)
include(convex_hull_3_options)
include(envelope_2_options)
include(envelope_3_options)
include(geometric_object_generators_options)
include(kernel_options)
include(kernel_d_options)
include(minkowski_sum_2)
include(nef_3_options)
include(polyhedron_3_options)
include(polygon_2_options)
include(polygon_mesh_processing_options)
include(polygon_partitioning_options)
include(spatial_searching_options)
include(spatial_sorting_options)
include(surface_mesh_options)
include(surface_sweep_2_options)
include(shape_detection_options)
include(triangulation_2_options)
include(triangulation_3_options)
include(triangulation_d_options)
include(triangulated_surface_mesh_segmentation_options)
include(triangulated_surface_mesh_simplification_options)
include(triangulated_surface_mesh_skeletonization_options)
include(visibility_2_options)

if (SKBUILD)
    # Create a default profile if it does not exist.
    # All the settings in the profile are overriden, so essentially a profile
    # is not needed, but conan requires a place holder.
    execute_process(COMMAND conan profile detect --exist-ok)

    # Determine the settings based on the current platform and build type
    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(OS_SETTING -s os=Windows)
        # Assume msvc; however, can also be MinGW or clang
        set(COMPILER_SETTING -s compiler=msvc)
        set(LIBCXX_SETTING ) # No libcxx setting for MSVC
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(OS_SETTING -s os=Linux)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(COMPILER_SETTING -s compiler=gcc)
        elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(COMPILER_SETTING -s compiler=clang)
        endif ()
        set(LIBCXX_SETTING -s compiler.libcxx=libstdc++11)
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(OS_SETTING -s os=Macos)
        set(COMPILER_SETTING -s compiler=apple-clang)
        set(LIBCXX_SETTING -s compiler.libcxx=libc++)
    endif ()

    # Build type (Debug/Release)
    if (CMAKE_BUILD_TYPE)
        set(BUILD_TYPE_SETTING -s build_type=${CMAKE_BUILD_TYPE})
    endif ()

    # Compile version
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        string(REGEX REPLACE "^([0-9]+)\\.([0-9]).*$" "\\1\\2" COMPILER_VERSION_NUM "${CMAKE_CXX_COMPILER_VERSION}")
        set(COMPILER_VERSION -s compiler.version=${COMPILER_VERSION_NUM})

    elseif (CMAKE_CXX_COMPILER_VERSION)
        # Extract only the major version
        string(REGEX MATCH "^[0-9]+" COMPILER_MAJOR_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
        set(COMPILER_VERSION -s compiler.version=${COMPILER_MAJOR_VERSION})
    else()
        set(COMPILER_VERSION -s compiler.version=11) # Fallback if version is not detected
    endif()

    # C++ Standard
    set(CPPSTD_SETTING -s compiler.cppstd=${CMAKE_CXX_STANDARD})

    execute_process(
        COMMAND conan install .
            ${OS_SETTING}
            ${COMPILER_SETTING}
            ${COMPILER_VERSION}
            ${CPPSTD_SETTING}
            ${LIBCXX_SETTING}
            ${BUILD_TYPE_SETTING}
            --profile default
            --build missing
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error
    )
    if (result)
        message(FATAL_ERROR "Conan install failed: ${error}")
    endif ()
    set(TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/generators/conan_toolchain.cmake)
    include(${TOOLCHAIN_FILE})
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
endif () # SKBUILD

message(STATUS "VERSION=${CMAKE_PROJECT_VERSION}")
message(STATUS "SOVERSION=${CMAKE_PROJECT_VERSION_MAJOR}")

# Add src subdirectory
add_subdirectory(src)

include(InstallRequiredSystemLibraries)

if (NOT SKBUILD)
    set(CPACK_GENERATOR TGZ)
    set(CPACK_PACKAGE_VENDOR "taucgl")
    set(CPACK_PACKAGE_VERSION_MAJOR "1")
    set(CPACK_PACKAGE_VERSION_MINOR "0")
    set(CPACK_PACKAGE_VERSION_PATCH "5")
    include(CPack)
endif ()
