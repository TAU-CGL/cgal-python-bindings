######## Packages ###############################################################
################################################################################

# Generate config.hpp
configure_file(config.hpp.in include/config.hpp)

# Add source files
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_module.cpp)

if (GMP_FOUND)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_gmp.cpp)
endif()

#############
# Add modules
#############

# 2-3D Kernel
if (CGALPY_KERNEL_BINDINGS)
  list (APPEND CGALPY_MODULES Ker)
  set (CGALPY_Ker_FILTERS [=[{}]=])
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/kernel_bindings.cpp)
  if (CGALPY_KERNEL_INTERSECTION_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/intersections_2_bindings.cpp)
  endif()
endif()

# dD Kernel
if (CGALPY_KERNEL_D_BINDINGS)
  list (APPEND CGALPY_MODULES Kerd)
  set (CGALPY_Kerd_FILTERS [=[{}]=])
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/kernel_d_bindings.cpp)
endif()

# 2D Alpha shape
if (CGALPY_ALPHA_SHAPE_2_BINDINGS)
  list (APPEND CGALPY_MODULES AS2)
  set (CGALPY_AS2_FILTERS [=[{}]=])
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/alpha_shape_2_bindings.cpp)
endif()

# 3D Alpha shape
if (CGALPY_ALPHA_SHAPE_3_BINDINGS)
  list (APPEND CGALPY_MODULES AS3)
  set (CGALPY_AS3_FILTERS [=[{}]=])
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/alpha_shape_3_bindings.cpp)
endif()

