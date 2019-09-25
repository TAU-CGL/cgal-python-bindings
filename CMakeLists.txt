cmake_minimum_required(VERSION 3.5)
if(POLICY CMP0011)
  cmake_policy(SET CMP0011 NEW)
endif()
project(cgalpy)

# Define options
option(BUILD_SHARED_LIBS "Build shared libs instead of static libs" ON)
option(CGALPY_DEF_PYBINDINGS "Define Python bindings" OFF)

# Kernel options
set(CGALPY_EPEC_KERNEL 0)
set(CGALPY_EPIC_KERNEL 1)
set(CGALPY_KERNEL_SHORT_NAMES "epec" "epic")
set(CGALPY_KERNEL_NAMES "epec" "epic")
# Default
set(CGALPY_KERNEL ${CGALPY_EPEC_KERNEL})
SET(CGALPY_KERNEL_NAME "epec" CACHE STRING "The kernel to use")

# Geometry Traits
set(CGALPY_ARR_LINEAR_GEOMETRY_TRAITS 0)
set(CGALPY_ARR_SEGMENT_GEOMETRY_TRAITS 1)
set(CGALPY_ARR_NON_CACHING_SEGMENT_GEOMETRY_TRAITS 2)
set(CGALPY_ARR_CONIC_GEOMETRY_TRAITS 3)
set(CGALPY_ARR_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS 4)
set(CGALPY_ARR_CIRCLE_SEGMENT_GEOMETRY_TRAITS 5)
set(CGALPY_GEOMETRY_TRAITS_SHORT_NAMES linear seg ncseg conic alg cs)
set(CGALPY_GEOMETRY_TRAITS_NAMES linear segment nonCachingSegment conic algebraic circleSegment)
# Default
SET(CGALPY_GEOMETRY_TRAITS ${CGALPY_ARR_SEGMENT_GEOMETRY_TRAITS})
SET(CGALPY_GEOMETRY_TRAITS_NAME "segment" CACHE STRING "The geometry traits to use")

# DCEL
set(CGALPY_ARR_PLAIN_DCEL 0)
set(CGALPY_ARR_FACE_EXTENDED_DCEL 1)
set(CGALPY_ARR_HALFEDGE_EXTENDED_DCEL 2)
set(CGALPY_ARR_VERTEX_EXTENDED_DCEL 3)
set(CGALPY_ARR_ALL_EXTENDED_DCEL 4)
set(CGALPY_DCEL_SHORT_NAMES "" "fex" "hex" "vex" "ex")
set(CGALPY_DCEL_NAMES "plain" "faceExtended" "halfedgeExtended" "vertexExtended" "allExtended")
#
SET(CGALPY_DCEL ${CGALPY_ARR_PLAIN_DCEL})
SET(CGALPY_DCEL_NAME "plain" CACHE STRING "The DCEL to use")

# General
set(CGALPY_MODULES_REL_DIR cmake/modules)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/${CGALPY_MODULES_REL_DIR})

# SONAME, SOVERSION
#
# The rule is that each new release or bug fix release should increse the soversion.
#
# SOVERSION is $(SONAME_VERSION).$(SOVERSION_MINOR).$(SOVERSION_RELEASE)
#
# - If the binary interface of libraries do not change from previous release
#   (example: most bug fix releases), increase SOVERSION_RELEASE (this third number).
# - If the binary interface is changed, but remains compatible with
#   previous release (example: only addition of new functions), then increase
#   SOVERSION_MINOR (second number) and set SOVERSION_RELEASE to 0.
# - If the binary interface is changed in an incompatible way to previous
#   release, then increase the SONAME_VERSION, and set the two other
#   numbers to 0.
#
# SOVERSION history:
#   CGALPY-0.1  : 0.1.0
#
set(CGALPY_SONAME_VERSION "0")
set(CGALPY_SOVERSION "0.1.0")
message(STATUS "CGALPY_SONAME_VERSION=${CGALPY_SONAME_VERSION}")
message(STATUS "CGALPY_SOVERSION=${CGALPY_SOVERSION}")

# Add src subdirectory
add_subdirectory(src)

include (InstallRequiredSystemLibraries)

set (CPACK_GENERATOR TGZ)
set (CPACK_PACKAGE_VENDOR "Xenia Optimal Ltd")
set (CPACK_PACKAGE_VERSION_MAJOR "0")
set (CPACK_PACKAGE_VERSION_MINOR "1")
set (CPACK_PACKAGE_VERSION_PATCH "0")
include (CPack)
