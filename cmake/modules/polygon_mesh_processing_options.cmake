if(NOT POLYGON_MESH_PROCESSING_OPTIONS_FILE_INCLUDED)
set(POLYGON_MESH_PROCESSING_OPTIONS_FILE_INCLUDED)

# Selection
function(select_pmp_distance)
  if(${CGALPY_PMP_DISTANCE_BINDINGS})
    add_definitions(-DCGALPY_PMP_DISTANCE_BINDINGS=)
  endif()
endfunction()

function(select_pmp_intersection)
  if(${CGALPY_PMP_INTERSECTION_BINDINGS})
    add_definitions(-DCGALPY_PMP_INTERSECTION_BINDINGS=)
  endif()
endfunction()

function(select_polygon_mesh_processing)
  select_polygonal_mesh()
  if (CGALPY_POLYGON_MESH_PROCESSING_BINDINGS)
    select_pmp_intersection()
    select_pmp_distance()
    add_definitions(-DCGALPY_POLYGON_MESH_PROCESSING_BINDINGS=)
  endif()
endfunction()

# library name
function(get_polygon_mesh_processing_lib_name ret)
  list(GET CGALPY_PMP_POLYGONAL_MESH_SHORT_NAMES ${CGALPY_PMP_POLYGONAL_MESH} part1)
  capitalize_first(part1)
  if(${CGALPY_PMP_INTERSECTION_BINDINGS})
    set(part2 "Int")
  endif()
  if(${CGALPY_PMP_DISTANCE_BINDINGS})
    set(part3 "Dis")
  endif()
  set(${ret} "pmp${part1}${part2}${part3}" PARENT_SCOPE)
endfunction()

endif()
