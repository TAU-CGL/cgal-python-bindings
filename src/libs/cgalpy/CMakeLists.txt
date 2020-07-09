# packages

# Add source files
if (CGALPY_ARRANGEMENT_2_BINDINGS)
  # Geometry Traits
  if(${CGALPY_ARR2_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_algebraic_segment_traits_bindings.cpp)
  endif()
  if(${CGALPY_ARR2_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR2_BEZIER_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_Bezier_curve_traits_bindings.cpp)
  endif()
  if(${CGALPY_ARR2_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR2_CIRCLE_SEGMENT_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_circle_segment_traits_bindings.cpp)
  endif()
  if(${CGALPY_ARR2_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR2_CONIC_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_conic_traits_bindings.cpp)
  endif()
  if(${CGALPY_ARR2_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR2_LINEAR_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_linear_traits_bindings.cpp)
  endif()
  if(${CGALPY_ARR2_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_non_caching_segment_traits_bindings.cpp)
  endif()
  if(${CGALPY_ARR2_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR2_RATIONAL_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_rational_function_traits_bindings.cpp)
  endif()
  if(${CGALPY_ARR2_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR2_SEGMENT_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_segment_traits_bindings.cpp)
  endif()

  # Cells
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_vertex_bindings.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_halfedge_bindings.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_face_bindings.cpp)

  # Point Location
  if (CGALPY_ARR2_POINT_LOCATION_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_point_location_bindings.cpp)
  endif()

  # Object
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_object_bindings.cpp)

  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arrangement_2_bindings.cpp)
endif()

if (CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/boolean_set_operations_2_bindings.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/general_polygon_2_bindings.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/general_polygon_set_2_bindings.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/general_polygon_with_holes_2_bindings.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_set_2_bindings.cpp)
endif()

if (CGALPY_BOUNDING_VOLUMES_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/bounding_volumes_bindings.cpp)
endif()

if (CGALPY_CONVEX_HULL_2_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/convex_hull_2_bindings.cpp)
endif()

if (CGALPY_MINKOWSKI_SUM_2_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/minkowski_sum_2_bindings.cpp)
endif()

if (CGALPY_POLYGON_PARTITIONING_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_partitioning_bindings.cpp)
endif()

if (CGALPY_SPATIAL_SEARCHING_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/spatial_searching_bindings.cpp)
endif()

if (CGALPY_TRIANGULATION_2_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/triangulation_2_bindings.cpp)
endif()

if (CGALPY_POLYGON_2_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_2_bindings.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_with_holes_2_bindings.cpp)
endif()

if (CGALPY_ALPHA_SHAPE_3_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/alpha_shape_3_bindings.cpp)
endif()

set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_module.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/kernel_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/intersections_2_bindings.cpp)

# Add all include directories
include_directories(include)

# Add dependencies
link_directories(${Boost_LIBRARY_DIRS})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_definitions(/wd4251)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

# Add the library target
# Always build a shared library
add_library(CGALPY SHARED ${CGALPY_SRC_FILES})

# set_target_properties(cgalpy PROPERTIES
#   VERSION "${cgalpy_SOVERSION}"
#   SOVERSION "${cgalpy_SONAME_VERSION}")

target_link_libraries(CGALPY
                      ${Boost_LIBRARIES}
		      ${PYTHON_LIBRARIES})

set(CGALPY_LIB_NAME "CGALPY")
if (NOT CGALPY_FIXED_LIBRARY_NAME)
  list (GET CGALPY_KERNEL_SHORT_NAMES ${CGALPY_KERNEL} CGALPY_KERNEL_PART)
  set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${CGALPY_KERNEL_PART}")
  # 3D Alpha shape
  if (CGALPY_ALPHA_SHAPE_3_BINDINGS)
    get_alpha_shape_3_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 2D Arrangement
  if (CGALPY_ARRANGEMENT_2_BINDINGS)
    get_arrangement_2_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 2D Regularized Boolean operations
  if (CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS)
    get_boolean_set_operations_2_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # Bounding volumes
  if (CGALPY_BOUNDING_VOLUMES_BINDINGS)
    get_bounding_volumes_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 2D convex hull
  if (CGALPY_CONVEX_HULL_2_BINDINGS)
    get_convex_hull_2_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 2D Minkowski sum
  if (CGALPY_MINKOWSKI_SUM_2_BINDINGS)
    get_minkowski_sum_2_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 2D polygon partitioning
  if (CGALPY_POLYGON_PARTITIONING_BINDINGS)
    get_polygon_partitioning_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # Spatial searching
  if (CGALPY_SPATIAL_SEARCHING_BINDINGS)
    get_spatial_searching_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 2D triangulation
  if (CGALPY_TRIANGULATION_2_BINDINGS)
    get_triangulation_2_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 3D triangulation
  if (CGALPY_TRIANGULATION_3_BINDINGS)
    get_triangulation_3_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  set_target_properties(CGALPY PROPERTIES OUTPUT_NAME ${CGALPY_LIB_NAME})
endif()
add_definitions(-DCGALPY_MODULE_NAME=${CGALPY_LIB_NAME})

# don't prepend wrapper library name with lib
set_target_properties(CGALPY PROPERTIES PREFIX "")

if(WIN32)
  set_target_properties(CGALPY PROPERTIES SUFFIX ".pyd")
endif()

# Documentation
find_package(Doxygen)

if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                 ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc
		    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
     		    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
     		    COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)

# Installation
set_property(TARGET CGALPY PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
set_property(TARGET CGALPY PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")

install(TARGETS CGALPY
        RUNTIME DESTINATION ${CGALPY_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${CGALPY_INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${CGALPY_INSTALL_LIB_DIR})
