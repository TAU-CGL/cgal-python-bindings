if(NOT SURFACE_MESH_TOPOLOGY_OPTIONS_FILE_INCLUDED)
set(SURFACE_MESH_TOPOLOGY_OPTIONS_FILE_INCLUDED)

function(select_surface_mesh_topology)
  select_polygonal_mesh()
  if (CGALPY_SURFACE_MESH_TOPOLOGY_BINDINGS)
    add_definitions(-DCGALPY_SURFACE_MESH_TOPOLOGY_BINDINGS=)
  endif()
endfunction()

# library name
function(get_surface_mesh_topology_lib_name ret)
  list(GET CGALPY_PMP_POLYGONAL_MESH_SHORT_NAMES ${CGALPY_PMP_POLYGONAL_MESH} part1)
  capitalize_first(part1)
  set(${ret} "smt${part1}" PARENT_SCOPE)
endfunction()

endif()
