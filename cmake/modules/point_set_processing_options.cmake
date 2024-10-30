if (NOT POINT_SET_PROCESSING_OPTIONS_FILE_INCLUDED)
set(POINT_SET_PROCESSING_OPTIONS_FILE_INCLUDED)

function(select_point_set_processing)
  if (CGALPY_POINT_SET_PROCESSING_PACKAGE_BINDINGS)
    set(CGALPY_3D_POINT_SET_BINDINGS ON CACHE BOOL "" FORCE)
    add_definitions(-DCGALPY_POINT_SET_PROCESSING_PACKAGE_BINDINGS=)
  endif()
endfunction()

# library name
function(get_point_set_processing_lib_name ret)
  set(${ret} "psp" PARENT_SCOPE)
endfunction()

endif()

