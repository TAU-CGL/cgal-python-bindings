# 3D Triangulation Vertex Base
set(CGALPY_TRI_3_VERTEX_BASE_PLAIN                                0)
set(CGALPY_TRI_3_VERTEX_BASE_PLAIN_WITH_INFO                      1)
set(CGALPY_TRI_3_VERTEX_BASE_REGULAR                              2)
set(CGALPY_TRI_3_VERTEX_BASE_REGULAR_WITH_INFO                    3)
set(CGALPY_TRI_3_VERTEX_BASE_ALPHA_SHAPE                          4)
set(CGALPY_TRI_3_VERTEX_BASE_ALPHA_SHAPE_WITH_INFO                5)
set(CGALPY_TRI_3_VERTEX_BASE_ALPHA_SHAPE_REGULAR                  6)
set(CGALPY_TRI_3_VERTEX_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO        7)
set(CGALPY_TRI_3_VERTEX_BASE_FIXED_ALPHA_SHAPE                    8)
set(CGALPY_TRI_3_VERTEX_BASE_FIXED_ALPHA_SHAPE_WITH_INFO          9)
set(CGALPY_TRI_3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR           10)
set(CGALPY_TRI_3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO 11)

set(CGALPY_TRI_3_VERTEX_BASE_SHORT_NAMES plain pwi reg rwi as aswh asr asrwi fas faswi fasr fasrwi)
set(CGALPY_TRI_3_VERTEX_BASE_NAMES plain plainWithInfo regular regualrWithInfo alphaShape alphaShapeWithInfo alphaShapeRegular alphaShapeRegularWithInfo fixedAlphaShape fixedAlphaShapeWithInfo fixedAlphaShapeRegular fixedAlphaShapeRegularWithInfo)
# Default
SET(CGALPY_TRI_3_VERTEX_BASE_ ${CGALPY_TRI_3_VERTEX_BASE_PLAIN})
SET(CGALPY_TRI_3_VERTEX_BASE_NAME "plain" CACHE STRING "The 3D Triangulation vertex base")
set_property(CACHE CGALPY_TRI_3_VERTEX_BASE_NAME PROPERTY STRINGS plain plainWithInfo regular regualrWithInfo alphaShape alphaShapeWithInfo alphaShapeRegular alphaShapeRegularWithInfo fixedAlphaShape fixedAlphaShapeWithInfo fixedAlphaShapeRegular fixedAlphaShapeRegularWithInfo)

# 3D Triangulation Cell Base
set(CGALPY_TRI_3_CELL_BASE_PLAIN                                0)
set(CGALPY_TRI_3_CELL_BASE_PLAIN_WITH_INFO                      1)
set(CGALPY_TRI_3_CELL_BASE_REGULAR                              2)
set(CGALPY_TRI_3_CELL_BASE_REGULAR_WITH_INFO                    3)
set(CGALPY_TRI_3_CELL_BASE_ALPHA_SHAPE                          4)
set(CGALPY_TRI_3_CELL_BASE_ALPHA_SHAPE_WITH_INFO                5)
set(CGALPY_TRI_3_CELL_BASE_ALPHA_SHAPE_REGULAR                  6)
set(CGALPY_TRI_3_CELL_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO        7)
set(CGALPY_TRI_3_CELL_BASE_FIXED_ALPHA_SHAPE                    8)
set(CGALPY_TRI_3_CELL_BASE_FIXED_ALPHA_SHAPE_WITH_INFO          9)
set(CGALPY_TRI_3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR           10)
set(CGALPY_TRI_3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO 11)

