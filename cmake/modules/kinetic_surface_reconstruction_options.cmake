# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

if (NOT KINETIC_SURFACE_RECONSTRUCTION_OPTIONS_FILE_INCLUDED)
set (KINETIC_SURFACE_RECONSTRUCTION_OPTIONS_FILE_INCLUDED)

function (select_kinetic_surface_reconstruction)
  if (CGALPY_KINETIC_SURFACE_RECONSTRUCTION_BINDINGS)
    if (NOT CGALPY_3D_POINT_SET_BINDINGS)
      message(WARNING "CGALPY_3D_POINT_SET_BINDINGS not set!\nTurning off CGALPY_KINETIC_SURFACE_RECONSTRUCTION_BINDINGS")
      set (CGALPY_KINETIC_SURFACE_RECONSTRUCTION_BINDINGS OFF CACHE BOOL "" FORCE)
    else ()
      add_definitions (-DCGALPY_KINETIC_SURFACE_RECONSTRUCTION_BINDINGS=)
    endif ()
  endif ()
endfunction ()

function (kinetic_surface_reconstruction_lib_name ret)
  set (${ret} "ksr" PARENT_SCOPE)
endfunction ()

endif ()
