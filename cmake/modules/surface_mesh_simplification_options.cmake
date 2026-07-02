if(NOT SURFACE_MESH_SIMPLIFICATION_OPTIONS_FILE_INCLUDED)
set(SURFACE_MESH_SIMPLIFICATION_OPTIONS_FILE_INCLUDED)

function(select_surface_mesh_simplification)
  select_triangulated_surface_mesh_simplification()
endfunction()

function(get_surface_mesh_simplification_lib_name ret)
  get_triangulated_surface_mesh_simplification_lib_name(local_ret)
  set(${ret} "${local_ret}" PARENT_SCOPE)
endfunction()

endif()
