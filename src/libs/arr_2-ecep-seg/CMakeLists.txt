# packages

# Add source files
# set(arr_2-ecep-seg_SRC_FILES ${arr_2-ecep-seg_SRC_FILES} lib/xxx.cpp)

# Add the generated files
set(arr_2-ecep-seg_SRC_FILES ${arr_2-ecep-seg_SRC_FILES} ${arr_2-ecep-seg_AUTO_GENERATED_SRC_FILES})

# Add all include directories
include_directories(lib)
include_directories(include)

# Add defines
add_definitions(-DCGALPY_arr_2-ecep-seg_SOURCE)

if (WIN32)
  add_definitions(/wd4251)
else(WIN32)
  add_definitions(-DGL_GLEXT_PROTOTYPES)
endif(WIN32)

# Add library
# if (BUILD_SHARED_LIBS)
#   add_definitions(-DCGALPY_ALL_DYN_LINK)
#   add_library(arr_2-ecep-seg SHARED ${arr_2-ecep-seg_SRC_FILES})
# else(BUILD_SHARED_LIBS)
#   add_library(arr_2-ecep-seg STATIC ${arr_2-ecep-seg_SRC_FILES})
# endif(BUILD_SHARED_LIBS)

# set_target_properties(arr_2-ecep-seg PROPERTIES
#   VERSION "${arr_2-ecep-seg_SOVERSION}"
#   SOVERSION "${arr_2-ecep-seg_SONAME_VERSION}")

# Add links
# if (NOT WIN32)
#   set (NON_WIN32_LIBRARIES ${X11_LIBRARIES} ${X11_Xxf86vm_LIB})
#   if (APPLE)
#     set (NON_WIN32_LIBRARIES ${NON_WIN32_LIBRARIES} /opt/X11/lib/libGL.dylib)
#   endif(APPLE)
# endif(NOT WIN32)

# Add Python bindings
add_library(arr_2-ecep-seg_PYBINDINGS SHARED lib/Polygon_2.cpp)
target_link_libraries(arr_2-ecep-seg_PYBINDINGS
                      ${arr_2-ecep-seg}
		      ${NON_WIN32_LIBRARIES}
                      ${Boost_LIBRARIES})
set_target_properties(arr_2-ecep-seg_PYBINDINGS PROPERTIES OUTPUT_NAME
		      arr_2-ecep-seg)
# don't prepend wrapper library name with lib
set_target_properties(arr_2-ecep-seg_PYBINDINGS PROPERTIES PREFIX "")

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
  set_property(TARGET arr_2-ecep-seg_PYBINDINGS PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
  set_property(TARGET arr_2-ecep-seg_PYBINDINGS PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")
endif()

# set_property(TARGET arr_2-ecep-seg PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
# set_property(TARGET arr_2-ecep-seg PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")

if (CGALPY_DEF_PYBINDINGS)
  install(TARGETS arr_2-ecep-seg_PYBINDINGS
          RUNTIME DESTINATION ${CGALPY_INSTALL_BIN_DIR}
          LIBRARY DESTINATION ${CGALPY_INSTALL_LIB_DIR}
          ARCHIVE DESTINATION ${CGALPY_INSTALL_LIB_DIR})
endif()

# install(TARGETS arr_2-ecep-seg
#         RUNTIME DESTINATION ${CGALPY_INSTALL_BIN_DIR}
#         LIBRARY DESTINATION ${CGALPY_INSTALL_LIB_DIR}
#         ARCHIVE DESTINATION ${CGALPY_INSTALL_LIB_DIR})
