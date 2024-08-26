if(NOT TRIANGULATED_SURFACE_MESH_SKELETONIZATION_FILE_INCLUDED)
set(TRIANGULATED_SURFACE_MESH_SKELETONIZATION_FILE_INCLUDED)

function(select_triangulated_surface_mesh_skeletonization)
  select_polygonal_mesh()
  if (CGALPY_TRIANGULATED_SURFACE_MESH_SKELETONIZATION_BINDINGS)
    add_definitions(-DCGALPY_TRIANGULATED_SURFACE_MESH_SKELETONIZATION_BINDINGS=)
  endif()
endfunction()

# library name
function(get_triangulated_surface_mesh_skeletonization_lib_name ret)
  list(GET CGALPY_PMP_POLYGONAL_MESH_SHORT_NAMES ${CGALPY_PMP_POLYGONAL_MESH} part1)
  capitalize_first(part1)
  set(${ret} "smsk${part1}" PARENT_SCOPE)
endfunction()

endif()

