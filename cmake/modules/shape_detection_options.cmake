if(NOT SHAPE_DETECTION_OPTIONS_FILE_INCLUDED)
set(SHAPE_DETECTION_OPTIONS_FILE_INCLUDED)

function(select_shape_detection)
  if (CGALPY_SHAPE_DETECTION_BINDINGS)
  # 3D Point Set for Region Growing
  set(CGALPY_3D_POINT_SET_BINDINGS ON CACHE BOOL "" FORCE)
    add_definitions(-DCGALPY_SHAPE_DETECTION_BINDINGS=)
  endif()
endfunction()

# library name
function(get_shape_detection_lib_name ret)
  set(${ret} "Sd" PARENT_SCOPE)
endfunction()

endif()
