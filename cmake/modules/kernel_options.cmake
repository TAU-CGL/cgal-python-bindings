if(NOT KERNEL_OPTIONS_FILE_INCLUDED)
set(KERNEL_OPTIONS_FILE_INCLUDED)

# Options
set(CGALPY_KERNEL_EPIC 0)
set(CGALPY_KERNEL_EPEC 1)
set(CGALPY_KERNEL_EPEC_WITH_SQRT 2)
set(CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ 3)

# Names
set(CGALPY_KERNEL_SHORT_NAMES "epic" "epec" "epecws" "fsclg")
set(CGALPY_KERNEL_NAMES "epic" "epec" "epecWithSqrt" "filteredSimpleCartesianLazyGmpq")

# Default
SET(CGALPY_KERNEL_NAME "epec" CACHE STRING "The kernel to use")
set(CGALPY_KERNEL ${CGALPY_KERNEL_EPEC} CACHE INTERNAL "")
set_property(CACHE CGALPY_KERNEL_NAME PROPERTY STRINGS epic epec epecWithSqrt filteredSimpleCartesianLazyGmpq)

# Selection
function(select_kernel_intersection)
  if(${CGALPY_KERNEL_INTERSECTION_BINDINGS})
    add_definitions(-DCGALPY_KERNEL_INTERSECTION_BINDINGS)
  endif()
endfunction()

function(select_kernel)
  if ("${CGALPY_KERNEL_NAME}" STREQUAL "epic")
    set(CGALPY_KERNEL ${CGALPY_KERNEL_EPIC} CACHE INTERNAL "")
  elseif ("${CGALPY_KERNEL_NAME}" STREQUAL "epec")
    set(CGALPY_KERNEL ${CGALPY_KERNEL_EPEC} CACHE INTERNAL "")
  elseif ("${CGALPY_KERNEL_NAME}" STREQUAL "epecWithSqrt")
    set(CGALPY_KERNEL ${CGALPY_KERNEL_EPEC_WITH_SQRT} CACHE INTERNAL "")
  elseif ("${CGALPY_KERNEL_NAME}" STREQUAL "filteredSimpleCartesianLazyGmpq")
    set(CGALPY_KERNEL ${CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ} CACHE INTERNAL "")
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
  set(${ret} "ker${part1}${part2}" PARENT_SCOPE)
endfunction()

endif()
