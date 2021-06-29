if(NOT KERNEL_D_OPTIONS_FILE_INCLUDED)
set(KERNEL_D_OPTIONS_FILE_INCLUDED)

# Options
set(CGALPY_KERNEL_D_EPIC_D 0)
set(CGALPY_KERNEL_D_EPEC_D 1)
set(CGALPY_KERNEL_D_SPACE_HOLDER 2)
set(CGALPY_KERNEL_D_CARTESIAN_D_DOUBLE 3)
set(CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ 4)

# Names
set(CGALPY_KERNEL_D_SHORT_NAMES "epicd" "epecd" "invalid" "cd" "clg")
set(CGALPY_KERNEL_D_NAMES "epicd" "epecd" "invalid" "cartesiandDouble" "cartesiandLazyGmpq")

# Default
SET(CGALPY_KERNEL_D_NAME "epecd" CACHE STRING "The kernel to use")
set(CGALPY_KERNEL_D ${CGALPY_KERNEL_D_EPEC_D} CACHE INTERNAL "")
set_property(CACHE CGALPY_KERNEL_D_NAME PROPERTY STRINGS epicd epecd cartesiandDouble cartesiandLazyGmpq)

# Selection
function(select_kernel_d_name)
  if ("${CGALPY_KERNEL_D_NAME}" STREQUAL "epicd")
    set(CGALPY_KERNEL_D ${CGALPY_KERNEL_D_EPIC_D} CACHE INTERNAL "")
  elseif ("${CGALPY_KERNEL_D_NAME}" STREQUAL "epecd")
    set(CGALPY_KERNEL_D ${CGALPY_KERNEL_D_EPEC_D} CACHE INTERNAL "")
  elseif ("${CGALPY_KERNEL_D_NAME}" STREQUAL "cartesiandDouble")
    set(CGALPY_KERNEL_D ${CGALPY_KERNEL_D_CARTESIAN_D_DOUBLE} CACHE INTERNAL "")
  elseif ("${CGALPY_KERNEL_D_NAME}" STREQUAL "cartesiandLazyGmpq")
    set(CGALPY_KERNEL_D ${CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ} CACHE INTERNAL "")
  endif()
  add_definitions(-DCGALPY_KERNEL_D=${CGALPY_KERNEL_D})
endfunction()

# Kern D dimension tag
set(CGALPY_KERNEL_D_DIMENSION_TAG_DYNAMIC 0)
set(CGALPY_KERNEL_D_DIMENSION_TAG_STATIC 1)

set(CGALPY_KERNEL_D_DIMENSION_TAG_SHORT_NAMES dyn sta)
set(CGALPY_KERNEL_D_DIMENSION_TAG_NAMES dynamic static)

# Default
SET(CGALPY_KERNEL_D_DIMENSION_TAG ${CGALPY_KERNEL_D_DIMENSION_TAG_DYNAMIC} CACHE INTERNAL "")
SET(CGALPY_KERNEL_D_DIMENSION_TAG_NAME "dynamic" CACHE STRING "The dD Kernel dimension tag")
set_property(CACHE CGALPY_KERNEL_D_DIMENSION_TAG_NAME PROPERTY STRINGS dynamic static)

# Default
SET(CGALPY_KERNEL_D_DIMENSION 2 CACHE INTERNAL "")

function(select_kernel_d_dimension_tag)
  add_definitions(-DCGALPY_KERNEL_D_DIMENSION_TAG=${CGALPY_KERNEL_D_DIMENSION_TAG})
  if (NOT CGALPY_KERNEL_D_DIMENSION MATCHES "^[0-9]+$")
    message("Error: Invalid dimension (${CGALPY_KERNEL_D_DIMENSION})")
    return()
  endif()
  add_definitions(-DCGALPY_KERNEL_D_DIMENSION=${CGALPY_KERNEL_D_DIMENSION})
endfunction()

function(select_kernel_d)
  if (CGALPY_KERNEL_D_BINDINGS)
    select_kernel_d_name()
    select_kernel_d_dimension_tag()
    add_definitions(-DCGALPY_KERNEL_D_BINDINGS)
  endif()
endfunction()

function(get_kernel_d_lib_name ret)
  list(GET CGALPY_KERNEL_D_SHORT_NAMES ${CGALPY_KERNEL_D} part1)
  capitalize_first(part1)
  list(GET CGALPY_KERNEL_D_DIMENSION_TAG_NAME ${CGALPY_KERNEL_D_DIMENSION_TAG} part2)
  capitalize_first(part2)
  if (${CGALPY_KERNEL_D_DIMENSION_TAG} STREQUAL ${CGALPY_KERNEL_D_DIMENSION_TAG_DYNAMIC})
    set(part3 ${CGALPY_KERNEL_D_DIMENSION})
  endif()
  set(${ret} "kerd${part1}${part2}${part3}" PARENT_SCOPE)
endfunction()

endif()
