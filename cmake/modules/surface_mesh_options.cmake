if(NOT SURFACE_MESH_OPTIONS_FILE_INCLUDED)
set(SURFACE_MESH_OPTIONS_FILE_INCLUDED)

function(select_surface_mesh)
  if (CGALPY_SURFACE_MESH_BINDINGS)
    if (NOT CGALPY_BGL_BINDINGS)
      message(WARNING "CGALPY_BGL_BINDINGS not set!\nTurning off CGALPY_SURFACE_MESH_BINDINGS")
      set(CGALPY_SURFACE_MESH_BINDINGS OFF CACHE BOOL "" FORCE)
    else()
      add_definitions(-DCGALPY_SURFACE_MESH_BINDINGS=)
    endif()
  endif()
endfunction()

function(get_surface_mesh_lib_name ret)
  set(${ret} "sm" PARENT_SCOPE)
endfunction()

endif()
