if(NOT SURFACE_SWEEP_2_OPTIONS_FILE_INCLUDED)
set(SURFACE_SWEEP_2_OPTIONS_FILE_INCLUDED)

function(select_surface_sweep_2)
  if(${CGALPY_SURFACE_SWEEP_2_BINDINGS})
    add_definitions(-DCGALPY_SURFACE_SWEEP_2_BINDINGS=)
  endif()
endfunction()

function(get_surface_sweep_2_lib_name ret)
  set(${ret} "ss2" PARENT_SCOPE)
endfunction()

endif()
