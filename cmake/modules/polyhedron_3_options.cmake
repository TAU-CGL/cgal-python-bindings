if(NOT POLYHEDRON_3_OPTIONS_FILE_INCLUDED)
set(POLYHEDRON_3_OPTIONS_FILE_INCLUDED)

# HDS
option(CGALPY_POL3_VERTEX_EXTENDED "Extend the Vertex record")
option(CGALPY_POL3_HALFEDGE_EXTENDED "Extend the Halfedge record")
option(CGALPY_POL3_FACET_EXTENDED "Extend the Facet record")

# Selection
function(select_pol3_hds)
  if (${CGALPY_POL3_VERTEX_EXTENDED})
    add_definitions(-DCGALPY_POL3_VERTEX_EXTENDED=)
  endif()
  if (${CGALPY_POL3_HALFEDGE_EXTENDED})
    add_definitions(-DCGALPY_POL3_HALFEDGE_EXTENDED=)
  endif()
  if (${CGALPY_POL3_FACET_EXTENDED})
    add_definitions(-DCGALPY_POL3_FACET_EXTENDED=)
  endif()
endfunction()

function(select_polyhedron_3)
  if(${CGALPY_POLYHEDRON_3_BINDINGS})
    select_pol3_hds()
    add_definitions(-DCGALPY_POLYHEDRON_3_BINDINGS=)
  endif()
endfunction()

# Library name
function(get_polyhedron_3_lib_name ret)
  if(${CGALPY_POL3_VERTEX_EXTENDED})
    set(part1 "Ve")
  endif()
  if(${CGALPY_POL3_HALFEDGE_EXTENDED})
    set(part2 "He")
  endif()
  if(${CGALPY_POL3_FACE_EXTENDED})
    set(part3 "Fe")
  endif()
  set(${ret} "pol3${part1}${part2}${part3}" PARENT_SCOPE)
endfunction()

endif()
