if(NOT ALPHA_SHAPE_2_OPTIONS_FILE_INCLUDED)
set(ALPHA_SHAPE_2_OPTIONS_FILE_INCLUDED)

# 2D comparison tag
set(CGALPY_AS2_EXACT_COMPARISON false CACHE BOOL "The exact comparison tag")

function(select_alpha_shape_2_comparison)
  if(${CGALPY_AS2_EXACT_COMPARISON})
    add_definitions(-DCGALPY_AS2_EXACT_COMPARISON=)
  endif()
endfunction()

# Selection
function(select_alpha_shape_2)
  if(${CGALPY_ALPHA_SHAPE_2_BINDINGS})
    select_alpha_shape_2_comparison()
    add_definitions(-DCGALPY_ALPHA_SHAPE_2_BINDINGS=)
  endif()
endfunction()

function(get_alpha_shape_2_lib_name ret)
  set(part1 "")
  if(${CGALPY_AS2_EXACT_COMPARISON})
    set(part1 "Exa")
  endif()
  set(${ret} "as2${part1}" PARENT_SCOPE)
endfunction()

endif()
