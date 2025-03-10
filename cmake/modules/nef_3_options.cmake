if (NOT NEF_3_OPTIONS_FILE_INCLUDED)
set (NEF_3_OPTIONS_FILE_INCLUDED)

# Polygonal mesh Options
set(CGALPY_NEF3_POLYHEDRON_3_POLYGONAL_MESH      0)
set(CGALPY_NEF3_SURFACE_MESH_POLYGONAL_MESH      1)

# Polygonal mesh Names
set(CGALPY_NEF3_POLYGONAL_MESH_SHORT_NAMES pol3 sm)
set(CGALPY_NEF3_POLYGONAL_MESH_NAMES polyhedron surfaceMesh)

# Geometry Traits Default
SET(CGALPY_NEF3_POLYGONAL_MESH_NAME "surfaceMesh" CACHE STRING "The polygonal mesh to use")
set(CGALPY_NEF3_POLYGONAL_MESH ${CGALPY_NEF3_SURFACE_MESH_POLYGONAL_MESH} CACHE INTERNAL "")
set_property(CACHE CGALPY_NEF3_POLYGONAL_MESH_NAME PROPERTY STRINGS polyhedron surfaceMesh)

# Selection
function(select_nef3_polygonal_mesh)
  if     ("${CGALPY_NEF3_POLYGONAL_MESH_NAME}" STREQUAL "polyhedron")
    set(CGALPY_NEF3_POLYGONAL_MESH ${CGALPY_NEF3_POLYHEDRON_3_POLYGONAL_MESH} CACHE INTERNAL "")
  elseif ("${CGALPY_NEF3_POLYGONAL_MESH_NAME}" STREQUAL "surfaceMesh")
    set(CGALPY_NEF3_POLYGONAL_MESH ${CGALPY_NEF3_SURFACE_MESH_POLYGONAL_MESH} CACHE INTERNAL "")
  endif()
  add_definitions(-DCGALPY_NEF3_POLYGONAL_MESH=${CGALPY_NEF3_POLYGONAL_MESH})
endfunction()

function(select_nef_3)
  select_nef3_polygonal_mesh()
  if (CGALPY_NEF_3_BINDINGS)
    add_definitions(-DCGALPY_NEF_3_BINDINGS=)
  endif ()
endfunction()

function(get_nef_3_lib_name ret)
  list(GET CGALPY_NEF3_POLYGONAL_MESH_SHORT_NAMES ${CGALPY_NEF3_POLYGONAL_MESH} part1)
  capitalize_first(part1)
  set(${ret} "nef3${part1}" PARENT_SCOPE)
endfunction()

endif ()
