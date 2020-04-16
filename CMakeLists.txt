cmake_minimum_required(VERSION 3.5)
if(POLICY CMP0011)
  cmake_policy(SET CMP0011 NEW)
endif()
project(cgalpy)

# Define options
option(BUILD_SHARED_LIBS "Build shared libs instead of static libs" ON)

# Kernel options
set(CGALPY_EPEC_KERNEL 0)
set(CGALPY_EPIC_KERNEL 1)
set(CGALPY_KERNEL_SHORT_NAMES "epec" "epic")
set(CGALPY_KERNEL_NAMES "epec" "epic")
# Default
set(CGALPY_KERNEL ${CGALPY_EPEC_KERNEL})
SET(CGALPY_KERNEL_NAME "epec" CACHE STRING "The kernel to use")
set_property(CACHE CGALPY_KERNEL_NAME PROPERTY STRINGS epec epic)

# Geometry Traits
set(CGALPY_ARR_LINEAR_GEOMETRY_TRAITS              0)
set(CGALPY_ARR_SEGMENT_GEOMETRY_TRAITS             1)
set(CGALPY_ARR_NON_CACHING_SEGMENT_GEOMETRY_TRAITS 2)
set(CGALPY_ARR_CONIC_GEOMETRY_TRAITS               3)
set(CGALPY_ARR_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS   4)
set(CGALPY_ARR_CIRCLE_SEGMENT_GEOMETRY_TRAITS      5)
set(CGALPY_ARR_BEZIER_GEOMETRY_TRAITS              6)
set(CGALPY_ARR_RATIONAL_GEOMETRY_TRAITS            7)

set(CGALPY_GEOMETRY_TRAITS_SHORT_NAMES linear seg ncseg conic alg cs)
set(CGALPY_GEOMETRY_TRAITS_NAMES linear segment nonCachingSegment conic algebraic circleSegment)
# Default
SET(CGALPY_GEOMETRY_TRAITS ${CGALPY_ARR_SEGMENT_GEOMETRY_TRAITS})
SET(CGALPY_GEOMETRY_TRAITS_NAME "segment" CACHE STRING "The geometry traits to use")
set_property(CACHE CGALPY_GEOMETRY_TRAITS_NAME PROPERTY STRINGS linear segment nonCachingSegment conic algebraic circleSegment)

# DCEL
set(CGALPY_ARR_PLAIN_DCEL 0)
set(CGALPY_ARR_FACE_EXTENDED_DCEL 1)
set(CGALPY_ARR_HALFEDGE_EXTENDED_DCEL 2)
set(CGALPY_ARR_VERTEX_EXTENDED_DCEL 3)
set(CGALPY_ARR_ALL_EXTENDED_DCEL 4)
set(CGALPY_DCEL_SHORT_NAMES "" "_fex" "_hex" "_vex" "_ex")
set(CGALPY_DCEL_NAMES "plain" "faceExtended" "halfedgeExtended" "vertexExtended" "allExtended")

# Default
SET(CGALPY_DCEL ${CGALPY_ARR_PLAIN_DCEL})
SET(CGALPY_DCEL_NAME "plain" CACHE STRING "The DCEL to use")
set_property(CACHE CGALPY_DCEL_NAME PROPERTY STRINGS plain faceExtended halfedgeExtended vertexExtended allExtended)

# Dimension
SET(CGALPY_DIMENSION 4 CACHE STRING "The dimension of spatial search objects")

# 2D Minkowsi sums
SET(CGALPY_MINKOWSKI_SUM_2_BINDINGS true CACHE BOOL "Compile bindings for 2D Minkowski sum")

# Triangulations
SET(CGALPY_TRIANGULATION_2_BINDINGS true CACHE BOOL "Compile bindings for triangulations")

# Spatial_searching_bindings
SET(CGALPY_SPATIAL_SEARCHING_BINDINGS true CACHE BOOL "Compile bindings for spatial searching")

# Convex hull
SET(CGALPY_CONVEX_HULL_BINDINGS true CACHE BOOL "Compile bindings for convex hull")

# Bounding volumes
SET(CGALPY_BOUNDING_VOLUMES_BINDINGS true CACHE BOOL "Compile bindings for bounding volumes")

