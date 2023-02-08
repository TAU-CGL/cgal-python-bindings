if(NOT VISIBILITY_2_OPTIONS_FILE_INCLUDED)
set(VISIBILITY_2_OPTIONS_FILE_INCLUDED)

# Regularization tag
set(CGALPY_VIS2_REGULARIZATION_CATEGORY false CACHE BOOL "The regularization category tag")

function(select_alpha_shape_2_comparison)
  if(${CGALPY_VIS2_REGULARIZATION_CATEGORY})
    add_definitions(-DCGALPY_VIS2_REGULARIZATION_CATEGORY=)
  endif()
endfunction()

# Selection
function(select_visibility_2)
  if(${CGALPY_VISIBILITY_2_BINDINGS})
    add_definitions(-DCGALPY_VISIBILITY_2_BINDINGS=)
  endif()
endfunction()

function(get_visibility_2_lib_name ret)
  set(part1 "")
  if(${CGALPY_VIS2_REGULARIZATION_CATEGORY})
    set(part1 "Rg")
  endif()
  set(${ret} "vis2${part1}" PARENT_SCOPE)
endfunction()

endif()
