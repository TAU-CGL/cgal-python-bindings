if(NOT SURFACE_MESH_SIMPLIFICATION_OPTIONS_FILE_INCLUDED)
set(SURFACE_MESH_SIMPLIFICATION_OPTIONS_FILE_INCLUDED)

function(select_surface_mesh_simplification)
  if (CGALPY_SURFACE_MESH_SIMPLIFICATION_BINDINGS AND
      "${CGALPY_KERNEL_NAME}" STREQUAL "epec")
    message(WARNING
      "CGALPY_SURFACE_MESH_SIMPLIFICATION_BINDINGS is incompatible with "
      "CGALPY_KERNEL_NAME=epec and will be disabled.")
    set(CGALPY_SURFACE_MESH_SIMPLIFICATION_BINDINGS OFF CACHE BOOL
      "Compile bindings for surface mesh simplification" FORCE)
  endif()

  select_polygonal_mesh()
  if (CGALPY_SURFACE_MESH_SIMPLIFICATION_BINDINGS)
    add_definitions(-DCGALPY_SURFACE_MESH_SIMPLIFICATION_BINDINGS=)
  endif()
endfunction()

# library name
function(get_surface_mesh_simplification_lib_name ret)
  list(GET CGALPY_PMP_POLYGONAL_MESH_SHORT_NAMES ${CGALPY_PMP_POLYGONAL_MESH} part1)
  capitalize_first(part1)
  set(${ret} "smsi${part1}" PARENT_SCOPE)
endfunction()

endif()
