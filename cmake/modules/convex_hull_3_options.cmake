if(NOT CONVEX_HULL_3_OPTIONS_FILE_INCLUDED)
set(CONVEX_HULL_3_OPTIONS_FILE_INCLUDED)

# Polygonal mesh Options
set(CGALPY_CH3_POLYHEDRON_3_POLYGONAL_MESH      0)
set(CGALPY_CH3_SURFACE_MESH_3_POLYGONAL_MESH    1)

# Polygonal mesh Names
set(CGALPY_CH3_POLYGONAL_MESH_SHORT_NAMES pol3 sm)
set(CGALPY_CH3_POLYGONAL_MESH_NAMES polyhedron surfaceMesh)

# Geometry Traits Default
SET(CGALPY_CH3_POLYGONAL_MESH_NAME "surfaceMesh" CACHE STRING "The polygonal mesh to use")
set(CGALPY_CH3_POLYGONAL_MESH ${CGALPY_CH3_SURFACE_MESH_3_POLYGONAL_MESH} CACHE INTERNAL "")
set_property(CACHE CGALPY_CH3_POLYGONAL_MESH_NAME PROPERTY STRINGS polyhedron surfaceMesh)

# Selection
function(select_ch3_polygonal_mesh)
  if     ("${CGALPY_CH3_POLYGONAL_MESH_NAME}" STREQUAL "polyhedron")
    set(CGALPY_CH3_POLYGONAL_MESH ${CGALPY_CH3_POLYHEDRON_3_POLYGONAL_MESH} CACHE INTERNAL "")
  elseif ("${CGALPY_CH3_POLYGONAL_MESH_NAME}" STREQUAL "surfaceMesh")
    set(CGALPY_CH3_POLYGONAL_MESH ${CGALPY_CH3_SURFACE_MESH_3_POLYGONAL_MESH} CACHE INTERNAL "")
  endif()
  add_definitions(-DCGALPY_CH3_POLYGONAL_MESH=${CGALPY_CH3_POLYGONAL_MESH})
endfunction()

function(select_convex_hull_3)
  select_ch3_polygonal_mesh()
  if(${CGALPY_CONVEX_HULL_3_BINDINGS})
    add_definitions(-DCGALPY_CONVEX_HULL_3_BINDINGS=)
  endif()
endfunction()

# library name
function(get_convex_hull_3_lib_name ret)
  list(GET CGALPY_CH3_POLYGONAL_MESH_SHORT_NAMES ${CGALPY_CH3_POLYGONAL_MESH} part1)
  capitalize_first(part1)
  set(${ret} "ch3${part1}" PARENT_SCOPE)
endfunction()

endif()
