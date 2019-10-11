cmake_minimum_required(VERSION 3.5)
set(BOOST_MIN_VERSION 1.68.0)
set(PY_MIN_VERSION 3.5)
# packages

find_package(Python3 COMPONENTS Interpreter Development)
find_package(PythonLibs ${PY_MIN_VERSION} REQUIRED)
# Set the Boost python library name (e.g., python35)
set (CGALPY_BOOST_PYTHON_LIB_NAME python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR})
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS system thread ${CGALPY_BOOST_PYTHON_LIB_NAME})

# Add source files
# set(triangulations_bindings_SRC_FILES ${triangulations_bindings_SRC_FILES} lib/xxx.cpp)

# Add the generated files
set(triangulations_bindings_SRC_FILES ${triangulations_bindings_SRC_FILES} ${triangulations_bindings_AUTO_GENERATED_SRC_FILES})

# Add all include directories
include_directories(lib)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(include)
link_directories(${Boost_LIBRARY_DIRS})
# Add defines
add_definitions(-DCGALPY_triangulations_bindings_SOURCE)

if (WIN32)
  add_definitions(/wd4251)
else(WIN32)
  add_definitions(-DGL_GLEXT_PROTOTYPES)
endif(WIN32)

# Add library
# if (BUILD_SHARED_LIBS)
#   add_definitions(-DCGALPY_ALL_DYN_LINK)
#   add_library(triangulations_bindings SHARED ${triangulations_bindings_SRC_FILES})
# else(BUILD_SHARED_LIBS)
#   add_library(triangulations_bindings STATIC ${triangulations_bindings_SRC_FILES})
# endif(BUILD_SHARED_LIBS)

# set_target_properties(triangulations_bindings PROPERTIES
#   VERSION "${triangulations_bindings_SOVERSION}"
#   SOVERSION "${triangulations_bindings_SONAME_VERSION}")

# Add links
# if (NOT WIN32)
#   set (NON_WIN32_LIBRARIES ${X11_LIBRARIES} ${X11_Xxf86vm_LIB})
#   if (APPLE)
#     set (NON_WIN32_LIBRARIES ${NON_WIN32_LIBRARIES} /opt/X11/lib/libGL.dylib)
#   endif(APPLE)
# endif(NOT WIN32)

# Add Python bindings
add_library(triangulations_bindings_PYBINDINGS SHARED lib/triangulations_bindings.cpp)
if(WIN32)
set_target_properties(triangulations_bindings_PYBINDINGS PROPERTIES SUFFIX ".pyd")
endif()
target_link_libraries(triangulations_bindings_PYBINDINGS
                      ${triangulations_bindings}
		      ${NON_WIN32_LIBRARIES}
                      ${Boost_LIBRARIES}
	  	      ${PYTHON_LIBRARIES})
set_target_properties(triangulations_bindings_PYBINDINGS PROPERTIES OUTPUT_NAME
		      triangulations)
# don't prepend wrapper library name with lib
set_target_properties(triangulations_bindings_PYBINDINGS PROPERTIES PREFIX "")

# Installation

# Add Python bindings
if (CGALPY_DEF_PYBINDINGS)
  set_property(TARGET triangulations_bindings_PYBINDINGS PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
  set_property(TARGET triangulations_bindings_PYBINDINGS PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")
endif()

# set_property(TARGET triangulations_bindings PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
# set_property(TARGET triangulations_bindings PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")

if (CGALPY_DEF_PYBINDINGS)
  install(TARGETS triangulations_bindings_PYBINDINGS
          RUNTIME DESTINATION ${CGALPY_INSTALL_BIN_DIR}
          LIBRARY DESTINATION ${CGALPY_INSTALL_LIB_DIR}
          ARCHIVE DESTINATION ${CGALPY_INSTALL_LIB_DIR})
endif()

# install(TARGETS triangulations_bindings
#         RUNTIME DESTINATION ${CGALPY_INSTALL_BIN_DIR}
#         LIBRARY DESTINATION ${CGALPY_INSTALL_LIB_DIR}
#         ARCHIVE DESTINATION ${CGALPY_INSTALL_LIB_DIR})
