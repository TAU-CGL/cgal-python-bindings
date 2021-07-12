if(NOT ALPHA_SHAPE_3_OPTIONS_FILE_INCLUDED)
set(ALPHA_SHAPE_3_OPTIONS_FILE_INCLUDED)

# Options
set(CGALPY_AS3_PLAIN 0)
set(CGALPY_AS3_FIXED 1)

# Names
set(CGALPY_AS3_SHORT_NAMES plain fixed)
set(CGALPY_AS3_NAMES plain fixed)

# Default
SET(CGALPY_AS3 ${CGALPY_AS3_PLAIN} CACHE INTERNAL "")
SET(CGALPY_AS3_NAME "plain" CACHE STRING "The 3D Alpha shape construct")
set_property(CACHE CGALPY_AS3_NAME PROPERTY STRINGS plain fixed)

# 3D comparison tag
set(CGALPY_AS3_EXACT_COMPARISON false CACHE BOOL "The exact comparison tag")

function(select_alpha_shape_3_comparison)
  if(${CGALPY_AS3_EXACT_COMPARISON})
    add_definitions(-DCGALPY_AS3_EXACT_COMPARISON=)
  endif()
endfunction()

# Selection
function(select_alpha_shape_3)
  if(${CGALPY_ALPHA_SHAPE_3_BINDINGS})
    select_alpha_shape_3_comparison()

    add_definitions(-DCGALPY_ALPHA_SHAPE_3_BINDINGS=)

    # Select Alpha shape
    if    ("${CGALPY_AS3_NAME}" STREQUAL "plain")
      set(CGALPY_AS3 ${CGALPY_AS3_PLAIN} CACHE INTERNAL "")
    elseif("${CGALPY_AS3_NAME}" STREQUAL "fixed")
      set(CGALPY_AS3 ${CGALPY_AS3_FIXED} CACHE INTERNAL "")
    endif()
    add_definitions(-DCGALPY_AS3=${CGALPY_AS3})
  endif()
endfunction()

function(get_alpha_shape_3_lib_name ret)
  list(GET CGALPY_AS3_SHORT_NAMES ${CGALPY_AS3} part1)
  capitalize_first(part1)
  if(${CGALPY_AS3_EXACT_COMPARISON})
    set(part2 "Exa")
  endif()
  set(${ret} "as3${part1}${part2}" PARENT_SCOPE)
endfunction()

endif()
