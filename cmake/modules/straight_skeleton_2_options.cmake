if(NOT STRAIGHT_SKELETON_2_OPTIONS_FILE_INCLUDED)
set(STRAIGHT_SKELETON_2_OPTIONS_FILE_INCLUDED)

function(select_straight_skeleton_2)
  if(${CGALPY_STRAIGHT_SKELETON_2_BINDINGS})
    add_definitions(-DCGALPY_STRAIGHT_SKELETON_2_BINDINGS=)
  endif()
endfunction()

function(get_straight_skeleton_2_lib_name ret)
  set(${ret} "sn2" PARENT_SCOPE)
endfunction()

endif()
