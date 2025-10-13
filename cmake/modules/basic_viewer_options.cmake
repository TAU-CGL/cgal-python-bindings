if (NOT BASIC_VIEWER_OPTIONS_FILE_INCLUDED)
set (BASIC_VIEWER_OPTIONS_FILE_INCLUDED)

# Data Structure Options
set(CGALPY_BVR_AOS_2_DATA_STRUCTURE               0)
set(CGALPY_BVR_AOS_WITH_HISTORY_2_DATA_STRUCTURE  1)
set(CGALPY_BVR_POLYHEDRON_3_DATA_STRUCTURE        2)

# Data Structure Names
set(CGALPY_BVR_DATA_STRUCTURE_SHORT_NAMES aos2 aosWH2 pol3)
set(CGALPY_BVR_DATA_STRUCTURE_NAMES aos2 aosWithHistory2 polyhedron3)

# Data Structure Default
SET(CGALPY_BVR_DATA_STRUCTURE_NAME "aos2" CACHE STRING "The data structure to use")
set(CGALPY_BVR_DATA_STRUCTURE ${CGALPY_BVR_AOS_2_DATA_STRUCTURE} CACHE INTERNAL "")
set_property(CACHE CGALPY_BVR_DATA_STRUCTURE_NAME PROPERTY STRINGS aos2 aosWithHistory2 polyhedron3)

# Selection
function(select_bv_data_structure)
  if     ("${CGALPY_BVR_DATA_STRUCTURE_NAME}" STREQUAL "aos2")
    set(CGALPY_BVR_DATA_STRUCTURE ${CGALPY_BVR_AOS_2_DATA_STRUCTURE} CACHE INTERNAL "")
  elseif ("${CGALPY_BVR_DATA_STRUCTURE_NAME}" STREQUAL "aosWithHistory2")
    set(CGALPY_BVR_DATA_STRUCTURE ${CGALPY_BVR_AOS_WITH_HISTORY_2_DATA_STRUCTURE} CACHE INTERNAL "")
  elseif ("${CGALPY_BVR_DATA_STRUCTURE_NAME}" STREQUAL "polyhedron3")
    set(CGALPY_BVR_DATA_STRUCTURE ${CGALPY_BVR_POLYHEDRON_3_DATA_STRUCTURE} CACHE INTERNAL "")
  endif()
  add_definitions(-DCGALPY_BVR_DATA_STRUCTURE=${CGALPY_BVR_DATA_STRUCTURE})
endfunction()

function(select_basic_viewer)
  select_bv_data_structure()
  if (${CGALPY_BASIC_VIEWER_BINDINGS})
    add_definitions(-DCGALPY_BASIC_VIEWER_BINDINGS=)
  endif()
endfunction()

# library name
function(get_basic_viewer_lib_name ret)
  list(GET CGALPY_BVR_DATA_STRUCTURE_SHORT_NAMES ${CGALPY_BVR_DATA_STRUCTURE} part1)
  capitalize_first(part1)
  set(${ret} "bv${part1}" PARENT_SCOPE)
endfunction()

endif()
