if (NOT SPATIAL_SORTING_OPTIONS_FILE_INCLUDED)
set (SPATIAL_SORTING_OPTIONS_FILE_INCLUDED)

# Spatial Sorting concurrency
set(CGALPY_ST_CONCURRENCY_SEQUENTIAL 0)
set(CGALPY_ST_CONCURRENCY_PARALLEL   1)

set(CGALPY_ST_CONCURRENCY_SHORT_NAMES seq par)
set(CGALPY_ST_CONCURRENCY_NAMES sequential parallel)
# Default
SET(CGALPY_ST_CONCURRENCY ${CGALPY_ST_CONCURRENCY_SEQUENTIAL} CACHE INTERNAL "")
SET(CGALPY_ST_CONCURRENCY_NAME "sequential" CACHE STRING "The Spatial Sorting concurrency tag")
set_property(CACHE CGALPY_ST_CONCURRENCY_NAME PROPERTY STRINGS sequential parallel)

# PolicyTag: Hilbert_sort_median_policy, Hilbert_sort_middle_policy

# Selection
function(select_st_concurrency)
  # Select spatial sorting concurrency
  if     ("${CGALPY_ST_CONCURRENCY_NAME}" STREQUAL "sequential")
    set(CGALPY_ST_CONCURRENCY ${CGALPY_ST_CONCURRENCY_SEQUENTIAL} CACHE INTERNAL "")
  elseif ("${CGALPY_ST_CONCURRENCY_NAME}" STREQUAL "parallel")
    set(CGALPY_ST_CONCURRENCY ${CGALPY_ST_CONCURRENCY_PARALLEL} CACHE INTERNAL "")
  endif()
  add_definitions(-DCGALPY_ST_CONCURRENCY=${CGALPY_ST_CONCURRENCY})
endfunction()

function (select_spatial_sorting)
  if (${CGALPY_SPATIAL_SORTING_BINDINGS})
    select_st_concurrency()
    add_definitions(-DCGALPY_SPATIAL_SORTING_BINDINGS=)
  endif ()
endfunction ()

function (get_spatial_sorting_lib_name ret)
  # "ss" is taken by Spatial Searching; instead we use "st"
  list(GET CGALPY_ST_CONCURRENCY_SHORT_NAMES ${CGALPY_ST_CONCURRENCY} part1)
  capitalize_first(part1)
  set (${ret} "st${part1}" PARENT_SCOPE)
endfunction ()

endif ()