# 2D Regularized Boolean set operations
SET(CGALPY_BOOLEAN_SET_OPERATIONS_BINDINGS true CACHE BOOL "Compile bindings for boolean set operations")

# Polygon partitioning
SET(CGALPY_POLYGON_PARTITIONING_BINDINGS true CACHE BOOL "Compile bindings for polygon partitioning")

# Point location
SET(CGALPY_POINT_LOCATION_BINDINGS true CACHE BOOL "Compile bindings for point location")

# 3D Triangulation Vertex Base
set(CGALPY_TRI3_VERTEX_BASE_PLAIN                                0)
set(CGALPY_TRI3_VERTEX_BASE_PLAIN_WITH_INFO                      1)
set(CGALPY_TRI3_VERTEX_BASE_REGULAR                              2)
set(CGALPY_TRI3_VERTEX_BASE_REGULAR_WITH_INFO                    3)
set(CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE                          4)
set(CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_WITH_INFO                5)
set(CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR                  6)
set(CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO        7)
set(CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE                    8)
set(CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_WITH_INFO          9)
set(CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR           10)
set(CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO 11)

set(CGALPY_TRI3_VERTEX_BASE_SHORT_NAMES plain pwi reg rwi as aswh asr asrwi fas faswi fasr fasrwi)
set(CGALPY_TRI3_VERTEX_BASE_NAMES plain plainWithInfo regular regualrWithInfo alphaShape alphaShapeWithInfo alphaShapeRegular alphaShapeRegularWithInfo fixedAlphaShape fixedAlphaShapeWithInfo fixedAlphaShapeRegular fixedAlphaShapeRegularWithInfo)
# Default
SET(CGALPY_TRI3VERTEX_BASE_ ${CGALPY_TRI3_VERTEX_BASE_PLAIN})
SET(CGALPY_TRI3_VERTEX_BASE_NAME "plain" CACHE STRING "The 3D Triangulation vertex base")
set_property(CACHE CGALPY_TRI3_VERTEX_BASE_NAME PROPERTY STRINGS plain plainWithInfo regular regualrWithInfo alphaShape alphaShapeWithInfo alphaShapeRegular alphaShapeRegularWithInfo fixedAlphaShape fixedAlphaShapeWithInfo fixedAlphaShapeRegular fixedAlphaShapeRegularWithInfo)

# 3D Triangulation Cell Base
set(CGALPY_TRI3_CELL_BASE_PLAIN                                0)
set(CGALPY_TRI3_CELL_BASE_PLAIN_WITH_INFO                      1)
set(CGALPY_TRI3_CELL_BASE_REGULAR                              2)
set(CGALPY_TRI3_CELL_BASE_REGULAR_WITH_INFO                    3)
set(CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE                          4)
set(CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_WITH_INFO                5)
set(CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR                  6)
set(CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO        7)
set(CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE                    8)
set(CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_WITH_INFO          9)
set(CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR           10)
set(CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO 11)

set(CGALPY_TRI3_CELL_BASE_SHORT_NAMES plain pwi reg rwi as aswh asr asrwi fas faswi fasr fasrwi)
set(CGALPY_TRI3_CELL_BASE_NAMES plain plainWithInfo regular regualrWithInfo alphaShape alphaShapeWithInfo alphaShapeRegular alphaShapeRegularWithInfo fixedAlphaShape fixedAlphaShapeWithInfo fixedAlphaShapeRegular fixedAlphaShapeRegularWithInfo)
# Default
SET(CGALPY_TRI3CELL_BASE_ ${CGALPY_TRI3_CELL_BASE_PLAIN})
SET(CGALPY_TRI3_CELL_BASE_NAME "plain" CACHE STRING "The 3D Triangulation cell base")
set_property(CACHE CGALPY_TRI3_CELL_BASE_NAME PROPERTY STRINGS plain plainWithInfo regular regualrWithInfo alphaShape alphaShapeWithInfo alphaShapeRegular alphaShapeRegularWithInfo fixedAlphaShape fixedAlphaShapeWithInfo fixedAlphaShapeRegular fixedAlphaShapeRegularWithInfo)

# 3D Triangulation traits
set(CGALPY_TRI3_TRAITS_KERNEL             0)
set(CGALPY_TRI3_TRAITS_PERIODIC3_DELAUNAY 1)

