set(CGALPY_PY_MIN_VERSION 3.7)
set(CGALPY_BOOST_MIN_VERSION 1.72.0)

# Find required CGAL library
if (CGALPY_WITH_VISUAL)
  find_package(Qt6 COMPONENTS Core Gui Widgets)
  if (NOT Qt6_FOUND)
    message(WARNING "Qt6 not found; all Qt6-dependant binding will be excluded.")
    set(CGALPY_WITH_VISUAL OFF CACHE BOOL "" FORCE)
  else ()
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
  endif()
endif()

if (CGALPY_WITH_VISUAL)
  find_package(CGAL REQUIRED COMPONENTS Core Qt6)
  if (NOT CGAL_Qt6_FOUND)
    message(WARNING "CGAL_Qt6 not found; all Qt6-dependant binding will be excluded.")
    set(CGALPY_WITH_VISUAL OFF CACHE BOOL "" FORCE)
  endif()
else()
  find_package(CGAL REQUIRED COMPONENTS Core)
endif()

if (NOT CGAL_FOUND AND NOT DEFINED ENV{CGALPY_PYPI})
  message(FATAL_ERROR "This library cannot be compiled, as it requires CGAL.")
  return()
endif ()

find_package(Eigen3 3.3.0 QUIET)
include(CGAL_Eigen3_support)
if (NOT TARGET CGAL::Eigen3_support)
  if (${CGALPY_KERNEL_D_BINDINGS} AND
    ((${CGALPY_KERNEL_D} EQUAL ${CGALPY_KERNEL_D_EPIC_D}) OR
    (${CGALPY_KERNEL_D} EQUAL ${CGALPY_KERNEL_D_EPEC_D})))
     SET(CGALPY_KERNEL_D_BINDINGS OFF CACHE BOOL "" FORCE)
     SET(CGALPY_SPATIAL_SEARCHING_BINDINGS OFF CACHE BOOL "" FORCE)
     SET(CGALPY_TRIANGULATION_D_BINDINGS OFF CACHE BOOL "" FORCE)
      message(WARNING "Eigen3 not found!\nTurning off the generations of\n(i) dD Kernel, \n(ii) spatial searching bindings, and\n(iii) dD Triangulation bindings")
  endif ()
  if (${CGALPY_POLYGON_MESH_PROCESSING_BINDINGS})
    SET(CGALPY_POLYGON_MESH_PROCESSING_BINDINGS OFF CACHE BOOL "" FORCE)
    message(WARNING "Eigen3 not found!\nTurning off the generations of Polygon Mesh Processing")
  endif ()
endif ()

# Find required Python library
if (SKBUILD)
    find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
    # ~~~~~~~~~ nanobind  ~~~~~~~~~~~~~~~~~~
    # The following part is copied from
    # https://github.com/wjakob/nanobind_example/blob/master/CMakeLists.txt
    # with slight modifications.
    # Constrain FindPython to find the Python version used by scikit-build
    set(Python_VERSION "${PYTHON_VERSION_STRING}")
    set(Python_EXECUTABLE "${PYTHON_EXECUTABLE}")
    # The following line will result in conflicts with conan.
    #  set(Python_INCLUDE_DIR "${PYTHON_INCLUDE_DIR}")
    set(Python_LIBRARIES "${PYTHON_LIBRARY}")

    if (MSVC)
        # MSVC needs a little extra help finding the Python library
        find_package(PythonInterp)
        find_package(Python)
    endif () # MSVC
else ()
    find_package(Python ${CGALPY_PY_MIN_VERSION} COMPONENTS Interpreter Development)
    set(CGALPY_PY_MAJOR_MINOR_VERSION "${Python_VERSION_MAJOR}${Python_VERSION_MINOR}")
endif ()

# Find required Boost library
if (SKBUILD)
    find_package(Boost REQUIRED headers)
else ()
    if (CGALPY_USE_SHARED_LIBS)
        set(Boost_USE_STATIC_LIBS OFF)
    else ()
        set(Boost_USE_STATIC_LIBS ON)
        add_definitions(-DBOOST_PYTHON_STATIC_LIB)
    endif ()

    find_package(Boost ${CGALPY_BOOST_MIN_VERSION} REQUIRED)
    if (NOT Boost_FOUND)
        message(STATUS "This library cannot be compiled, as it requires Boost.")
        return()
    endif ()
endif ()

