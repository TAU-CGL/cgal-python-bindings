if(NOT ENVELOPE_3_OPTIONS_FILE_INCLUDED)
set(ENVELOPE_3_OPTIONS_FILE_INCLUDED)

# Geometry Traits Options
set(CGALPY_ENV3_PLANE_GEOMETRY_TRAITS		0)
set(CGALPY_ENV3_SPHERE_GEOMETRY_TRAITS          1)
set(CGALPY_ENV3_TRIANGLE_GEOMETRY_TRAITS        2)

# Geometry Traits Names
set(CGALPY_ENV3_GEOMETRY_GEOMETRY_TRAITS_SHORT_NAMES plane sphere tri)
set(CGALPY_ENV3_GEOMETRY_GEOMETRY_TRAITS_NAMES plane sphere triangle)

# Geometry Traits Default
SET(CGALPY_ENV3_GEOMETRY_GEOMETRY_TRAITS_NAME "plane" CACHE STRING "The geometry traits to use")
set(CGALPY_ENV3_GEOMETRY_GEOMETRY_TRAITS ${CGALPY_ENV3_PLANE_GEOMETRY_GEOMETRY_TRAITS} CACHE INTERNAL "")
set_property(CACHE CGALPY_ENV3_GEOMETRY_GEOMETRY_TRAITS_NAME PROPERTY STRINGS plane sphere triangle)

# Surface data
set(CGALPY_ENV3_SURFACE_DATA false CACHE BOOL "The surface data option")

# Selection
function(select_envelope_3_surface_data)
  if(${CGALPY_ENV3_SURFACE_DATA})
    add_definitions(-DCGALPY_ENV3_SURFACE_DATA=)
  endif()
endfunction()

function(select_envelope_3_geometry_traits)
  if     ("${CGALPY_ENV3_GEOMETRY_TRAITS_NAME}" STREQUAL "plane")
    set(CGALPY_ENV3_GEOMETRY_TRAITS ${CGALPY_ENV3_PLANE_GEOMETRY_TRAITS} CACHE INTERNAL "")
  elseif ("${CGALPY_ENV3_GEOMETRY_TRAITS_NAME}" STREQUAL "sphere")
    set(CGALPY_ENV3_GEOMETRY_TRAITS ${CGALPY_ENV3_SPHERE_GEOMETRY_TRAITS} CACHE INTERNAL "")
  elseif ("${CGALPY_ENV3_GEOMETRY_TRAITS_NAME}" STREQUAL "triangle")
    set(CGALPY_ENV3_GEOMETRY_TRAITS ${CGALPY_ENV3_TRIANGLE_GEOMETRY_TRAITS} CACHE INTERNAL "")
  endif()
  add_definitions(-DCGALPY_ENV3_GEOMETRY_TRAITS=${CGALPY_ENV3_GEOMETRY_TRAITS})
endfunction()

#
function(select_envelope_3)
  if(${CGALPY_ENVELOPE_3_BINDINGS})
    add_definitions(-DCGALPY_ENVELOPE_3_BINDINGS=)
    select_envelope_3_geometry_traits()
    select_envelope_3_surface_data()
  endif()
endfunction()

function(get_envelope_3_lib_name ret)
  list(GET CGALPY_ENV3_GEOMETRY_TRAITS_SHORT_NAMES ${CGALPY_ENV3_GEOMETRY_TRAITS} part1)
  capitalize_first(part1)
  set(part1 "")
  if(${CGALPY_ENV3_SURFACE_DATA})
    set(part1 "Sd")
  endif()
  set(${ret} "env3${part1}${part2}" PARENT_SCOPE)
endfunction()

endif()
