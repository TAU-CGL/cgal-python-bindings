# packages

# Add source files
# set(arr2_epec_seg_SRC_FILES ${arr2_epec_seg_SRC_FILES} lib/xxx.cpp)

# Add the generated files
set(arr2_epec_seg_SRC_FILES ${arr2_epec_seg_SRC_FILES} ${arr2_epec_seg_AUTO_GENERATED_SRC_FILES})

# Add all include directories
include_directories(lib)
include_directories(include)

# Add defines
add_definitions(-DCGALPY_arr2_epec_seg_SOURCE)

if (WIN32)
  add_definitions(/wd4251)
else(WIN32)
  add_definitions(-DGL_GLEXT_PROTOTYPES)
endif(WIN32)

# Add library
# if (BUILD_SHARED_LIBS)
#   add_definitions(-DCGALPY_ALL_DYN_LINK)
#   add_library(arr2_epec_seg SHARED ${arr2_epec_seg_SRC_FILES})
# else(BUILD_SHARED_LIBS)
#   add_library(arr2_epec_seg STATIC ${arr2_epec_seg_SRC_FILES})
# endif(BUILD_SHARED_LIBS)

# set_target_properties(arr2_epec_seg PROPERTIES
#   VERSION "${arr2_epec_seg_SOVERSION}"
#   SOVERSION "${arr2_epec_seg_SONAME_VERSION}")

# Add links
# if (NOT WIN32)
#   set (NON_WIN32_LIBRARIES ${X11_LIBRARIES} ${X11_Xxf86vm_LIB})
#   if (APPLE)
#     set (NON_WIN32_LIBRARIES ${NON_WIN32_LIBRARIES} /opt/X11/lib/libGL.dylib)
#   endif(APPLE)
# endif(NOT WIN32)

# Add Python bindings
add_library(arr2_epec_seg_PYBINDINGS SHARED lib/arr2_epec_seg.cpp lib/Kernel_bindings.cpp lib/Arrangement_2_bindings.cpp
lib/Vertex_bindings.cpp lib/Halfedge_bindings.cpp lib/Face_bindings.cpp lib/Intersections_2_bindings.cpp lib/Point_location_bindings.cpp
lib/Arr_segment_traits_bindings.cpp lib/Arr_linear_traits_bindings.cpp lib/Arr_circle_segment_traits_bindings.cpp
lib/Arr_non_caching_segment_traits_bindings.cpp lib/Arr_conic_traits_bindings.cpp lib/Arr_Bezier_curve_traits_bindings.cpp
lib/Arr_algabraic_segment_traits_bindings.cpp lib/Arr_rational_function_traits_bindings.cpp)
if(WIN32)
set_target_properties(arr2_epec_seg_PYBINDINGS PROPERTIES SUFFIX ".pyd")
endif()
target_link_libraries(arr2_epec_seg_PYBINDINGS
                      ${arr2_epec_seg}
		      ${NON_WIN32_LIBRARIES}
                      ${Boost_LIBRARIES}
					  ${PYTHON_LIBRARIES})
set_target_properties(arr2_epec_seg_PYBINDINGS PROPERTIES OUTPUT_NAME
		      arr2_epec_seg)
# don't prepend wrapper library name with lib
set_target_properties(arr2_epec_seg_PYBINDINGS PROPERTIES PREFIX "")

# target_link_libraries(arr_2-ecep-seg ${NON_WIN32_LIBRARIES})

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

# Add Python bindings
if (CGALPY_DEF_PYBINDINGS)
  set_property(TARGET arr2_epec_seg_PYBINDINGS PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
  set_property(TARGET arr2_epec_seg_PYBINDINGS PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")
endif()

# set_property(TARGET arr2_epec_seg PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
# set_property(TARGET arr2_epec_seg PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")

if (CGALPY_DEF_PYBINDINGS)
  install(TARGETS arr2_epec_seg_PYBINDINGS
          RUNTIME DESTINATION ${CGALPY_INSTALL_BIN_DIR}
          LIBRARY DESTINATION ${CGALPY_INSTALL_LIB_DIR}
          ARCHIVE DESTINATION ${CGALPY_INSTALL_LIB_DIR})
endif()

# install(TARGETS arr2_epec_seg
#         RUNTIME DESTINATION ${CGALPY_INSTALL_BIN_DIR}
#         LIBRARY DESTINATION ${CGALPY_INSTALL_LIB_DIR}
#         ARCHIVE DESTINATION ${CGALPY_INSTALL_LIB_DIR})
