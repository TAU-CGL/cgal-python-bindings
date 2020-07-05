if(NOT TRIANGULATION_2_OPTIONS_FILE_INCLUDED)
set(TRIANGULATION_2_OPTIONS_FILE_INCLUDED)

function(select_triangulation_2)
  if (${CGALPY_TRIANGULATION_2_BINDINGS})
    add_definitions(-DCGALPY_TRIANGULATION_2_BINDINGS)
  endif()
endfunction()

function(get_triangulation_2_lib_name ret)
  set(${ret} "tri2" PARENT_SCOPE)
endfunction()

endif()
