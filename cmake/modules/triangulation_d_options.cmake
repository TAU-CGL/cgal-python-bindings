if(NOT TRIANGULATION_D_OPTIONS_FILE_INCLUDED)
set(TRIANGULATION_D_OPTIONS_FILE_INCLUDED)

######## 3D Triangulations types
set(CGALPY_TRID_PLAIN             0)
set(CGALPY_TRID_REGULAR           1)
set(CGALPY_TRID_DELAUNAY          2)

set(CGALPY_TRID_SHORT_NAMES plain reg del pp preg pdel)
set(CGALPY_TRID_NAMES plain regular delaunay)

# Default
SET(CGALPY_TRID ${CGALPY_TRID_PLAIN} CACHE INTERNAL "")
SET(CGALPY_TRID_NAME "plain" CACHE STRING "The dD Triangulation")
set_property(CACHE CGALPY_TRID_NAME PROPERTY STRINGS plain regular delaunay)

# 3D Triangulation storage policy
set(CGALPY_TRID_STORAGE_POLICY_DEFAULT 0)
set(CGALPY_TRID_STORAGE_POLICY_MIRROR 1)

set(CGALPY_TRID_STORAGE_POLICY_SHORT_NAMES def mir)
set(CGALPY_TRID_STORAGE_POLICY_NAMES default mirror)

# Default
SET(CGALPY_TRID_STORAGE_POLICY ${CGALPY_TRID_STORAGE_POLICY_DEFAULT} CACHE INTERNAL "")
SET(CGALPY_TRID_STORAGE_POLICY_NAME "default" CACHE STRING "The dD Triangulation point storage strategy")
set_property(CACHE CGALPY_TRID_STORAGE_POLICY_NAME PROPERTY STRINGS default mirror)

function(select_trid_storage_policy)
  if     ("${CGALPY_TRID_STORAGE_POLICY_NAME}" STREQUAL "default")
    set(CGALPY_TRID_STORAGE_POLICY ${CGALPY_TRID_STORAGE_POLICY_DEFAULT} CACHE INTERNAL "")
  elseif ("${CGALPY_TRID_STORAGE_POLICY_NAME}" STREQUAL "mirror")
    set(CGALPY_TRID_STORAGE_POLICY ${CGALPY_TRID_STORAGE_POLICY_MIRROR} CACHE INTERNAL "")
  endif()
  add_definitions(-DCGALPY_TRID_STORAGE_POLICY=${CGALPY_TRID_STORAGE_POLICY})
endfunction()

# dimension tag
set(CGALPY_TRID_DIMENSION_TAG_DYNAMIC 0)
set(CGALPY_TRID_DIMENSION_TAG_STATIC 1)

set(CGALPY_TRID_DIMENSION_TAG_SHORT_NAMES dyn sta)
set(CGALPY_TRID_DIMENSION_TAG_NAMES dynamic static)

# Default
SET(CGALPY_TRID_DIMENSION_TAG ${CGALPY_TRID_DIMENSION_TAG_DYNAMIC} CACHE INTERNAL "")
SET(CGALPY_TRID_DIMENSION_TAG_NAME "dynamic" CACHE STRING "The dD Kernel dimension tag")
set_property(CACHE CGALPY_TRID_DIMENSION_TAG_NAME PROPERTY STRINGS dynamic static)

# Default
SET(CGALPY_TRID_DIMENSION 2 CACHE STRING "")

function(select_trid_dimension_tag)
  if ("${CGALPY_TRID_DIMENSION_TAG_NAME}" STREQUAL "static")
    set(CGALPY_TRID_DIMENSION_TAG ${CGALPY_TRID_DIMENSION_TAG_STATIC} CACHE INTERNAL "")
  elseif  ("${CGALPY_TRID_DIMENSION_TAG_NAME}" STREQUAL "dynamic")
    set(CGALPY_TRID_DIMENSION_TAG ${CGALPY_TRID_DIMENSION_TAG_DYNAMIC} CACHE INTERNAL "")
  endif()
  if (NOT CGALPY_TRID_DIMENSION MATCHES "^[0-9]+$")
    message(WARNING, "Invalid dimension (${CGALPY_TRID_DIMENSION})")
    return()
  endif()
  add_definitions(-DCGALPY_TRID_DIMENSION_TAG=${CGALPY_TRID_DIMENSION_TAG})
  add_definitions(-DCGALPY_TRID_DIMENSION=${CGALPY_TRID_DIMENSION})
endfunction()

function(select_trid)
  if    ("${CGALPY_TRID_NAME}" STREQUAL "plain")
    set(CGALPY_TRID ${CGALPY_TRID_PLAIN} CACHE INTERNAL "")
  elseif("${CGALPY_TRID_NAME}" STREQUAL "regular")
    set(CGALPY_TRID ${CGALPY_TRID_REGULAR} CACHE INTERNAL "")
  elseif("${CGALPY_TRID_NAME}" STREQUAL "delaunay")
    set(CGALPY_TRID ${CGALPY_TRID_DELAUNAY} CACHE INTERNAL "")
  endif()
  add_definitions(-DCGALPY_TRID=${CGALPY_TRID})
endfunction()

function(select_triangulation_d)
  if (${CGALPY_TRIANGULATION_D_BINDINGS})
    select_trid()
    select_trid_storage_policy()
    select_trid_dimension_tag()
    if (${CGALPY_TRID_VERTEX_WITH_DATA})
      add_definitions(-DCGALPY_TRID_VERTEX_WITH_DATA=)
    endif()
    if (${CGALPY_TRID_FULL_CELL_WITH_DATA})
      add_definitions(-DCGALPY_TRID_FULL_CELL_WITH_DATA=)
    endif()
    if (${CGALPY_TRID_HIERARCHY})
      add_definitions(-DCGALPY_TRID_HIERARCHY=)
    endif()
    add_definitions(-DCGALPY_TRIANGULATION_D_BINDINGS=)
  endif()
endfunction()

function(get_triangulation_d_lib_name ret)
  list(GET CGALPY_TRID_SHORT_NAMES ${CGALPY_TRID} part1)
  capitalize_first(part1)
  if (${CGALPY_TRID_VERTEX_WITH_DATA})
    set(part2 "Vd")
  endif ()
  set(part3 "")
  if(${CGALPY_TRID_FULL_CELL_WITH_DATA})
    set(part3 "Fcd")
  endif()
  list(GET CGALPY_TRID_STORAGE_POLICY_SHORT_NAMES ${CGALPY_TRID_STORAGE_POLICY} part4)
  capitalize_first(part4)
  list(GET CGALPY_TRID_DIMENSION_TAG_SHORT_NAMES ${CGALPY_TRID_DIMENSION_TAG} part5)
  capitalize_first(part5)
  if (${CGALPY_TRID_DIMENSION_TAG} STREQUAL ${CGALPY_TRID_DIMENSION_TAG_STATIC})
    set(part5 ${CGALPY_TRID_DIMENSION})
  endif()
  set(${ret} "trid${part1}${part2}${part3}${part4}${part5}" PARENT_SCOPE)
endfunction()

endif()
