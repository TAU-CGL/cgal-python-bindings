if(NOT SURFACE_MESH_SKELETONIZATION_OPTIONS_FILE_INCLUDED)
set(SURFACE_MESH_SKELETONIZATION_OPTIONS_FILE_INCLUDED)

function(select_surface_mesh_skeletonization)
  select_triangulated_surface_mesh_skeletonization()
endfunction()

function(get_surface_mesh_skeletonization_lib_name ret)
  get_triangulated_surface_mesh_skeletonization_lib_name(local_ret)
  set(${ret} "${local_ret}" PARENT_SCOPE)
endfunction()

endif()
