if(NOT SURFACE_MESH_OPTIONS_FILE_INCLUDED)
set(SURFACE_MESH_OPTIONS_FILE_INCLUDED)

function(select_surface_mesh)
  if (CGALPY_SURFACE_MESH_BINDINGS)
    add_definitions(-DCGALPY_SURFACE_MESH_BINDINGS=)
  endif()
endfunction()

function(get_surface_mesh_lib_name ret)
  set(${ret} "sm" PARENT_SCOPE)
endfunction()

endif()