set(CGALPY_TRI_3_CELL_BASE_SHORT_NAMES plain pwi reg rwi as aswh asr asrwi fas faswi fasr fasrwi)
set(CGALPY_TRI_3_CELL_BASE_NAMES plain plainWithInfo regular regualrWithInfo alphaShape alphaShapeWithInfo alphaShapeRegular alphaShapeRegularWithInfo fixedAlphaShape fixedAlphaShapeWithInfo fixedAlphaShapeRegular fixedAlphaShapeRegularWithInfo)
# Default
SET(CGALPY_TRI_3_CELL_BASE_ ${CGALPY_TRI_3_CELL_BASE_PLAIN})
SET(CGALPY_TRI_3_CELL_BASE_NAME "plain" CACHE STRING "The 3D Triangulation cell base")
set_property(CACHE CGALPY_TRI_3_CELL_BASE_NAME PROPERTY STRINGS plain plainWithInfo regular regualrWithInfo alphaShape alphaShapeWithInfo alphaShapeRegular alphaShapeRegularWithInfo fixedAlphaShape fixedAlphaShapeWithInfo fixedAlphaShapeRegular fixedAlphaShapeRegularWithInfo)

# 3D Triangulation traits
set(CGALPY_TRI_3_TRAITS_KERNEL             0)
set(CGALPY_TRI_3_TRAITS_PERIODIC3_DELAUNAY 1)

set(CGALPY_TRI_3_TRAITS_SHORT_NAMES kernel p3d)
set(CGALPY_TRI_3_TRAITS_NAMES kernel periodic3Delaunay)
# Default
SET(CGALPY_TRI_3_TRAITS ${CGALPY_TRI_3_TRAITS_KERNEL})
SET(CGALPY_TRI_3_TRAITS_NAME "kernel" CACHE STRING "The 3D Triangulation traits")
set_property(CACHE CGALPY_TRI_3_TRAITS_NAME PROPERTY STRINGS kernel periodic3Delaunay)

# 3D Triangulation concurrency
set(CGALPY_TRI_3_CONCURRENCY_SEQUENTIAL 0)
set(CGALPY_TRI_3_CONCURRENCY_PARALLEL   1)

set(CGALPY_TRI_3_CONCURRENCY_SHORT_NAMES seq par)
set(CGALPY_TRI_3_CONCURRENCY_NAMES sequential parallel)
# Default
SET(CGALPY_TRI_3_CONCURRENCY ${CGALPY_TRI_3_CONCURRENCY_SEQUENTIAL})
SET(CGALPY_TRI_3_CONCURRENCY_NAME "sequential" CACHE STRING "The 3D Triangulation concurrency tag")
set_property(CACHE CGALPY_TRI_3_CONCURRENCY_NAME PROPERTY STRINGS sequential parallel)

# 3D Triangulation location policy
set(CGALPY_TRI_3_LOCATION_POLICY_FAST 0)
set(CGALPY_TRI_3_LOCATION_POLICY_COMPACT 1)

set(CGALPY_TRI_3_LOCATION_POLICY_SHORT_NAMES fast comp)
set(CGALPY_TRI_3_LOCATION_POLICY_NAMES fast compact)
# Default
SET(CGALPY_TRI_3_LOCATION_POLICY ${CGALPY_TRI_3_LOCATION_POLICY_COMPACT})
SET(CGALPY_TRI_3_LOCATION_POLICY_NAME "compact" CACHE STRING "The 3D Triangulation point location strategy")
set_property(CACHE CGALPY_TRI_3_LOCATION_POLICY_NAME PROPERTY STRINGS fast compact)

# 3D Triangulations
set(CGALPY_TRI_3_PLAIN              0)
set(CGALPY_TRI_3_REGULAR            1)
set(CGALPY_TRI_3_DELAUNAY           2)
set(CGALPY_TRI_3_PERIODIC3_DELAUNAY 3)
set(CGALPY_TRI_3_SHORT_NAMES plain reg del p3del)
set(CGALPY_TRI_3_NAMES plain regular delaunay periodic3Delaunay)
# Default
SET(CGALPY_TRI_3 ${CGALPY_TRI_3_REGULAR})
SET(CGALPY_TRI_3_NAME "regular" CACHE STRING "The 3D Triangulation")
set_property(CACHE CGALPY_TRI_3_NAME PROPERTY STRINGS plain regular delaunay periodic3Delaunay)
