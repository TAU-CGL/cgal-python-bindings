cmake_minimum_required(VERSION 3.5)
set(BOOST_VERSION 1.69.0)
set(PY_VERSION 3.7)
# packages

find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS system thread python37)
find_package(Python3 COMPONENTS Interpreter Development)
find_package(PythonLibs ${PY_VERSION} REQUIRED)

# Add source files
# set(shared_vector_SRC_FILES ${shared_vector_SRC_FILES} lib/xxx.cpp)

# Add the generated files
set(shared_vector_SRC_FILES ${shared_vector_SRC_FILES} ${shared_vector_AUTO_GENERATED_SRC_FILES})

# Add all include directories
include_directories(lib)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(include)
link_directories(${Boost_LIBRARY_DIRS})
# Add defines
add_definitions(-DCGALPY_shared_vector_SOURCE)

if (WIN32)
  add_definitions(/wd4251)
else(WIN32)
  add_definitions(-DGL_GLEXT_PROTOTYPES)
endif(WIN32)

# Add library
# if (BUILD_SHARED_LIBS)
#   add_definitions(-DCGALPY_ALL_DYN_LINK)
#   add_library(shared_vector SHARED ${shared_vector_SRC_FILES})
# else(BUILD_SHARED_LIBS)
#   add_library(shared_vector STATIC ${shared_vector_SRC_FILES})
# endif(BUILD_SHARED_LIBS)

# set_target_properties(shared_vector PROPERTIES
#   VERSION "${shared_vector_SOVERSION}"
#   SOVERSION "${shared_vector_SONAME_VERSION}")

# Add links
# if (NOT WIN32)
#   set (NON_WIN32_LIBRARIES ${X11_LIBRARIES} ${X11_Xxf86vm_LIB})
#   if (APPLE)
#     set (NON_WIN32_LIBRARIES ${NON_WIN32_LIBRARIES} /opt/X11/lib/libGL.dylib)
#   endif(APPLE)
# endif(NOT WIN32)

# Add Python bindings
add_library(shared_vector_PYBINDINGS SHARED lib/shared_vector.cpp)
if(WIN32)
set_target_properties(shared_vector_PYBINDINGS PROPERTIES SUFFIX ".pyd")
endif()
target_link_libraries(shared_vector_PYBINDINGS
                      ${shared_vector}
		      ${NON_WIN32_LIBRARIES}
                      ${Boost_LIBRARIES}
	  	      ${PYTHON_LIBRARIES})
set_target_properties(shared_vector_PYBINDINGS PROPERTIES OUTPUT_NAME
		      shared_vector)
# don't prepend wrapper library name with lib
set_target_properties(shared_vector_PYBINDINGS PROPERTIES PREFIX "")

# Installation

# Add Python bindings
if (CGALPY_DEF_PYBINDINGS)
  set_property(TARGET shared_vector_PYBINDINGS PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
  set_property(TARGET shared_vector_PYBINDINGS PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")
endif()

# set_property(TARGET shared_vector PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
# set_property(TARGET shared_vector PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")

if (CGALPY_DEF_PYBINDINGS)
  install(TARGETS shared_vector_PYBINDINGS
          RUNTIME DESTINATION ${CGALPY_INSTALL_BIN_DIR}
          LIBRARY DESTINATION ${CGALPY_INSTALL_LIB_DIR}
          ARCHIVE DESTINATION ${CGALPY_INSTALL_LIB_DIR})
endif()

# install(TARGETS shared_vector
#         RUNTIME DESTINATION ${CGALPY_INSTALL_BIN_DIR}
#         LIBRARY DESTINATION ${CGALPY_INSTALL_LIB_DIR}
#         ARCHIVE DESTINATION ${CGALPY_INSTALL_LIB_DIR})
