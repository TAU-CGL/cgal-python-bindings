if (NOT KINETIC_SURFACE_RECONSTRUCTION_OPTIONS_FILE_INCLUDED)
set (KINETIC_SURFACE_RECONSTRUCTION_OPTIONS_FILE_INCLUDED)

function (select_kinetic_surface_reconstruction)
  if (CGALPY_KINETIC_SURFACE_RECONSTRUCTION_BINDINGS)
    set (CGALPY_3D_POINT_SET_BINDINGS ON CACHE BOOL "" FORCE)
    add_definitions (-DCGALPY_KINETIC_SURFACE_RECONSTRUCTION_BINDINGS=)
  endif ()
endfunction ()

function (kinetic_surface_reconstruction_lib_name ret)
  set (${ret} "ksr" PARENT_SCOPE)
endfunction ()

endif ()
