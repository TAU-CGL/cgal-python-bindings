if(NOT POLYGON_2_OPTIONS_FILE_INCLUDED)
    set(POLYGON_2_OPTIONS_FILE_INCLUDED)

    function(select_polygon_2)
        if(${CGALPY_POLYGON_2_BINDINGS})
            add_definitions(-DCGALPY_POLYGON_2_BINDINGS=)
        endif()
    endfunction()

    function(get_polygon_2_lib_name ret)
        set(${ret} "pol2" PARENT_SCOPE)
    endfunction()

endif()
