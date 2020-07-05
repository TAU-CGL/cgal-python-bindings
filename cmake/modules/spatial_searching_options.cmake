if(NOT SPATIAL_SEARCHING_OPTIONS_FILE_INCLUDED)
set(SPATIAL_SEARCHING_OPTIONS_FILE_INCLUDED)

# Dimension
SET(CGALPY_SPATIAL_SEARCHING_DIMENSION 2 CACHE STRING "The dimension of spatial search objects")

function(select_spatial_searching)
  if(${CGALPY_SPATIAL_SEARCHING_BINDINGS})
    # Select dimension
    add_definitions(-DCGALPY_SPATIAL_SEARCHING_DIMENSION=${CGALPY_SPATIAL_SEARCHING_DIMENSION})

    add_definitions(-DCGALPY_SPATIAL_SEARCHING_BINDINGS)
  endif()
endfunction()

function(get_spatial_searching_lib_name ret)
  set(${ret} "ss" PARENT_SCOPE)
endfunction()

endif()
