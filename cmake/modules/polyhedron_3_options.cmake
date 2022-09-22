if(NOT POLYHEDRON_3_OPTIONS_FILE_INCLUDED)
  set(POLYHEDRON_3_OPTIONS_FILE_INCLUDED)

  function(select_polyhedron_3)
    if(${CGALPY_POLYHEDRON_3_BINDINGS})
      add_definitions(-DCGALPY_POLYHEDRON_3_BINDINGS=)
    endif()
  endfunction()

  function(get_polyhedron_3_lib_name ret)
    set(${ret} "pol3" PARENT_SCOPE)
  endfunction()

endif()
