if(NOT VISIBILITY_2_OPTIONS_FILE_INCLUDED)
set(VISIBILITY_2_OPTIONS_FILE_INCLUDED)

function(select_visibility_2)
  if(${CGALPY_VISIBILITY_2_BINDINGS})
    add_definitions(-DCGALPY_VISIBILITY_2_BINDINGS=)
  endif()
endfunction()

function(get_visibility_2_lib_name ret)
  set(${ret} "vis2" PARENT_SCOPE)
endfunction()

endif()