# 2D Arrangement
if (CGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS)
  list (APPEND CGALPY_MODULES Aos2)
  set (CGALPY_Aos2_FILTERS [=[{}]=])
  # Geometry Traits
  if(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS})
    set (CGALPY_Aos2_FILTERS [=[\{ \
      \"Aos2.Geometry_traits_2\":\"\\\"Arr_algebraic_segment_traits_2\\\"\", \
      \"Arr_algebraic_segment_traits_2\":\"true\" \
      \}]=])
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_algebraic_segment_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_BEZIER_GEOMETRY_TRAITS})
    set (CGALPY_Aos2_FILTERS [=[\{ \
      \"Aos2.Geometry_traits_2\":\"\\\"Arr_Bezier_curve_traits_2\\\"\", \
      \"Arr_Bezier_curve_traits_2\":\"true\" \
      \}]=])
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_Bezier_curve_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS})
    set (CGALPY_Aos2_FILTERS [=[\{ \
      \"Aos2.Geometry_traits_2\":\"\\\"Arr_circle_segment_traits_2\\\"\", \
      \"Arr_circle_segment_traits_2\":\"true\" \
      \}]=])
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_circle_segment_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_CONIC_GEOMETRY_TRAITS})
    set (CGALPY_Aos2_FILTERS [=[\{ \
      \"Aos2.Geometry_traits_2\":\"\\\"Arr_conic_traits_2\\\"\", \
      \"Arr_conic_traits_2\":\"true\" \
      \}]=])
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_conic_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS})
    set (CGALPY_Aos2_FILTERS [=[\{ \
      \"Aos2.Geometry_traits_2\":\"\\\"Arr_linear_traits_2\\\"\", \
      \"Arr_linear_traits_2\":\"true\" \
      \}]=])
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_linear_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS})
    set (CGALPY_Aos2_FILTERS [=[\{ \
      \"Arr_non_caching_segment_basic_traits_2\":\"true\", \
      \"AosBasicTraits_2.Point_2\":\"false\" \
      \}]=])
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_non_caching_segment_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_RATIONAL_GEOMETRY_TRAITS})
    set (CGALPY_Aos2_FILTERS [=[\{ \
      \"Aos2.Geometry_traits_2\":\"\\\"Arr_rational_function_traits_2\\\"\", \
      \"Arr_rational_function_traits_2\":\"true\" \
      \}]=])
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_rational_function_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS})
    set (CGALPY_Aos2_FILTERS [=[\{ \
      \"Aos2.Geometry_traits_2\":\"\\\"Arr_segment_traits_2\\\"\", \
      \"Arr_segment_traits_2\":\"true\", \
      \"AosBasicTraits_2.Point_2\":\"false\", \
      \"AosBasicTraits_2.Equal_2\":\"false\" \
      \}]=])
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_segment_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_TRAITS})
    set (CGALPY_Aos2_FILTERS [=[\{ \
      \"Aos2.Geometry_traits_2\":\"\\\"Arr_geodesic_arc_on_sphere_traits_2\\\"\", \
      \"Arr_geodesic_arc_on_sphere_traits_2\":\"true\" \
      \}]=])
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_geodesic_arc_on_sphere_traits_bindings.cpp)
  endif()

  # Cells
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_vertex_bindings.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_halfedge_bindings.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_face_bindings.cpp)

  # Point Location
  if (CGALPY_AOS2_POINT_LOCATION_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_point_location_bindings.cpp)
  endif()

  # Object
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_object_bindings.cpp)

  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arrangement_on_surface_2_bindings.cpp)
endif()

# 2D Regularized Boolean operations
if (CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS)
  list (APPEND CGALPY_MODULES Bso2)
  set (CGALPY_Bso2_FILTERS [=[{}]=])
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/boolean_set_operations_2_bindings.cpp)

  if(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_set_2_bindings.cpp)
  else()
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/general_polygon_set_2_bindings.cpp)
  endif()

  # Geometry Traits
  if(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/gps_segment_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/gps_segment_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/gps_circle_segment_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/gps_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_BEZIER_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/gps_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_CONIC_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/gps_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/gps_traits_bindings.cpp)
  elseif(${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_RATIONAL_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/gps_traits_bindings.cpp)
  endif()

endif()

# Bounding volumes
if (CGALPY_BOUNDING_VOLUMES_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/bounding_volumes_bindings.cpp)
  list (APPEND CGALPY_MODULES BV)
  set (CGALPY_BV_FILTERS [=[{}]=])
endif()
# 2D convex hull
if (CGALPY_CONVEX_HULL_2_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/convex_hull_2_bindings.cpp)
  list (APPEND CGALPY_MODULES CH2)
  set (CGALPY_CH2_FILTERS [=[{}]=])
endif()
# 2D Minkowski sum
if (CGALPY_MINKOWSKI_SUM_2_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/minkowski_sum_2_bindings.cpp)
  list (APPEND CGALPY_MODULES MS2)
  set (CGALPY_MS2_FILTERS [=[{}]=])
endif()
# 2D Polygon
if (CGALPY_POLYGON_2_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_2_bindings.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_with_holes_2_bindings.cpp)
  list (APPEND CGALPY_MODULES Pol2)
  set (CGALPY_Pol2_FILTERS [=[{}]=])
endif()
# 2D polygon partitioning
if (CGALPY_POLYGON_PARTITIONING_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_partitioning_bindings.cpp)
  list (APPEND CGALPY_MODULES PP)
  set (CGALPY_PP_FILTERS [=[{}]=])
endif()
# Spatial searching
if (CGALPY_SPATIAL_SEARCHING_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/spatial_searching_bindings.cpp)
  list (APPEND CGALPY_MODULES SS)
  set (CGALPY_SS_FILTERS [=[{}]=])
endif()
# 2D triangulation
if (CGALPY_TRIANGULATION_2_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/triangulation_2_bindings.cpp)
  list (APPEND CGALPY_MODULES Tri2)
  set (CGALPY_Tri2_FILTERS [=[{}]=])
endif()
# 3D triangulation
if (CGALPY_TRIANGULATION_3_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/triangulation_3_bindings.cpp)
  list (APPEND CGALPY_MODULES Tri3)
  set (CGALPY_Tri3_FILTERS [=[{}]=])
endif()

# Add all include directories
include_directories(include)

# Add dependencies
link_directories(${Boost_LIBRARY_DIRS})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_definitions(/wd4251)
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

# Add the library target
# Always build a shared library
add_library(CGALPY SHARED ${CGALPY_SRC_FILES})
set(CGALPY_BUILD_DEPENDS CGALPY)

set_target_properties(CGALPY PROPERTIES
  VERSION "${CMAKE_PROJECT_VERSION}"
  SOVERSION "${CMAKE_PROJECT_VERSION_MAJOR}")

target_link_libraries(CGALPY
  ${Boost_LIBRARIES}
  ${PYTHON_LIBRARIES})

set(CGALPY_LIB_NAME "CGALPY")
if (NOT CGALPY_FIXED_LIBRARY_NAME)
  if (CGALPY_KERNEL_BINDINGS)
    get_kernel_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # dD Kernel
  if (CGALPY_KERNEL_D_BINDINGS)
    get_kernel_d_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 2D Alpha shape
  if (CGALPY_ALPHA_SHAPE_2_BINDINGS)
    get_alpha_shape_2_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 3D Alpha shape
  if (CGALPY_ALPHA_SHAPE_3_BINDINGS)
    get_alpha_shape_3_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 2D Arrangement
  if (CGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS)
    get_arrangement_on_surface_2_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 2D Regularized Boolean operations
  if (CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS)
    get_boolean_set_operations_2_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # Bounding volumes
  if (CGALPY_BOUNDING_VOLUMES_BINDINGS)
    get_bounding_volumes_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 2D convex hull
  if (CGALPY_CONVEX_HULL_2_BINDINGS)
    get_convex_hull_2_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 2D Polygon
  if (CGALPY_POLYGON_2_BINDINGS)
    get_polygon_2_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 2D Minkowski sum
  if (CGALPY_MINKOWSKI_SUM_2_BINDINGS)
    get_minkowski_sum_2_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 2D polygon partitioning
  if (CGALPY_POLYGON_PARTITIONING_BINDINGS)
    get_polygon_partitioning_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # Spatial searching
  if (CGALPY_SPATIAL_SEARCHING_BINDINGS)
    get_spatial_searching_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 2D triangulation
  if (CGALPY_TRIANGULATION_2_BINDINGS)
    get_triangulation_2_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  # 3D triangulation
  if (CGALPY_TRIANGULATION_3_BINDINGS)
    get_triangulation_3_lib_name(ret)
    set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
  endif()
  set_target_properties(CGALPY PROPERTIES OUTPUT_NAME ${CGALPY_LIB_NAME})
endif()
set(CGALPY_LIB_DIR "${CMAKE_BINARY_DIR}/src/libs/cgalpy")
reportWithEmphasis(${CGALPY_LIB_NAME} ${CGALPY_LIB_DIR} ${CGALPY_INSTALL_PYTHON_DIR})

add_definitions(-DCGALPY_MODULE_NAME=${CGALPY_LIB_NAME})

# don't prepend wrapper library name with lib
set_target_properties(CGALPY PROPERTIES PREFIX "")

set (CGALPY_LIBRARY_SUFFIX ".so")
if(WIN32)
  set (CGALPY_LIBRARY_SUFFIX ".pyd")
  set_target_properties(CGALPY PROPERTIES SUFFIX ".pyd")
endif()

################################################################################
# Documentation
################################################################################

# Doxygen
find_package(Doxygen)

if(DOXYGEN_FOUND)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    COMMAND "${CMAKE_COMMAND}"
    "-DFILE_IN=${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
    "-DFILE_OUT=${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
    -P "${CGALPY_MODULE_DIR}/BuildTimeFile.cmake"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")

  set(CGALPY_CPP_DOC_DEPENDS
    ${CGALPY_SRC_FILES}
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    COMMAND "${CMAKE_COMMAND}" -E touch "${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp"
    DEPENDS ${CGALPY_CPP_DOC_DEPENDS}
    COMMENT "Generating API documentation with Doxygen" VERBATIM)
  list(APPEND CGALPY_CPP_DOC_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/doxygen.stamp")
  add_custom_target(
    CGALPY_CPP_DOC
    DEPENDS ${CGALPY_CPP_DOC_DEPENDS})
  if(NOT TARGET doc)
    add_custom_target(doc)
  endif()
  add_dependencies(doc CGALPY_CPP_DOC)
endif(DOXYGEN_FOUND)

# Sphinx
find_package(Sphinx)

if(Sphinx_FOUND)
  add_sphinx_document(${CGALPY_LIB_NAME}
    MODULES "${CGALPY_MODULES}"
    AUTHORS "${CGALPY_PROJECT_AUTHORS}"
    CONF_FILE "${CMAKE_CURRENT_LIST_DIR}/doc/source/conf.py"
    INDEX_FILE "${CMAKE_CURRENT_LIST_DIR}/doc/source/index.rst")
else()
  message(WARNING "No Sphinx found. Documentation target not available.")
endif()

################################################################################
# Stubs (Type Annotation)
################################################################################
set (CGALPY_SRC_STUBS_DIR "${CMAKE_CURRENT_LIST_DIR}/stubs")
set (CGALPY_MODULES_DIR "${CGALPY_SRC_STUBS_DIR}/modules")
set (CGALPY_STUBS_GENERATOR "${CGALPY_SRC_STUBS_DIR}/generate.py")
set (CGALPY_DST_STUBS_DIR "${CMAKE_BINARY_DIR}/src/libs/cgalpy")

set (CGALPY_MODULE_OUTPUT ${CGALPY_DST_STUBS_DIR}/${CGALPY_LIB_NAME}.pyi)
set (CGALPY_MODULE_INPUT ${CGALPY_MODULES_DIR}/CGALPY.json)
string(REPLACE ";" " " STR_MODULES "${CGALPY_MODULES}")
set (CGALPY_MODULE_ARGS
  --output-file ${CGALPY_LIB_NAME}.pyi
  --input-path ${CGALPY_MODULES_DIR}
  --output-path ${CGALPY_DST_STUBS_DIR}
  --imports "${STR_MODULES}"
  CGALPY.json)

add_custom_command(
  OUTPUT ${CGALPY_MODULE_OUTPUT}
  COMMAND ${CGALPY_STUBS_GENERATOR}
  ARGS ${CGALPY_MODULE_ARGS}
  DEPENDS ${CGALPY_STUBS_GENERATOR} ${CGALPY_MODULE_INPUT})

set (CGALPY_STUBS ${CGALPY_MODULE_OUTPUT})

function(add_module name)
  set (MODULE_OUTPUT_PATH ${CGALPY_DST_STUBS_DIR}/${name})
  set (MODULE_OUTPUT ${MODULE_OUTPUT_PATH}/__init__.pyi)
  set (MODULE_INPUT ${CGALPY_MODULES_DIR}/${name}.json)
  set (MODULE_ARGS
    --filters ${CGALPY_${name}_FILTERS}
    --name ${name}
    --input-path ${CGALPY_MODULES_DIR}
    --output-path ${CGALPY_DST_STUBS_DIR})

  # There is no need to generate the directory, as the Python script is
  # responsible for this task.
  # file(MAKE_DIRECTORY ${MODULE_OUTPUT_PATH})

  add_custom_command(
    OUTPUT ${MODULE_OUTPUT}
    COMMAND ${CGALPY_STUBS_GENERATOR}
    ARGS ${MODULE_ARGS}
    DEPENDS ${CGALPY_STUBS_GENERATOR} ${MODULE_INPUT})

  set(CGALPY_STUBS "${CGALPY_STUBS}" ${MODULE_OUTPUT} PARENT_SCOPE)
endfunction()

foreach(MODULE IN LISTS CGALPY_MODULES)
  add_module(${MODULE})
endforeach()

add_custom_target(STUBS ALL DEPENDS ${CGALPY_STUBS})
list(APPEND CGALPY_BUILD_DEPENDS ${CGALPY_STUBS})

################################################################################
# Post build
################################################################################
# Setup generation
set (CGALPY_SRC_SETUP "${CMAKE_CURRENT_LIST_DIR}/setup.py")
set (CGALPY_TRG_SETUP "${CGALPY_LIB_DIR}/setup.py")

# It is possible to obtain the linker file name only at run time.
# This seems an overkill
# add_custom_command(TARGET CGALPY PRE_BUILD
#   COMMAND echo "$<TARGET_LINKER_FILE_NAME:CGALPY>" VERBATIM)
set (CGALPY_LINKER_FILE_NAME ${CGALPY_LIB_NAME}${CGALPY_LIBRARY_SUFFIX})

set (CGALPY_SETUP_DATA "'${CGALPY_LINKER_FILE_NAME}'"
  "'${CGALPY_LIB_NAME}.pyi'")
foreach(MODULE IN LISTS CGALPY_MODULES)
  list(APPEND CGALPY_SETUP_DATA "'${MODULE}/__init__.pyi'")
endforeach()
string (REPLACE ";" "," CGALPY_SETUP_DATA "${CGALPY_SETUP_DATA}")

add_custom_command(
  OUTPUT ${CGALPY_TRG_SETUP}
  COMMAND "${CMAKE_COMMAND}"
  "-DFILE_IN=${CGALPY_SRC_SETUP}"
  "-DFILE_OUT=${CGALPY_TRG_SETUP}"
  "-DTARGET_NAME=${CGALPY_LIB_NAME}"
  "-DTARGET_DATA='${CGALPY_SETUP_DATA}'"
  "-DTARGET_MODULES=${CGALPY_LIB_NAME}"
  "-DTARGET_AUTHOR=${CGALPY_PROJECT_AUTHORS}"
  "-DTARGET_EMAIL=${CGALPY_PROJECT_EMAILS}"
  "-DTARGET_URL=${CGALPY_PROJECT_URL}"
  -P "${CGALPY_MODULE_DIR}/BuildTimeFile.cmake"
  VERBATIM
  DEPENDS ${CGALPY_SRC_SETUP})

list(APPEND CGALPY_BUILD_DEPENDS ${CGALPY_TRG_SETUP})

set (CGALPY_SRC_README "${CMAKE_CURRENT_LIST_DIR}/README.rst")
set (CGALPY_TRG_README "${CGALPY_LIB_DIR}/README.rst")
add_custom_command(
  OUTPUT ${CGALPY_TRG_README}
  COMMAND "${CMAKE_COMMAND}"
  "-DFILE_IN=${CGALPY_SRC_README}"
  "-DFILE_OUT=${CGALPY_TRG_README}"
  "-DTARGET_NAME=${CGALPY_LIB_NAME}"
  -P "${CGALPY_MODULE_DIR}/BuildTimeFile.cmake"
  DEPENDS ${CGALPY_SRC_README})

list(APPEND CGALPY_BUILD_DEPENDS ${CGALPY_TRG_README})

# Setup application
set (CGALPY_BUILD_OUTPUT_DIR "${CGALPY_LIB_DIR}/dist")
set (CGALPY_TAR ${CGALPY_BUILD_OUTPUT_DIR}/${CGALPY_LIB_NAME}-1.0.tar.gz)
set (CGALPY_WHL ${CGALPY_BUILD_OUTPUT_DIR}/${CGALPY_LIB_NAME}-1.0-py3-none-any.whl)
set (CGALPY_BUILD_OUTPUT ${CGALPY_TAR} ${CGALPY_WHL})
set (CGALPY_BUILD_CMD ${Python3_EXECUTABLE})
set (CGALPY_BUILD_ARGS -m build ${CGALPY_LIB_DIR})
set (CGALPY_BUILD_DEPENDS CGALPY ${CGALPY_STUBS} ${CGALPY_TRG_SETUP} ${CGALPY_TRG_README})
add_custom_command(
  OUTPUT ${CGALPY_BUILD_OUTPUT}
  COMMAND ${CGALPY_BUILD_CMD}
  ARGS ${CGALPY_BUILD_ARGS}
  DEPENDS ${CGALPY_BUILD_DEPENDS})

add_custom_target(BUILD ALL DEPENDS ${CGALPY_BUILD_OUTPUT})

################################################################################
# Installation
################################################################################
install(TARGETS CGALPY
  RUNTIME DESTINATION ${CGALPY_INSTALL_BIN_DIR}
  LIBRARY DESTINATION ${CGALPY_INSTALL_PYTHON_DIR}
  ARCHIVE DESTINATION ${CGALPY_INSTALL_LIB_DIR})

install(FILES ${CGALPY_MODULE_OUTPUT} DESTINATION ${CGALPY_INSTALL_PYTHON_DIR})
foreach(MODULE IN LISTS CGALPY_MODULES)
  set (MODULE_OUTPUT_PATH ${CGALPY_DST_STUBS_DIR}/${name})
  install(DIRECTORY ${MODULE_OUTPUT_PATH} DESTINATION ${CGALPY_INSTALL_PYTHON_DIR})
endforeach()
