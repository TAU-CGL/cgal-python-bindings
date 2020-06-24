if(NOT CONVEX_HULL_2_OPTIONS_FILE_INCLUDED)
set(CONVEX_HULL_2_OPTIONS_FILE_INCLUDED)

function(select_convex_hull_2)
  if(${CGALPY_CONVEX_HULL_2_BINDINGS})
    add_definitions(-DCGALPY_CONVEX_HULL_2_BINDINGS)
  endif()
endfunction()

endif()
