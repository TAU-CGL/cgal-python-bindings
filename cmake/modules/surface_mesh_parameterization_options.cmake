# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

if(NOT SURFACE_MESH_PARAMETERIZATION_OPTIONS_FILE_INCLUDED)
set(SURFACE_MESH_PARAMETERIZATION_OPTIONS_FILE_INCLUDED)

function(select_surface_mesh_parameterization)
  if (CGALPY_SURFACE_MESH_PARAMETERIZATION_BINDINGS AND
      "${CGALPY_KERNEL_NAME}" STREQUAL "epec")
    message(WARNING
      "CGALPY_SURFACE_MESH_PARAMETERIZATION_BINDINGS is incompatible with "
      "CGALPY_KERNEL_NAME=epec and will be disabled.")
    set(CGALPY_SURFACE_MESH_PARAMETERIZATION_BINDINGS OFF CACHE BOOL
      "Compile bindings for surface mesh parameterization" FORCE)
  endif()

  select_polygonal_mesh()
  if (CGALPY_SURFACE_MESH_PARAMETERIZATION_BINDINGS)
    add_definitions(-DCGALPY_SURFACE_MESH_PARAMETERIZATION_BINDINGS=)
  endif()
endfunction()

# library name
function(get_surface_mesh_parameterization_lib_name ret)
  list(GET CGALPY_PMP_POLYGONAL_MESH_SHORT_NAMES ${CGALPY_PMP_POLYGONAL_MESH} part1)
  capitalize_first(part1)
  set(${ret} "smp${part1}" PARENT_SCOPE)
endfunction()

endif()
