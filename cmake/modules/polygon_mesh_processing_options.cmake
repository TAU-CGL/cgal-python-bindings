if(NOT POLYGON_MESH_PROCESSING_OPTIONS_FILE_INCLUDED)
set(POLYGON_MESH_PROCESSING_OPTIONS_FILE_INCLUDED)

function(select_polygon_mesh_processing)
  if (CGALPY_POLYGON_MESH_PROCESSING_BINDINGS)
    add_definitions(-DCGALPY_POLYGON_MESH_PROCESSING_BINDINGS=)
  endif()
endfunction()

function(get_polygon_mesh_processing_lib_name ret)
  set(${ret} "pmp" PARENT_SCOPE)
endfunction()

endif()
