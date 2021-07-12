if(NOT BOUNDING_VOLUMES_OPTIONS_FILE_INCLUDED)
set(BOUNDING_VOLUMES_OPTIONS_FILE_INCLUDED)

function(select_bounding_volumes)
  if(${CGALPY_BOUNDING_VOLUMES_BINDINGS})
    add_definitions(-DCGALPY_BOUNDING_VOLUMES_BINDINGS=)
  endif()
endfunction()

function(get_bounding_volumes_lib_name ret)
  set(${ret} "bv" PARENT_SCOPE)
endfunction()

endif()
