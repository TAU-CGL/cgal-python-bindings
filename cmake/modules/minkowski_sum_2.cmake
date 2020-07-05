if(NOT MINKOWSKI_SUM_2_OPTIONS_FILE_INCLUDED)
set(MINKOWSKI_SUM_2_OPTIONS_FILE_INCLUDED)

function(select_minkowski_sum_2)
  if(${CGALPY_MINKOWSKI_SUM_2_BINDINGS})
    add_definitions(-DCGALPY_MINKOWSKI_SUM_2_BINDINGS)
  endif()
endfunction()

function(get_minkowski_sum_2_lib_name ret)
  set(${ret} "ms2" PARENT_SCOPE)
endfunction()

endif()
