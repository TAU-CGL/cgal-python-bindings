# packages

# Add source files
set(CGALPY_TRI3_SRC_FILES ${CGALPY_TRI3_SRC_FILES} lib/alpha_shapes_3_bindings.cpp)
set(CGALPY_TRI3_SRC_FILES ${CGALPY_TRI3_SRC_FILES} lib/export_module.cpp)
set(CGALPY_TRI3_SRC_FILES ${CGALPY_TRI3_SRC_FILES} ../cgalpy/lib/kernel_bindings.cpp)
# set(CGALPY_TRI3_SRC_FILES ${CGALPY_TRI3_SRC_FILES} lib/triangulation_3_bindings.cpp)

# Add all include directories
include_directories(include)
include_directories(../cgalpy/include)

# Add dependencies
link_directories(${Boost_LIBRARY_DIRS})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_definitions(/wd4251)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

# Add Python bindings
if(BUILD_SHARED_LIBS)
  add_library(CGALPY_TRI3 SHARED ${CGALPY_TRI3_SRC_FILES})
else (BUILD_SHARED_LIBS)
  add_library(CGALPY_TRI3 STATIC ${CGALPY_TRI3_SRC_FILES})
endif(BUILD_SHARED_LIBS)

# set_target_properties(cgalpy PROPERTIES
#   VERSION "${cgalpy_SOVERSION}"
#   SOVERSION "${cgalpy_SONAME_VERSION}")

if(WIN32)
  set_target_properties(CGALPY PROPERTIES SUFFIX ".pyd")
endif()

target_link_libraries(CGALPY_TRI3
		      ${NON_WIN32_LIBRARIES}
                      ${Boost_LIBRARIES}
		      ${PYTHON_LIBRARIES})

list (GET CGALPY_KERNEL_SHORT_NAMES ${CGALPY_KERNEL} CGALPY_KERNEL_PART)
list (GET CGALPY_GEOMETRY_TRAITS_SHORT_NAMES ${CGALPY_GEOMETRY_TRAITS} CGALPY_GT_PART)
list (GET CGALPY_DCEL_SHORT_NAMES ${CGALPY_DCEL} CGALPY_DCEL_PART)
# set(CGALPY_LIB_NAME "tri3_${CGALPY_KERNEL_PART}_${CGALPY_GT_PART}${CGALPY_DCEL_PART}")
set(CGALPY_TRI3_LIB_NAME "tri3_${CGALPY_KERNEL_PART}")
set_target_properties(CGALPY_TRI3 PROPERTIES OUTPUT_NAME ${CGALPY_TRI3_LIB_NAME})
add_definitions(-DCGALPY_TRI3_MODULE_NAME=${CGALPY_TRI3_LIB_NAME})

# don't prepend wrapper library name with lib
set_target_properties(CGALPY_TRI3 PROPERTIES PREFIX "")

# Documentation
find_package(Doxygen)

if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                 ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc_tri3
		    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
     		    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
     		    COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)

# Installation
set_property(TARGET CGALPY_TRI3 PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
set_property(TARGET CGALPY_TRI3 PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")

install(TARGETS CGALPY_TRI3
        RUNTIME DESTINATION ${CGALPY_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${CGALPY_INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${CGALPY_INSTALL_LIB_DIR})
