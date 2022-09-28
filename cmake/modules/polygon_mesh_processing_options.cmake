if(NOT POLYGON_MESH_PROCESSING_OPTIONS_FILE_INCLUDED)
set(POLYGON_MESH_PROCESSING_OPTIONS_FILE_INCLUDED)

# Polygonal mesh Options
set(CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH      0)
set(CGALPY_PMP_SURFACE_MESH_3_POLYGONAL_MESH    1)

# Polygonal mesh Names
set(CGALPY_PMP_POLYGONAL_MESH_SHORT_NAMES pol3 sm)
set(CGALPY_PMP_POLYGONAL_MESH_NAMES polyhedron surfaceMesh)

# Geometry Traits Default
SET(CGALPY_PMP_POLYGONAL_MESH_NAME "surfaceMesh" CACHE STRING "The polygonal mesh to use")
set(CGALPY_PMP_POLYGONAL_MESH ${CGALPY_PMP_SURFACE_MESH_3_POLYGONAL_MESH} CACHE INTERNAL "")
set_property(CACHE CGALPY_PMP_POLYGONAL_MESH_NAME PROPERTY STRINGS polyhedron surfaceMesh)

# Selection
function(select_polygonal_mesh)
  if     ("${CGALPY_PMP_POLYGONAL_MESH_NAME}" STREQUAL "polyhedron")
    set(CGALPY_PMP_POLYGONAL_MESH ${CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH} CACHE INTERNAL "")
  elseif ("${CGALPY_PMP_POLYGONAL_MESH_NAME}" STREQUAL "surfaceMesh")
    set(CGALPY_PMP_POLYGONAL_MESH ${CGALPY_PMP_SURFACE_MESH_3_POLYGONAL_MESH} CACHE INTERNAL "")
  endif()
  add_definitions(-DCGALPY_PMP_POLYGONAL_MESH=${CGALPY_PMP_POLYGONAL_MESH})
endfunction()

function(select_polygon_mesh_processing)
  select_polygonal_mesh()
  if (CGALPY_POLYGON_MESH_PROCESSING_BINDINGS)
    add_definitions(-DCGALPY_POLYGON_MESH_PROCESSING_BINDINGS=)
  endif()
endfunction()

# library name
function(get_polygon_mesh_processing_lib_name ret)
  list(GET CGALPY_PMP_POLYGONAL_MESH_SHORT_NAMES ${CGALPY_PMP_POLYGONAL_MESH} part1)
  capitalize_first(part1)
  set(${ret} "pmp${part1}" PARENT_SCOPE)
endfunction()

endif()
