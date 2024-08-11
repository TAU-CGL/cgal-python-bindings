if(NOT POLYGON_MESH_PROCESSING_OPTIONS_FILE_INCLUDED)
set(POLYGON_MESH_PROCESSING_OPTIONS_FILE_INCLUDED)

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
