if(NOT BGL_OPTIONS_FILE_INCLUDED)
set(BGL_OPTIONS_FILE_INCLUDED)

function(select_bgl)
  if(${CGALPY_BGL_BINDINGS})
    add_definitions(-DCGALPY_BGL_BINDINGS=)
  endif()
endfunction()

function(get_bgl_lib_name ret)
  set(${ret} "bgl" PARENT_SCOPE)
endfunction()

endif()
