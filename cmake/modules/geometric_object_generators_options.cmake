if(NOT GEOMETRIC_OBJECT_GENERATORS_OPTIONS_FILE_INCLUDED)
set(GEOMETRIC_OBJECT_GENERATORS_OPTIONS_FILE_INCLUDED)

function(select_geometric_object_generators)
  if(${CGALPY_GEOMETRIC_OBJECT_GENERATORS_BINDINGS})
    add_definitions(-DCGALPY_GEOMETRIC_OBJECT_GENERATORS_BINDINGS=)
  endif()
endfunction()

function(get_geometric_object_generators_lib_name ret)
  set(${ret} "gog" PARENT_SCOPE)
endfunction()

endif()
