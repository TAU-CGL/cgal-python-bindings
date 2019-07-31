set(PY_VERSION 3.5)
set(BOOST_VERSION 1.64.0)

# Find required libraries
find_package(CGAL REQUIRED COMPONENTS Core)
if (NOT CGAL_FOUND)
  message(STATUS "This library cannot be compiled, as it requires CGAL.")
  return()
endif()
include(${CGAL_USE_FILE})

set(Boost_USE_STATIC_LIBS OFF)
set(CGALPY_BOOST_LIBRARIES system thread)
if (CGALPY_DEF_PYBINDINGS)
  find_package(PythonLibs ${PY_VERSION} REQUIRED)
  if (PYTHONLIBS_VERSION_STRING VERSION_LESS 3)
    set(CGALPY_BOOST_LIBRARIES ${CGALPY_BOOST_LIBRARIES} python)
  else()
    set(CGALPY_BOOST_LIBRARIES ${CGALPY_BOOST_LIBRARIES} python35)
  endif()
  include_directories(${PYTHON_INCLUDE_DIRS})
  add_definitions(-DCGALPY_DEF_PYBINDINGS)
endif()
find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS ${CGALPY_BOOST_LIBRARIES})

if (NOT Boost_FOUND)
  message(STATUS "This library cannot be compiled, as it requires Boost.")
  return()
endif()

find_package(ImageMagick REQUIRED COMPONENTS Magick++)
find_package(OpenGL REQUIRED)

# Add includes and libs
include_directories(${Boost_INCLUDE_DIR})
include_directories(${ImageMagick_Magick++_INCLUDE_DIRS})

if (WIN32)
  include_directories(libs/GL/include)
endif(WIN32)

# Add some compiler options
if (WIN32)
  add_definitions(-GR)
  add_definitions(-vmg)
endif()

# Use C++14 for this directory and its sub-directories.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (SGAL_TRACE)
  add_definitions(-DSGAL_TRACE)
endif()

set (CGALPY_INSTALL_INC_DIR "include" CACHE STRING "The folder where CGALPY header files will be installed, relative to CMAKE_INSTALL_PREFIX")
set (CGALPY_INSTALL_LIB_DIR "lib"     CACHE STRING "The folder where CGALPY libraries will be installed, relative to CMAKE_INSTALL_PREFIX")
set (CGALPY_INSTALL_BIN_DIR "bin"
  CACHE STRING "The folder where CGALPY executables, e.g., binaried and scripts, will be installed, relative to CMAKE_INSTALL_PREFIX")
set (CGALPY_INSTALL_CMAKE_DIR "${CGALPY_INSTALL_LIB_DIR}/CGALPY"
  CACHE STRING "The folder where CGALPY CMake modules will be installed, relative to CMAKE_INSTALL_PREFIX")

# Add subdirectories
add_subdirectory(libs)
