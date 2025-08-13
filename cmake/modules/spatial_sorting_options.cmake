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

# Spatial Sorting policy
set(CGALPY_ST_POLICY_HILBERT_SORT_MEDIAN 0)
set(CGALPY_ST_POLICY_HILBERT_SORT_MIDDLE 1)

set(CGALPY_ST_POLICY_SHORT_NAMES hsmed hsmid)
set(CGALPY_ST_POLICY_NAMES hilbert_sort_median hilbert_sort_middle)
# Default
SET(CGALPY_ST_POLICY ${CGALPY_ST_POLICY_HILBERT_SORT_MEDIAN} CACHE INTERNAL "")
SET(CGALPY_ST_POLICY_NAME "hilbert_sort_median" CACHE STRING "The Spatial Sorting policy")
set_property(CACHE CGALPY_ST_POLICY_NAME PROPERTY STRINGS hilbert_sort_median hilbert_sort_middle)

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

function(select_st_policy)
  if     ("${CGALPY_ST_POLICY_NAME}" STREQUAL "hilbert_sort_median")
    set(CGALPY_ST_POLICY ${CGALPY_ST_POLICY_HILBERT_SORT_MEDIAN} CACHE INTERNAL "")
  elseif ("${CGALPY_ST_POLICY_NAME}" STREQUAL "hilbert_sort_middle")
    set(CGALPY_ST_POLICY ${CGALPY_ST_POLICY_HILBERT_SORT_MIDDLE} CACHE INTERNAL "")
  endif()
  add_definitions(-DCGALPY_ST_POLICY=${CGALPY_ST_POLICY})
endfunction()

function (select_spatial_sorting)
  if (${CGALPY_SPATIAL_SORTING_BINDINGS})
    select_st_concurrency()
    select_st_policy()
    add_definitions(-DCGALPY_SPATIAL_SORTING_BINDINGS=)
  endif ()
endfunction ()

function (get_spatial_sorting_lib_name ret)
  # "ss" is taken by Spatial Searching; instead we use "st"
  list(GET CGALPY_ST_CONCURRENCY_SHORT_NAMES ${CGALPY_ST_CONCURRENCY} part1)
  capitalize_first(part1)
  list(GET CGALPY_ST_POLICY_SHORT_NAMES ${CGALPY_ST_POLICY} part2)
  capitalize_first(part2)
  set (${ret} "st${part1}${part2}" PARENT_SCOPE)
endfunction ()

endif ()
