cmake_minimum_required(VERSION 3.14)
if (POLICY CMP0011)
  cmake_policy(SET CMP0011 NEW)
endif()
# Policy CMP0148 is set to old, because FindSphinx still uses FindPythonInterp
if (POLICY CMP0148)
  cmake_policy(SET CMP0148 OLD)
endif()
project(cgalpy VERSION "1.0.4" LANGUAGES CXX) # the pyproject.toml file sets the version

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

# Binding options. Notice that The default value of option is OFF.
option(CGALPY_KERNEL_BINDINGS "Compile bindings for 2,3 Kernel" ON)
option(CGALPY_KERNEL_D_BINDINGS "Compile bindings for dD Kernel")
option(CGALPY_2D_RANGE_AND_NEIGHBOR_SEARCH_BINDINGS "Compile bindings for 2D Range and Neighbor Search")
option(CGALPY_3D_POINT_SET_BINDINGS "Compile bindings for 3D Point Set")
option(CGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS "Compile bindings for 2D Arrangements on Surfaces")
option(CGALPY_ALPHA_SHAPE_2_BINDINGS "Compile bindings for 2D Alpha Shapes")
option(CGALPY_ALPHA_SHAPE_3_BINDINGS "Compile bindings for 3D Alpha Shapes")
option(CGALPY_BASIC_VIEWER_BINDINGS "Compile bindings for basic viewers")
option(CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS "Compile bindings for 2D boolean set operations")
option(CGALPY_BOUNDING_VOLUMES_BINDINGS "Compile bindings for bounding volumes")
option(CGALPY_CONVEX_HULL_2_BINDINGS "Compile bindings for 2D convex hull")
option(CGALPY_CONVEX_HULL_3_BINDINGS "Compile bindings for 3D convex hull")
option(CGALPY_ENVELOPE_2_BINDINGS "Compile bindings for 2D Envelopes")
option(CGALPY_ENVELOPE_3_BINDINGS "Compile bindings for 3D Envelopes")
option(CGALPY_POLYHEDRON_3_BINDINGS "Compile bindings for 3D polyhedron")
option(CGALPY_POLYGON_2_BINDINGS "Compile bindings for 2D polygon")
option(CGALPY_POLYGON_MESH_PROCESSING_BINDINGS "Compile bindings for polygon mesh processing")
option(CGALPY_POINT_SET_PROCESSING_BINDINGS "Compile bindings for point set processing")
option(CGALPY_TRIANGULATED_SURFACE_MESH_SEGMENTATION_BINDINGS "Compile bindings for surface mesh segmentation")
option(CGALPY_TRIANGULATED_SURFACE_MESH_SIMPLIFICATION_BINDINGS "Compile bindings for surface mesh simplification")
option(CGALPY_TRIANGULATED_SURFACE_MESH_SKELETONIZATION_BINDINGS "Compile bindings for surface mesh skeletonization")
option(CGALPY_KINETIC_SURFACE_RECONSTRUCTION_BINDINGS "Compile bindings for kinetic surface reconstruction")
option(CGALPY_POLYGON_PARTITIONING_BINDINGS "Compile bindings for polygon partitioning")
option(CGALPY_MINKOWSKI_SUM_2_BINDINGS "Compile bindings for 2D Minkowski sum")
option(CGALPY_SPATIAL_SEARCHING_BINDINGS "Compile bindings for spatial searching")
option(CGALPY_SURFACE_MESH_BINDINGS "Compile bindings for surface mesh")
option(CGALPY_SURFACE_SWEEP_2_BINDINGS "Compile bindings for 2D surface sweep")
option(CGLAPY_SHAPE_DETECTION_BINDINGS "Compile bindings for shape detection")
option(CGALPY_TRIANGULATION_2_BINDINGS "Compile bindings for 2D triangulations")
option(CGALPY_TRIANGULATION_3_BINDINGS "Compile bindings for 3D triangulations")
option(CGALPY_AOS2_POINT_LOCATION_BINDINGS "Compile bindings for point location")
option(CGALPY_KERNEL_INTERSECTION_BINDINGS "Compile bindings for intersections")
option(CGALPY_KERNEL_D_BINDINGS "Compile bindings for dD Kernel")
option(CGALPY_VISIBILITY_2_BINDINGS "Compile bindings for 2D Visibility")

