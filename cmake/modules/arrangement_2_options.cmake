# Geometry Traits Options
set(CGALPY_ARR_2_LINEAR_GEOMETRY_TRAITS              0)
set(CGALPY_ARR_2_SEGMENT_GEOMETRY_TRAITS             1)
set(CGALPY_ARR_2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS 2)
set(CGALPY_ARR_2_CONIC_GEOMETRY_TRAITS               3)
set(CGALPY_ARR_2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS   4)
set(CGALPY_ARR_2_CIRCLE_SEGMENT_GEOMETRY_TRAITS      5)
set(CGALPY_ARR_2_BEZIER_GEOMETRY_TRAITS              6)
set(CGALPY_ARR_2_RATIONAL_GEOMETRY_TRAITS            7)

# Geometry Traits Names
set(CGALPY_ARR_2_GEOMETRY_TRAITS_SHORT_NAMES linear seg ncseg conic alg cs)
set(CGALPY_ARR_2_GEOMETRY_TRAITS_NAMES linear segment nonCachingSegment conic algebraic circleSegment)

# Geometry Traits Default
SET(CGALPY_ARR_2_GEOMETRY_TRAITS ${CGALPY_ARR_2_SEGMENT_GEOMETRY_TRAITS})
SET(CGALPY_ARR_2_GEOMETRY_TRAITS_NAME "segment" CACHE STRING "The geometry traits to use")
set_property(CACHE CGALPY_ARR_2_GEOMETRY_TRAITS_NAME PROPERTY STRINGS linear segment nonCachingSegment conic algebraic circleSegment)

# DCEL Options
set(CGALPY_ARR_2_PLAIN_DCEL 0)
set(CGALPY_ARR_2_FACE_EXTENDED_DCEL 1)
set(CGALPY_ARR_2_HALFEDGE_EXTENDED_DCEL 2)
set(CGALPY_ARR_2_VERTEX_EXTENDED_DCEL 3)
set(CGALPY_ARR_2_ALL_EXTENDED_DCEL 4)

# DCEL Names
set(CGALPY_ARR_2_DCEL_SHORT_NAMES "" "_fex" "_hex" "_vex" "_ex")
set(CGALPY_ARR_2_DCEL_NAMES "plain" "faceExtended" "halfedgeExtended" "vertexExtended" "allExtended")

# DCEL Default
SET(CGALPY_ARR_2_DCEL ${CGALPY_ARR_2_PLAIN_DCEL})
SET(CGALPY_ARR_2_DCEL_NAME "plain" CACHE STRING "The DCEL to use")
set_property(CACHE CGALPY_ARR_2_DCEL_NAME PROPERTY STRINGS plain faceExtended halfedgeExtended vertexExtended allExtended)

# Point location
option(CGALPY_ARR_2_POINT_LOCATION_BINDINGS "Compile bindings for point location" ON)
