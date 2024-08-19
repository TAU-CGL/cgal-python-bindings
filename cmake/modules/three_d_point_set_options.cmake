if (NOT THREE_D_POINT_SET_OPTIONS_FILE_INCLUDED)
set(THREE_D_POINT_SET_OPTIONS_FILE_INCLUDED)

function(select_three_d_point_set)
  if (CGALPY_3D_POINT_SET_BINDINGS)
    add_definitions(-DCGALPY_3D_POINT_SET_BINDINGS=)
  endif()
endfunction()


endif()
