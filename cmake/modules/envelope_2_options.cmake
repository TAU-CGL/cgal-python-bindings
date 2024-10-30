if(NOT ENVELOPE_2_OPTIONS_FILE_INCLUDED)
set(ENVELOPE_2_OPTIONS_FILE_INCLUDED)

# Selection
function(select_envelope_2)
  if(${CGALPY_ENVELOPE_2_BINDINGS})
    add_definitions(-DCGALPY_ENVELOPE_2_BINDINGS=)
  endif()
endfunction()

function(get_envelope_2_lib_name ret)
  set(${ret} "env2" PARENT_SCOPE)
endfunction()

endif()
