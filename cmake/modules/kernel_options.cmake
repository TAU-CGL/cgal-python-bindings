if(NOT KERNEL_OPTIONS_FILE_INCLUDED)
set(KERNEL_OPTIONS_FILE_INCLUDED)

# Options
set(CGALPY_EPEC_KERNEL 0)
set(CGALPY_EPIC_KERNEL 1)

# Names
set(CGALPY_KERNEL_SHORT_NAMES "epec" "epic")
set(CGALPY_KERNEL_NAMES "epec" "epic")

# Default
SET(CGALPY_KERNEL_NAME "epec" CACHE STRING "The kernel to use")
set_property(CACHE CGALPY_KERNEL_NAME PROPERTY STRINGS epec epic)

function(select_kernel)
  if ("${CGALPY_KERNEL_NAME}" STREQUAL "epec")
    set(CGALPY_KERNEL ${CGALPY_EPEC_KERNEL} CACHE INTERNAL "")
  elseif ("${CGALPY_KERNEL_NAME}" STREQUAL "epic")
    set(CGALPY_KERNEL ${CGALPY_EPIC_KERNEL} CACHE INTERNAL "")
  endif()
  add_definitions(-DCGALPY_KERNEL=${CGALPY_KERNEL})
endfunction()

endif()
