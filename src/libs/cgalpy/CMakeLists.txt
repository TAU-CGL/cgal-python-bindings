# packages

# Add source files
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arrangement_2_bindings.cpp)

if(${CGALPY_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS})
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_algebraic_segment_traits_bindings.cpp)
endif()
if(${CGALPY_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR_BEZIER_GEOMETRY_TRAITS})
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_Bezier_curve_traits_bindings.cpp)
endif()
if(${CGALPY_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR_CIRCLE_SEGMENT_GEOMETRY_TRAITS})
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_circle_segment_traits_bindings.cpp)
endif()
if(${CGALPY_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR_CONIC_GEOMETRY_TRAITS})
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_conic_traits_bindings.cpp)
endif()
if(${CGALPY_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR_LINEAR_GEOMETRY_TRAITS})
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_linear_traits_bindings.cpp)
endif()
if(${CGALPY_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR_NON_CACHING_SEGMENT_GEOMETRY_TRAITS})
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_non_caching_segment_traits_bindings.cpp)
endif()
if(${CGALPY_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR_RATIONAL_GEOMETRY_TRAITS})
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_rational_function_traits_bindings.cpp)
endif()
if(${CGALPY_GEOMETRY_TRAITS} EQUAL ${CGALPY_ARR_SEGMENT_GEOMETRY_TRAITS})
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_segment_traits_bindings.cpp)
endif()

set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/boolean_set_operations_2_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/bounding_volumes_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/convex_hull_2_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_module.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/face_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/general_polygon_2_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/general_polygon_set_2_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/general_polygon_with_holes_2_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/kernel_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/halfedge_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/intersections_2_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/object_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/minkowski_sum_2_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/point_location_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_2_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_partitioning_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_set_2_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_with_holes_2_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/spatial_searching_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/triangulation_2_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/vertex_bindings.cpp)

# Add all include directories
include_directories(include)

# Add dependencies
link_directories(${Boost_LIBRARY_DIRS})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_definitions(/wd4251)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

# Add Python bindings
if(BUILD_SHARED_LIBS)
  add_library(CGALPY SHARED ${CGALPY_SRC_FILES})
else (BUILD_SHARED_LIBS)
  add_library(CGALPY STATIC ${CGALPY_SRC_FILES})
endif(BUILD_SHARED_LIBS)

# set_target_properties(cgalpy PROPERTIES
#   VERSION "${cgalpy_SOVERSION}"
#   SOVERSION "${cgalpy_SONAME_VERSION}")

if(WIN32)
  set_target_properties(CGALPY PROPERTIES SUFFIX ".pyd")
endif()

target_link_libraries(CGALPY
		      ${NON_WIN32_LIBRARIES}
                      ${Boost_LIBRARIES}
		      ${PYTHON_LIBRARIES})

list (GET CGALPY_KERNEL_SHORT_NAMES ${CGALPY_KERNEL} CGALPY_KERNEL_PART)
list (GET CGALPY_GEOMETRY_TRAITS_SHORT_NAMES ${CGALPY_GEOMETRY_TRAITS} CGALPY_GT_PART)
list (GET CGALPY_DCEL_SHORT_NAMES ${CGALPY_DCEL} CGALPY_DCEL_PART)
set(CGALPY_LIB_NAME "arr2_${CGALPY_KERNEL_PART}_${CGALPY_GT_PART}${CGALPY_DCEL_PART}")
set_target_properties(CGALPY PROPERTIES OUTPUT_NAME ${CGALPY_LIB_NAME})

# don't prepend wrapper library name with lib
set_target_properties(CGALPY PROPERTIES PREFIX "")

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
