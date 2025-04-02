if(NOT POLYGON_MESH_PROCESSING_OPTIONS_FILE_INCLUDED)
set(POLYGON_MESH_PROCESSING_OPTIONS_FILE_INCLUDED)

# Selection
# 1. combinatorial repair
function(select_pmp_combinatorial_repair)
  if(${CGALPY_PMP_COMBINATORIAL_REPAIR_BINDINGS})
    add_definitions(-DCGALPY_PMP_COMBINATORIAL_REPAIR_BINDINGS=)
  endif()
endfunction()

# 2. connected components
function(select_pmp_connected_components)
  if(${CGALPY_PMP_CONNECTED_COMPONENTS_BINDINGS})
    add_definitions(-DCGALPY_PMP_CONNECTED_COMPONENTS_BINDINGS=)
  endif()
endfunction()

# 3. corefinement
function(select_pmp_corefinement)
  if(${CGALPY_PMP_COREFINEMENT_BINDINGS})
    add_definitions(-DCGALPY_PMP_COREFINEMENT_BINDINGS=)
  endif()
endfunction()

# 4. corrected curvature computation
function(select_pmp_corrected_curvature_computation)
  if(${CGALPY_PMP_CORRECTED_CURVATURE_COMPUTATION_BINDINGS})
    add_definitions(-DCGALPY_PMP_CORRECTED_CURVATURE_COMPUTATION_BINDINGS=)
  endif()
endfunction()

# 5. distance
function(select_pmp_distance)
  if(${CGALPY_PMP_DISTANCE_BINDINGS})
    add_definitions(-DCGALPY_PMP_DISTANCE_BINDINGS=)
  endif()
endfunction()

# 6. feature detection
function(select_pmp_feature_detection)
  if(${CGALPY_PMP_FEATURE_DETECTION_BINDINGS})
    add_definitions(-DCGALPY_PMP_FEATURE_DETECTION_BINDINGS=)
  endif()
endfunction()

# 7. geometric measure
function(select_pmp_geometric_measure)
  if(${CGALPY_PMP_GEOMETRIC_MEASURE_BINDINGS})
    add_definitions(-DCGALPY_PMP_GEOMETRIC_MEASURE_BINDINGS=)
  endif()
endfunction()

# 8. geometric repair
function(select_pmp_geometric_repair)
  if(${CGALPY_PMP_GEOMETRIC_REPAIR_BINDINGS})
    add_definitions(-DCGALPY_PMP_GEOMETRIC_REPAIR_BINDINGS=)
  endif()
endfunction()

# 9. hole filling
function(select_pmp_hole_filling)
  if(${CGALPY_PMP_HOLE_FILLING_BINDINGS})
    add_definitions(-DCGALPY_PMP_HOLE_FILLING_BINDINGS=)
  endif()
endfunction()

# 10. intersection
function(select_pmp_intersection)
  if(${CGALPY_PMP_INTERSECTION_BINDINGS})
    add_definitions(-DCGALPY_PMP_INTERSECTION_BINDINGS=)
  endif()
endfunction()

# 11. location
function(select_pmp_location)
  if(${CGALPY_PMP_LOCATION_BINDINGS})
    add_definitions(-DCGALPY_PMP_LOCATION_BINDINGS=)
  endif()
endfunction()

# 12. normal computation
function(select_pmp_normal_computation)
  if(${CGALPY_PMP_NORMAL_COMPUTATION_BINDINGS})
    add_definitions(-DCGALPY_PMP_NORMAL_COMPUTATION_BINDINGS=)
  endif()
endfunction()

# 13. orientation
function(select_pmp_orientation)
  if(${CGALPY_PMP_ORIENTATION_BINDINGS})
    add_definitions(-DCGALPY_PMP_ORIENTATION_BINDINGS=)
  endif()
endfunction()

# 14. meshing
function(select_pmp_meshing)
  if(${CGALPY_PMP_MESHING_BINDINGS})
    add_definitions(-DCGALPY_PMP_MESHING_BINDINGS=)
  endif()
endfunction()


function(select_polygon_mesh_processing)
  select_polygonal_mesh()
  if (CGALPY_POLYGON_MESH_PROCESSING_BINDINGS)
    select_pmp_combinatorial_repair()            #  1
    select_pmp_connected_components()            #  2
    select_pmp_corefinement()                    #  3
    select_pmp_corrected_curvature_computation() #  4
    select_pmp_distance()                        #  5
    select_pmp_feature_detection()               #  6
    select_pmp_geometric_measure()               #  7
    select_pmp_geometric_repair()                #  8
    select_pmp_hole_filling()                    #  9
    select_pmp_intersection()                    # 10
    select_pmp_location()                        # 11
    select_pmp_normal_computation()              # 12
    select_pmp_orientation()                     # 13
    select_pmp_meshing()                         # 14
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
  if(${CGALPY_PMP_CONNECTED_COMPONENTS_BINDINGS})
    set(part4 "Cc")
  endif()
  set(${ret} "pmp${part1}${part2}${part3}${part4}" PARENT_SCOPE)
endfunction()

endif()
