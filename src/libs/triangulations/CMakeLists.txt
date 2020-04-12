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
# set(triangulations_bindings_SRC_FILES ${triangulations_bindings_SRC_FILES} lib/triangulations_bindings.cpp)

# Add all include directories
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
if (BUILD_SHARED_LIBS)
  add_library(CGALPY_TRIANGULATIONS_PYBINDINGS SHARED ${triangulations_bindings_SRC_FILES})
else(BUILD_SHARED_LIBS)
  add_library(CGALPY_TRIANGULATIONS_PYBINDINGS STATIC ${triangulations_bindings_SRC_FILES})
endif(BUILD_SHARED_LIBS)

# set_target_properties(triangulations_bindings PROPERTIES
#   VERSION "${triangulations_bindings_SOVERSION}"
#   SOVERSION "${triangulations_bindings_SONAME_VERSION}")

if(WIN32)
  set_target_properties(CGALPY_TRIANGULATIONS_PYBINDINGS PROPERTIES SUFFIX ".pyd")
endif()

target_link_libraries(CGALPY_TRIANGULATIONS_PYBINDINGS
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
set_property(TARGET CGALPY_TRIANGULATIONS_PYBINDINGS PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
set_property(TARGET CGALPY_TRIANGULATIONS_PYBINDINGS PROPERTY INSTALL_RPATH "${CGAL_DIR}/lib")

install(TARGETS CGALPY_TRIANGULATIONS_PYBINDINGS
        RUNTIME DESTINATION ${CGALPY_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${CGALPY_INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${CGALPY_INSTALL_LIB_DIR})