if (SKBUILD)  # scikit-build
    # this block was added for scikit-build, but should also be fine for
    # manual builds.
    #~~~~~~~~~~~~~~~~~~~ CPM Dependencies ~~~~~~~~~~~~~~~~
    # CPM allows us to include some simple dependencies without much fuss.
    # Great default, for everything more complicated, uses conan.
    # Why not always use conan? CPM works without any interaction (conan needs
    # to be called externally). CPM also has no problems with install PyBind11
    # (which conan has at the time).
    # If the dependency has complicated dependencies on its own or is slow to
    # compile, better use conan.
    # include(./cmake/CPM.cmake)  # Package manager for simple requirements.

    # further CPM-dependencies may be defined in the subdirectories
    # CPMAddPackage("gh:wjakob/nanobind@2.1.0")
    find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
    find_package(nanobind CONFIG REQUIRED)
    #~~~~~~~~~~~~~~~~~ Conan dependencies ~~~~~~~~~~~~~~~~~~~~~
    # We are using the `cmake_find_package` generator to create `FindXXX`-files
    # in the `cmake`-folder. This is only necessary, if the packages are not
    # already installed. This approach is not without its problems:
    # PRO: Simple option that is compatible with scikit-build. Can work without conan.
    # CON: Difficult if you want to use changing conan settings.

    # Create a conan2 profile
    execute_process(COMMAND conan profile detect --exist-ok)
    execute_process(COMMAND conan install ${CMAKE_SOURCE_DIR} --build=missing)
    set(TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/build/Release/generators/conan_toolchain.cmake)
    include(${TOOLCHAIN_FILE})

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

    find_package(CGAL REQUIRED)
    find_package(Boost REQUIRED)
    include_directories(${cgal_INCLUDE_DIRS_RELEASE})

    # ~~~~~~~~~ nanobind  ~~~~~~~~~~~~~~~~~~
    # The following part is copied from
    # https://github.com/wjakob/nanobind_example/blob/master/CMakeLists.txt
    # with slight modifications.
    # Constrain FindPython to find the Python version used by scikit-build
    set(Python_VERSION "${PYTHON_VERSION_STRING}")
    set(Python_EXECUTABLE "${PYTHON_EXECUTABLE}")
    # The following line will result in conflicts with conan.
    #  set(Python_INCLUDE_DIR "${PYTHON_INCLUDE_DIR}")
    set(Python_LIBRARIES "${PYTHON_LIBRARY}")

    # Get configuration from config.cmake
    include(${CMAKE_SOURCE_DIR}/config.cmake)

    if (MSVC)
        # MSVC needs a little extra help finding the Python library
        find_package(PythonInterp)
        find_package(Python)
    endif () # MSVC
endif () # SKBUILD

include(basic_viewer_options)
include(polygonal_mesh_options) # needs to be before packages using polygonal_mesh
include(kinetic_surface_reconstruction_options) # needs to be before 3D Point Set
include(point_set_processing_options) # needs to be before 3D Point Set
include(three_d_point_set_options)
include(two_d_range_and_neighbor_search_options)
include(alpha_shape_2_options)
include(alpha_shape_3_options)
include(arrangement_on_surface_2_options)
include(boolean_set_operations_2_options)
include(bounding_volumes_options)
include(convex_hull_2_options)
include(convex_hull_3_options)
include(envelope_2_options)
include(envelope_3_options)
include(kernel_options)
include(kernel_d_options)
include(minkowski_sum_2)
include(polyhedron_3_options)
include(polygon_2_options)
include(polygon_mesh_processing_options)
include(polygon_partitioning_options)
include(spatial_searching_options)
include(surface_mesh_options)
include(surface_sweep_2_options)
include(shape_detection_options)
include(triangulation_2_options)
include(triangulation_3_options)
include(triangulated_surface_mesh_segmentation_options)
include(triangulated_surface_mesh_simplification_options)
include(triangulated_surface_mesh_skeletonization_options)
include(visibility_2_options)

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
