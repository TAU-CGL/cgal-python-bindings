# packages
find_package(SGAL REQUIRED)

# var - should be somehow taken from the SCGAL compilation.
set(SCGAL_KERNEL "0" CACHE STRING "CGAL Kernel")
set(SCGAL_NT "3" CACHE STRING "CGAL number type")
# defs
add_definitions(-DSCGAL_NT=${SCGAL_NT})
add_definitions(-DSCGAL_KERNEL=${SCGAL_KERNEL})

set(arr_2-ecep-seg_AUTO_GENERATED_SRC_FILES "" CACHE INTERNAL "")

# Add source files
# set(arr_2-ecep-seg_SRC_FILES ${arr_2-ecep-seg_SRC_FILES} lib/xxx.cpp)

# Add the generated files
set(arr_2-ecep-seg_SRC_FILES ${arr_2-ecep-seg_SRC_FILES} ${arr_2-ecep-seg_AUTO_GENERATED_SRC_FILES})

# Add all include directories
include_directories(lib)
include_directories(include)
include_directories(include/engines)
include_directories(${GENERATED_INC_DIR})
include_directories(${SGAL_INCLUDE_DIRS})

# Add defines
add_definitions(-DSGAL_arr_2-ecep-seg_SOURCE)

if (WIN32)
  add_definitions(/wd4251)
else(WIN32)
  add_definitions(-DGL_GLEXT_PROTOTYPES)
endif(WIN32)

add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)

# Add library
# if (BUILD_SHARED_LIBS)
#   add_definitions(-DSGAL_ALL_DYN_LINK)
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
if (CGALPY_DEF_PYBINDINGS)
  add_library(arr_2-ecep-seg_PYBINDINGS SHARED lib/Polygon_2.cpp)
  target_link_libraries(arr_2-ecep-seg_PYBINDINGS
                        ${arr_2-ecep-seg}
                        ${SGAL_SGAL_LIBRARY}
  		        ${ImageMagick_Magick++_LIBRARY}
  		        ${OPENGL_LIBRARIES}
  		        ${NON_WIN32_LIBRARIES}
                        ${Boost_LIBRARIES})
  set_target_properties(arr_2-ecep-seg_PYBINDINGS PROPERTIES OUTPUT_NAME arr_2-ecep-seg)
  # don't prepend wrapper library name with lib
  set_target_properties(arr_2-ecep-seg_PYBINDINGS PROPERTIES PREFIX "")
endif(CGALPY_DEF_PYBINDINGS)

# target_link_libraries(arr_2-ecep-seg
#                       ${SGAL_SGAL_LIBRARY}
#   			${ImageMagick_Magick++_LIBRARY}
#   			${OPENGL_LIBRARIES}
#   			${NON_WIN32_LIBRARIES})

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
  set_property(TARGET arr_2-ecep-seg_PYBINDINGS PROPERTY INSTALL_RPATH "${SGAL_DIR}/lib")
endif()

# set_property(TARGET arr_2-ecep-seg PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
# set_property(TARGET arr_2-ecep-seg PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")
# set_property(TARGET arr_2-ecep-seg PROPERTY INSTALL_RPATH "${SGAL_DIR}/lib")

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
