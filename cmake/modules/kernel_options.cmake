if(NOT KERNEL_OPTIONS_FILE_INCLUDED)
set(KERNEL_OPTIONS_FILE_INCLUDED)

# Options
set(CGALPY_KERNEL_EPEC 0)
set(CGALPY_KERNEL_EPIC 1)

# Names
set(CGALPY_KERNEL_SHORT_NAMES "epec" "epic")
set(CGALPY_KERNEL_NAMES "epec" "epic")

# Default
SET(CGALPY_KERNEL_NAME "epec" CACHE STRING "The kernel to use")
set(CGALPY_KERNEL ${CGALPY_KERNEL_EPEC} CACHE INTERNAL "")
set_property(CACHE CGALPY_KERNEL_NAME PROPERTY STRINGS epec epic)

# Intersections
option(CGALPY_KERNEL_INTERSECTION_BINDINGS "Compile bindings for intersections" ON)

# Selection
function(select_kernel_intersection)
  if(${CGALPY_KERNEL_INTERSECTION_BINDINGS})
    add_definitions(-DCGALPY_KERNEL_INTERSECTION_BINDINGS)
  endif()
endfunction()

function(select_kernel)
  if ("${CGALPY_KERNEL_NAME}" STREQUAL "epec")
    set(CGALPY_KERNEL ${CGALPY_KERNEL_EPEC} CACHE INTERNAL "")
  elseif ("${CGALPY_KERNEL_NAME}" STREQUAL "epic")
    set(CGALPY_KERNEL ${CGALPY_KERNEL_EPIC} CACHE INTERNAL "")
  endif()
  add_definitions(-DCGALPY_KERNEL=${CGALPY_KERNEL})
  select_kernel_intersection()
endfunction()

function(get_kernel_lib_name ret)
  list(GET CGALPY_KERNEL_SHORT_NAMES ${CGALPY_KERNEL} part1)
  capitalize_first(part1)
  if(${CGALPY_KERNEL_INTERSECTION_BINDINGS})
    set(part2 "Int")
  endif()
  set(${ret} "kernel${part1}${part2}" PARENT_SCOPE)
endfunction()

endif()
