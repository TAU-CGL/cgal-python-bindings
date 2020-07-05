if(NOT CONVEX_HULL_2_OPTIONS_FILE_INCLUDED)
set(CONVEX_HULL_2_OPTIONS_FILE_INCLUDED)

function(select_convex_hull_2)
  if(${CGALPY_CONVEX_HULL_2_BINDINGS})
    add_definitions(-DCGALPY_CONVEX_HULL_2_BINDINGS)
  endif()
endfunction()

function(get_convex_hull_2_lib_name ret)
  set(${ret} "ch2" PARENT_SCOPE)
endfunction()

endif()
