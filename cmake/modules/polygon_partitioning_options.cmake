if(NOT POLYGON_PARTITIONING_OPTIONS_FILE_INCLUDED)
set(POLYGON_PARTITIONING_OPTIONS_FILE_INCLUDED)

function(select_polygon_partitioning)
  if(${CGALPY_POLYGON_PARTITIONING_BINDINGS})
    add_definitions(-DCGALPY_POLYGON_PARTITIONING_BINDINGS=)
  endif()
endfunction()

function(get_polygon_partitioning_lib_name ret)
  set(${ret} "pp" PARENT_SCOPE)
endfunction()

endif()
