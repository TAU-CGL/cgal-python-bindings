if(NOT ALPHA_SHAPE_3_OPTIONS_FILE_INCLUDED)
set(ALPHA_SHAPE_3_OPTIONS_FILE_INCLUDED)

# Options
set(CGALPY_AS3_TYPE_PLAIN 0)
set(CGALPY_AS3_TYPE_FIXED 1)

# Names
set(CGALPY_AS3_TYPE_SHORT_NAMES plain fixed)
set(CGALPY_AS3_TYPE_NAMES plain fixed)

# Default
SET(CGALPY_AS3_TYPE ${CGALPY_AS3_TYPE_PLAIN} CACHE INTERNAL "")
SET(CGALPY_AS3_TYPE_NAME "plain" CACHE STRING "The 3D Alpha shape construct")
set_property(CACHE CGALPY_AS3_TYPE_NAME PROPERTY STRINGS plain fixed)

# 3D comparison tag
set(CGALPY_AS3_EXACT_COMPARISON true CACHE BOOL "The exact comparison tag")

# Verification
function(verify_alpha_shape_3)
  if(${CGALPY_ALPHA_SHAPE_3_BINDINGS})
    # Check validity of vertex-base settings
    if((${CGALPY_TRI3_VERTEX_BASE_NAME} STREQUAL "plain") OR
       (${CGALPY_TRI3_VERTEX_BASE_NAME} STREQUAL "plainWithInfo") OR
       (${CGALPY_TRI3_VERTEX_BASE_NAME} STREQUAL "regular")  OR
       (${CGALPY_TRI3_VERTEX_BASE_NAME} STREQUAL "regularWithInfo"))
      message("Error: Invalid vertex base (${CGALPY_TRI3_VERTEX_BASE_NAME})")
      return()
    endif()
    # Check validity of cell-base settings
    if((${CGALPY_TRI3_CELL_BASE_NAME} STREQUAL "plain") OR
       (${CGALPY_TRI3_CELL_BASE_NAME} STREQUAL "plainWithInfo") OR
       (${CGALPY_TRI3_CELL_BASE_NAME} STREQUAL "regular")  OR
       (${CGALPY_TRI3_CELL_BASE_NAME} STREQUAL "regularWithInfo"))
      message("Error: Invalid vertex base (${CGALPY_TRI3_VERTEX_BASE_NAME})")
      return()
    endif()

    # Check validity of triangulation
    if((${CGALPY_TRI3_NAME} STREQUAL "[Rr]egular") AND
       (NOT ${CGALPY_TRI3_VERTEX_BASE_NAME} MATCHES "regular"))
      message("Error: Triangulation (${CGALPY_TRI3_NAME}) does not match vertex base (${CGALPY_TRI3_VERTEX_BASE_NAME})")
      return()
    endif()

    # Check validity of triangulation
    if((${CGALPY_TRI3_NAME} STREQUAL "[Rr]egular") AND
       (NOT ${CGALPY_TRI3_CELL_BASE_NAME} MATCHES "regular"))
      message("Error: Triangulation (${CGALPY_TRI3_NAME}) does not match cell base (${CGALPY_TRI3_CELL_BASE_NAME})")
      return()
    endif()
  endif()
endfunction()

function(select_alpha_shape_3_comparison)
  if(${CGALPY_AS3_EXACT_COMPARISON})
    set(CGALPY_AS3_EXACT_COMPARISON 1)
  else()
    set(CGALPY_AS3_EXACT_COMPARISON 0)
  endif()
  add_definitions(-DCGALPY_AS3_EXACT_COMPARISON=${CGALPY_AS3_EXACT_COMPARISON})
endfunction()

# Selection
function(select_alpha_shape_3)
  if(${CGALPY_ALPHA_SHAPE_3_BINDINGS})
    verify_alpha_shape_3()
    select_alpha_shape_3_comparison()

    add_definitions(-DCGALPY_ALPHA_SHAPE_3_BINDINGS)

    # Select Alpha shape
    if    ("${CGALPY_ALPHA_SHAPE_NAME}" STREQUAL "plain")
      set(CGALPY_ALPHA_SHAPE ${CGALPY_ALPHA_SHAPE_PLAIN} CACHE INTERNAL "")
    elseif("${CGALPY_ALPHA_SHAPE_NAME}" STREQUAL "fixed")
      set(CGALPY_ALPHA_SHAPE ${CGALPY_ALPHA_SHAPE_FIXED} CACHE INTERNAL "")
    endif()
    add_definitions(-DCGALPY_ALPHA_SHAPE=${CGALPY_ALPHA_SHAPE})
  endif()
endfunction()

function(get_alpha_shape_3_lib_name ret)
  list(GET CGALPY_AS3_TYPE_SHORT_NAMES ${CGALPY_AS3_TYPE} CGALPY_AS3_TYPE_PART)
  set(${ret} "as3_${CGALPY_AS3_TYPE_PART}" PARENT_SCOPE)
endfunction()

endif()