set(CGALPY_TRI3_TRAITS_SHORT_NAMES kernel p3d)
set(CGALPY_TRI3_TRAITS_NAMES kernel periodic3Delaunay)
# Default
SET(CGALPY_TRI3_TRAITS ${CGALPY_TRI3_TRAITS_KERNEL})
SET(CGALPY_TRI3_TRAITS_NAME "kernel" CACHE STRING "The 3D Triangulation traits")
set_property(CACHE CGALPY_TRI3_TRAITS_NAME PROPERTY STRINGS kernel periodic3Delaunay)

# 3D Triangulation concurrency
set(CGALPY_TRI3_CONCURRENCY_SEQUENTIAL 0)
set(CGALPY_TRI3_CONCURRENCY_PARALLEL   1)

set(CGALPY_TRI3_CONCURRENCY_SHORT_NAMES seq par)
set(CGALPY_TRI3_CONCURRENCY_NAMES sequential parallel)
# Default
SET(CGALPY_TRI3_CONCURRENCY ${CGALPY_TRI3_CONCURRENCY_SEQUENTIAL})
SET(CGALPY_TRI3_CONCURRENCY_NAME "sequential" CACHE STRING "The 3D Triangulation concurrency tag")
set_property(CACHE CGALPY_TRI3_CONCURRENCY_NAME PROPERTY STRINGS sequential parallel)

# 3D Triangulation location policy
set(CGALPY_TRI3_LOCATION_POLICY_FAST 0)
set(CGALPY_TRI3_LOCATION_POLICY_COMPACT 1)

set(CGALPY_TRI3_LOCATION_POLICY_SHORT_NAMES fast comp)
set(CGALPY_TRI3_LOCATION_POLICY_NAMES fast compact)
# Default
SET(CGALPY_TRI3_LOCATION_POLICY ${CGALPY_TRI3_LOCATION_POLICY_COMPACT})
SET(CGALPY_TRI3_LOCATION_POLICY_NAME "compact" CACHE STRING "The 3D Triangulation point location strategy")
set_property(CACHE CGALPY_TRI3_LOCATION_POLICY_NAME PROPERTY STRINGS fast compact)

# 3D Triangulations
set(CGALPY_TRI3_PLAIN              0)
set(CGALPY_TRI3_REGULAR            1)
set(CGALPY_TRI3_DELAUNAY           2)
set(CGALPY_TRI3_PERIODIC3_DELAUNAY 3)
set(CGALPY_TRI3_SHORT_NAMES plain reg del p3del)
set(CGALPY_TRI3_NAMES plain regular delaunay periodic3Delaunay)
# Default
SET(CGALPY_TRI3 ${CGALPY_TRI3_REGULAR})
SET(CGALPY_TRI3_NAME "regular" CACHE STRING "The 3D Triangulation")
set_property(CACHE CGALPY_TRI3_NAME PROPERTY STRINGS plain regular delaunay periodic3Delaunay)

# 3D comparison tag
SET(CGALPY_ALPHA_SHAPE_EXACT_COMPARISON true CACHE BOOL "The exact comparison tag")

# 3D Alpha Shapes
set(CGALPY_ALPHA_SHAPE_PLAIN 0)
set(CGALPY_ALPHA_SHAPE_FIXED 1)
set(CGALPY_ALPHA_SHAPE_SHORT_NAMES plain fixed)
set(CGALPY_ALPHA_SHAPE_NAMES plain fixed)
# Default
SET(CGALPY_ALPHA_SHAPE ${CGALPY_ALPHA_SHAPE_PLAIN})
SET(CGALPY_ALPHA_SHAPE_NAME "plain" CACHE STRING "The 3D Alpha shape construct")
set_property(CACHE CGALPY_ALPHA_SHAPE_NAME PROPERTY STRINGS plain fixed)

SET(CGALPY_ALPHA_SHAPES_3_BINDINGS false CACHE BOOL "Compile bindings for 3D Alpha Shapes")

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
set (CPACK_PACKAGE_VENDOR "Tel Aviv University")
set (CPACK_PACKAGE_VERSION_MAJOR "0")
set (CPACK_PACKAGE_VERSION_MINOR "1")
set (CPACK_PACKAGE_VERSION_PATCH "0")
include (CPack)