# Handle Gmp and Mpfr libraries
if (NOT SKBUILD)
    # The following is needed for Windows. We extract the directory names
    # of the dlls that we depend on, and use them later on
    # \todo introduce a function and use it
    if (WIN32)
      set(FLAG OFF)
      foreach (item ${MPFR_LIBRARIES})
        if (FLAG)
          get_filename_component(MPFR_LIBRARY_DIR ${item} DIRECTORY)
          break ()
        elseif ((${item} STREQUAL "debug") OR (${item} STREQUAL "optimized"))
          set(FLAG ON)
        endif ()
      endforeach ()
      if ("${MPFR_LIBRARY_DIR}" STREQUAL "")
        get_filename_component(MPFR_LIBRARY_DIR ${MPFR_LIBRARIES} DIRECTORY)
      endif ()

      set(FLAG OFF)
      foreach (item ${GMP_LIBRARIES})
        if (FLAG)
          get_filename_component(GMP_LIBRARY_DIR ${item} DIRECTORY)
          break ()
        elseif ((${item} STREQUAL "debug") OR (${item} STREQUAL "optimized"))
          set(FLAG ON)
        endif ()
      endforeach ()
      if ("${GMP_LIBRARY_DIR}" STREQUAL "")
        get_filename_component(GMP_LIBRARY_DIR ${GMP_LIBRARIES} DIRECTORY)
      endif ()
    endif ()
endif ()

# Find required nanobind library
find_package(nanobind CONFIG REQUIRED)

# Handle Visual effects
if (CGALPY_WITH_VISUAL)
  add_definitions(-DCGALPY_HAS_VISUAL=)
endif()

# Add includes and libs
if (Qt6_FOUND AND NOT SKBUILD)
  include_directories(${Qt6Widgets_INCLUDE_DIRS})
  include_directories(${Qt6OpenGL_INCLUDE_DIRS})
endif()
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})

# Add some compiler options
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(-GR)
    add_compile_options(-vmg)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    add_compile_options(-Wno-error=non-pod-varargs)
    message(WARNING "detected Clang")
endif ()

if (MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

if (NOT SKBUILD)
    set(CGALPY_INSTALL_INC_DIR "include"
            CACHE STRING "The folder where CGALPY header files will be installed, relative to CMAKE_INSTALL_PREFIX")
    set(CGALPY_INSTALL_LIB_DIR "lib"
            CACHE STRING "The folder where CGALPY libraries will be installed, relative to CMAKE_INSTALL_PREFIX")
    set(CGALPY_INSTALL_BIN_DIR "bin"
            CACHE STRING "The folder where CGALPY executables, e.g., binaries and scripts, will be installed, relative to CMAKE_INSTALL_PREFIX")
    set(CGALPY_INSTALL_CMAKE_DIR "${CGALPY_INSTALL_LIB_DIR}/CGALPY"
            CACHE STRING "The folder where CGALPY CMake modules will be installed, relative to CMAKE_INSTALL_PREFIX")
    set(CGALPY_INSTALL_PYTHON_DIR "${CGALPY_INSTALL_LIB_DIR}/Python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}/dist-packages"
            CACHE STRING "The folder where CGALPY CMake modules will be installed, relative to CMAKE_INSTALL_PREFIX")
endif ()

select_basic_viewer()
select_kinetic_surface_reconstruction() # needs to be before 3D Point Set
select_point_set_processing() # needs to be before 3D Point Set
select_three_d_point_set()
select_two_d_range_and_neighbor_search()
select_alpha_shape_2()
select_alpha_shape_3()
select_arrangement_2()
select_bgl()
select_boolean_set_operations_2()
select_bounding_volumes()
select_convex_hull_2()
select_convex_hull_3()
select_envelope_2()
select_envelope_3()
select_geometric_object_generators()
select_kernel()
select_kernel_d()
select_minkowski_sum_2()
select_nef_3()
select_polyhedron_3()
select_polygon_2()
select_polygon_mesh_processing()
select_triangulated_surface_mesh_segmentation()
select_triangulated_surface_mesh_simplification()
select_triangulated_surface_mesh_skeletonization()
select_polygon_partitioning()
select_spatial_searching()
select_spatial_sorting()
select_surface_mesh()
select_surface_sweep_2()
select_shape_detection()
select_triangulation_2()
select_triangulation_3()
select_triangulation_d()
select_visibility_2()

# Add subdirectories
add_subdirectory(libs)
