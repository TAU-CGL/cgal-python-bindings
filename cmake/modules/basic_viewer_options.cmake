if (NOT BASIC_VIEWER_OPTIONS_FILE_INCLUDED)
set (BASIC_VIEWER_OPTIONS_FILE_INCLUDED)

# Data Structure Options
set(CGALPY_BV_ARR_2_DATA_STRUCTURE             0)
set(CGALPY_BV_AOS_2_DATA_STRUCTURE             1)
set(CGALPY_BV_POLYHEDRON_3_DATA_STRUCTURE      2)

# Data Structure Names
set(CGALPY_BV_DATA_STRUCTURE_SHORT_NAMES arr2 aos2 pol3)
set(CGALPY_BV_DATA_STRUCTURE_NAMES arrangement2 arrangementOnSurface2 polyhedron3)

# Data Structure Default
SET(CGALPY_BV_DATA_STRUCTURE_NAME "arrangement2" CACHE STRING "The data structure to use")
set(CGALPY_BV_DATA_STRUCTURE ${CGALPY_BV_ARR_2_DATA_STRUCTURE} CACHE INTERNAL "")
set_property(CACHE CGALPY_BV_DATA_STRUCTURE_NAME PROPERTY STRINGS arrangement2 arrangementOnSurface2 polyhedron3)

# Selection
function(select_bv_data_structure)
  if     ("${CGALPY_BV_DATA_STRUCTURE_NAME}" STREQUAL "arrangement2")
    set(CGALPY_BV_DATA_STRUCTURE ${CGALPY_BV_ARR_2_DATA_STRUCTURE} CACHE INTERNAL "")
  elseif ("${CGALPY_BV_DATA_STRUCTURE_NAME}" STREQUAL "arrangementOnSurface2")
    set(CGALPY_BV_DATA_STRUCTURE ${CGALPY_BV_AOS_2_DATA_STRUCTURE} CACHE INTERNAL "")
  elseif ("${CGALPY_BV_DATA_STRUCTURE_NAME}" STREQUAL "polyhedron3")
    set(CGALPY_BV_DATA_STRUCTURE ${CGALPY_BV_AOS_2_DATA_STRUCTURE} CACHE INTERNAL "")
  endif()
  add_definitions(-DCGALPY_BV_DATA_STRUCTURE=${CGALPY_BV_DATA_STRUCTURE})
endfunction()

function(select_basic_viewer)
  select_bv_data_structure()
  if (${CGALPY_BASIC_VIEWER_BINDINGS})
    add_definitions(-DCGALPY_BASIC_VIEWER_BINDINGS=)
  endif()
endfunction()

# library name
function(get_basic_viewer_lib_name ret)
  list(GET CGALPY_BV_DATA_STRUCTURE_SHORT_NAMES ${CGALPY_BV_DATA_STRUCTURE} part1)
  capitalize_first(part1)
  set(${ret} "bv${part1}" PARENT_SCOPE)
endfunction()

endif()
