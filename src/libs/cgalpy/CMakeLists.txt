######## Packages ###############################################################
################################################################################

# Generate config.hpp
configure_file(config.hpp.in include/config.hpp)

# Add source files
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_module.cpp)

# A workaround an oversight in the conen CGAL setup files
# if (SKBUILD)
#    set(CGAL_USE_GMP TRUE)
# endif()

if (CGAL_USE_GMP)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_gmp.cpp)
endif ()

# set(CGALPY_DATA_DIR "${CGAL_DIR}/Data/data")
# add_compile_definitions(CGALPY_DATA_DIR="${CGALPY_DATA_DIR}")

#############
# Add modules
#############

set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/cgal_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/tools_bindings.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_bbox_2.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_bbox_3.cpp)
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_approximate_kernel.cpp)

# 2-3D Kernel
if (CGALPY_KERNEL_BINDINGS)
    list(APPEND CGALPY_MODULES Ker)
    set(CGALPY_Ker_FILTERS [=[{}]=])
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/kernel_bindings.cpp)
    if (CGALPY_KERNEL_INTERSECTION_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/intersections_2_bindings.cpp)
    endif ()
endif ()

# dD Kernel
if (CGALPY_KERNEL_D_BINDINGS)
    list(APPEND CGALPY_MODULES Kerd)
    set(CGALPY_Kerd_FILTERS [=[{}]=])
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/kernel_d_bindings.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_point_d.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_linear_algebra_cd.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_segment_d.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_vector_d.cpp)
endif ()

# 2D Alpha shape
if (CGALPY_ALPHA_SHAPE_2_BINDINGS)
    list(APPEND CGALPY_MODULES As2)
    set(CGALPY_As2_FILTERS [=[{}]=])
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/alpha_shape_2_bindings.cpp)
endif ()

# 3D Alpha shape
if (CGALPY_ALPHA_SHAPE_3_BINDINGS)
    list(APPEND CGALPY_MODULES As3)
    set(CGALPY_As3_FILTERS [=[{}]=])
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/alpha_shape_3_bindings.cpp)
endif ()

# 2D Arrangement
if (CGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS)
    list(APPEND CGALPY_MODULES Aos2)
    set(CGALPY_Aos2_FILTERS [=[{}]=])
    # Geometry Traits
    # if (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS})
    #     set(CGALPY_Aos2_FILTERS [=[{\"Aos2.Geometry_traits_2\":\"Arr_algebraic_segment_traits_2\",\"Arr_algebraic_segment_traits_2\":\"true\"}]=])
    #     set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_algebraic_segment_traits_2.cpp)
    # elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_BEZIER_GEOMETRY_TRAITS})
    #     set(CGALPY_Aos2_FILTERS [=[{\"Aos2.Geometry_traits_2\":\"Arr_Bezier_curve_traits_2\",\"Arr_Bezier_curve_traits_2\":\"true\"}]=])
    #     set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_bezier_curve_traits_2.cpp)
    # elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS})
    #     set(CGALPY_Aos2_FILTERS [=[{\"Aos2.Geometry_traits_2\":\"Arr_circle_segment_traits_2\",\"Arr_circle_segment_traits_2\":\"true\"}]=])
    #     set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_circle_segment_traits_2.cpp)
    # elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_CONIC_GEOMETRY_TRAITS})
    #     set(CGALPY_Aos2_FILTERS [=[{\"Aos2.Geometry_traits_2\":\"Arr_conic_traits_2\",\"Arr_conic_traits_2\":\"true\"}]=])
    #     set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_conic_traits_2.cpp)
    # elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_TRAITS})
    #     set(CGALPY_Aos2_FILTERS [=[{\"Aos2.Geometry_traits_2\":\"Arr_geodesic_arc_on_sphere_traits_2\",\"Arr_geodesic_arc_on_sphere_traits_2\":\"true\"}]=])
    #     set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_geodesic_arc_on_sphere_traits_2.cpp)
    # elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS})
    #     set(CGALPY_Aos2_FILTERS [=[{\"Aos2.Geometry_traits_2\":\"Arr_linear_traits_2\",\"Arr_linear_traits_2\":\"true\"}]=])
    #     set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_linear_traits_2.cpp)
    # elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS})
    #     set(CGALPY_Aos2_FILTERS [=[{\"Arr_non_caching_segment_basic_traits_2\":\"true\",\"AosBasicTraits_2.Point_2\":\"false\"}]=])
    #     set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_non_caching_segment_traits_2.cpp)
    #     set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_non_caching_segment_basic_traits_2.cpp)
    # elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_POLYLINE_OF_SEGMENTS_GEOMETRY_TRAITS})
    #     set(CGALPY_Aos2_FILTERS [=[{\"Aos2.Geometry_traits_2\":\"Arr_polyline_traits_2\",\"Arr_polyline_traits_2\":\"true\"}]=])
    #     set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_segment_traits_2.cpp)
    #     set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_polyline_traits_2.cpp)
    # elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS})
    #     set(CGALPY_Aos2_FILTERS [=[{\"Aos2.Geometry_traits_2\":\"Arr_segment_traits_2\",\"Arr_segment_traits_2\":\"true\",\"AosBasicTraits_2.Point_2\":\"false\",\"AosBasicTraits_2.Equal_2\":\"false\"}]=])
    #     set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_segment_traits_2.cpp)
    # elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_RATIONAL_FUNCTION_GEOMETRY_TRAITS})
    #     set(CGALPY_Aos2_FILTERS [=[{\"Aos2.Geometry_traits_2\":\"Arr_rational_function_traits_2\",\"Arr_rational_function_traits_2\":\"true\"}]=])
    #     set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_rational_function_traits_2.cpp)
    #     set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_kernel_algebraic.cpp)
    # endif ()

    # instead, set Aos2_filters just to a string eg. :
    # "Aos2.Arr_algebraic_segment_geometry_traits_2":
    if (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS})
        set(CGALPY_Aos2_FILTERS "Aos2.Arr_algebraic_segment_geometry_traits_2")
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_algebraic_segment_traits_2.cpp)
    elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_BEZIER_GEOMETRY_TRAITS})
        set(CGALPY_Aos2_FILTERS "Aos2.Arr_Bezier_curve_geometry_traits_2")
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_bezier_curve_traits_2.cpp)
    elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS})
        set(CGALPY_Aos2_FILTERS "Aos2.Arr_circle_segment_geometry_traits_2")
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_circle_segment_traits_2.cpp)
    elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_CONIC_GEOMETRY_TRAITS})
        set(CGALPY_Aos2_FILTERS "Aos2.Arr_conic_geometry_traits_2")
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_conic_traits_2.cpp)
    elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_TRAITS})
        set(CGALPY_Aos2_FILTERS "Aos2.Arr_geodesic_arc_on_sphere_geometry_traits_2")
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_geodesic_arc_on_sphere_traits_2.cpp)
    elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS})
        set(CGALPY_Aos2_FILTERS "Aos2.Arr_linear_geometry_traits_2")
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_linear_traits_2.cpp)
    elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS})
        set(CGALPY_Aos2_FILTERS "Aos2.Arr_non_caching_segment_geometry_traits_2")
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_non_caching_segment_traits_2.cpp)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_non_caching_segment_basic_traits_2.cpp)
    elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_POLYLINE_OF_SEGMENTS_GEOMETRY_TRAITS})
        set(CGALPY_Aos2_FILTERS "Aos2.Arr_polyline_of_segments_geometry_traits_2")
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_segment_traits_2.cpp)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_polyline_traits_2.cpp)
    elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS})
        set(CGALPY_Aos2_FILTERS "Aos2.Arr_segment_geometry_traits_2")
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_segment_traits_2.cpp)
    elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_RATIONAL_FUNCTION_GEOMETRY_TRAITS})
        set(CGALPY_Aos2_FILTERS "Aos2.Arr_rational_function_geometry_traits_2")
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_rational_function_traits_2.cpp)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_kernel_algebraic.cpp)
    endif ()


    if (CGALPY_AOS2_CONSOLIDATED_CURVE_DATA)
      if (NOT CGALPY_AOS2_CURVE_DATA)
        message(FATAL_ERROR "CGALPY_AOS2_CURVE_DATA not set!\nTurning off CGALPY_AOS2_CONSOLIDATED_CURVE_DATA")
        set (CGALPY_AOS2_CONSOLIDATED_CURVE_DATA OFF)
      endif()
    endif ()

    if (CGALPY_AOS2_WITH_HISTORY)
      if (NOT CGALPY_AOS2_CONSOLIDATED_CURVE_DATA)
        message(FATAL_ERROR "CGALPY_AOS2_CONSOLIDATED_CURVE_DATA not set!\nTurning off CGALPY_AOS2_WITH_HISTORY")
        set (CGALPY_AOS2_WITH_HISTORY OFF)
      endif()
    endif()

    if (CGALPY_AOS2_CONSOLIDATED_CURVE_DATA)
      set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_consolidated_curve_data_traits_2.cpp)
    endif ()

    if (CGALPY_AOS2_CURVE_DATA)
      set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_curve_data_traits_2.cpp)
    endif ()

    if (CGALPY_AOS2_COUNTING_TRAITS)
      set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_counting_traits_2.cpp)
    endif ()
    if (CGALPY_AOS2_TRACING_TRAITS)
      set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_arr_tracing_traits_2.cpp)
    endif ()

    # Cells
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_vertex_bindings.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_halfedge_bindings.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_face_bindings.cpp)

    # Point Location
    if (CGALPY_AOS2_POINT_LOCATION_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arr_point_location_bindings.cpp)
    endif ()

    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arrangement_on_surface_2_bindings.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/arrangement_2_io_bindings.cpp)
endif ()

# 2D Regularized Boolean operations
if (CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS)
  if (NOT CGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS)
    message(WARNING "CGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS not set!\nTurning off CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS")
    set (CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS OFF)
  endif ()

  list(APPEND CGALPY_MODULES Bso2)
  set(CGALPY_Bso2_FILTERS [=[{}]=])
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/boolean_set_operations_2_bindings.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/general_polygon_set_2_bindings.cpp)
  if (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_set_2_bindings.cpp)
  endif ()

  # Geometry Traits
  if (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_gps_segment_traits_2.cpp)
  elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_gps_segment_traits_2.cpp)
  elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_gps_traits_2.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_gps_circle_segment_traits_2.cpp)
  elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_gps_traits_2.cpp)
  elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_BEZIER_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_gps_traits_2.cpp)
  elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_CONIC_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_gps_traits_2.cpp)
  elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_gps_traits_2.cpp)
  elseif (${CGALPY_AOS2_GEOMETRY_TRAITS} EQUAL ${CGALPY_AOS2_RATIONAL_FUNCTION_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_gps_traits_2.cpp)
  endif ()
endif ()

# Basic viewer
if (CGALPY_BASIC_VIEWER_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/basic_viewer_bindings.cpp)
    list(APPEND CGALPY_MODULES Bvr)
    set(CGALPY_Bvr_FILTERS [=[{}]=])
endif ()
# Bounding volumes
if (CGALPY_BOUNDING_VOLUMES_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/bounding_volumes_bindings.cpp)
    list(APPEND CGALPY_MODULES Bv)
    set(CGALPY_Bv_FILTERS [=[{}]=])
endif ()
# 2D convex hull
if (CGALPY_CONVEX_HULL_2_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/convex_hull_2_bindings.cpp)
    list(APPEND CGALPY_MODULES Ch2)
    set(CGALPY_Ch2_FILTERS [=[{}]=])
endif ()
# 3D convex hull
if (CGALPY_CONVEX_HULL_3_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/convex_hull_3_bindings.cpp)
    list(APPEND CGALPY_MODULES Ch3)
    set(CGALPY_Ch3_FILTERS [=[{}]=])
endif ()
# 2D envelopes
if (CGALPY_ENVELOPE_2_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/envelope_2_bindings.cpp)
    list(APPEND CGALPY_MODULES Env2)
    set(CGALPY_Env2_FILTERS [=[{}]=])
endif ()
# 3D envelopes
if (CGALPY_ENVELOPE_3_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/envelope_3_bindings.cpp)
    if (${CGALPY_ENV3_GEOMETRY_TRAITS} EQUAL ${CGALPY_ENV3_PLANE_GEOMETRY_TRAITS})
      set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_env_plane_traits_3.cpp)
    elseif (${CGALPY_ENV3_GEOMETRY_TRAITS} EQUAL ${CGALPY_ENV3_SPHERE_GEOMETRY_TRAITS})
      set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_env_sphere_traits_3.cpp)
    elseif (${CGALPY_ENV3_GEOMETRY_TRAITS} EQUAL ${CGALPY_ENV3_TRIANGLE_GEOMETRY_TRAITS})
      set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_env_tri_traits_3.cpp)
    endif ()
    if (CGALPY_ENV3_SURFACE_DATA)
      set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_env_surface_data_traits_3.cpp)
    endif ()
    list(APPEND CGALPY_MODULES Env3)
    set(CGALPY_Env3_FILTERS [=[{}]=])
endif ()
# 2D Minkowski sum
if (CGALPY_MINKOWSKI_SUM_2_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/minkowski_sum_2_bindings.cpp)
    list(APPEND CGALPY_MODULES Ms2)
    set(CGALPY_Ms2_FILTERS [=[{}]=])
endif ()
# 3D Nef
if (CGALPY_NEF_3_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/nef_3_bindings.cpp)
    list(APPEND CGALPY_MODULES Nef3)
    set(CGALPY_Nef3_FILTERS [=[{}]=])
endif ()
# 3D Polyhedron
if (CGALPY_POLYHEDRON_3_BINDINGS)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polyhedron_3_bindings.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_polyhedron_builder.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_polyhedron_face.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_polyhedron_halfedge.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_polyhedron_halfedge_ds.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_polyhedron_incremental_builder_3.cpp)
  set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_polyhedron_vertex.cpp)
  if (${CGALPY_POL3_GEOMETRY_TRAITS} EQUAL ${CGALPY_POL3_WITH_NORMALS_GEOMETRY_TRAITS})
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_polyhedron_traits_with_normals_3.cpp)
  endif ()
  list(APPEND CGALPY_MODULES Pol3)
  set(CGALPY_Pol3_FILTERS [=[{}]=])
endif ()
# 2D Polygon
if (CGALPY_POLYGON_2_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_2_bindings.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_with_holes_2_bindings.cpp)
    list(APPEND CGALPY_MODULES Pol2)
    set(CGALPY_Pol2_FILTERS [=[{}]=])
endif ()
if (CGALPY_2D_RANGE_AND_NEIGHBOR_SEARCH_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/two_d_range_and_neighbor_search_bindings.cpp)
    set(CGALPY_Rns2_FILTERS [=[{}]=])
endif ()
if (CGALPY_KINETIC_SURFACE_RECONSTRUCTION_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/kinetic_surface_reconstruction_bindings.cpp)
    set(CGALPY_Ksr_BINDINGS_FILTERS [=[{}]=])
endif ()
if (CGALPY_3D_POINT_SET_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/three_d_point_set_bindings.cpp)
    set(CGALPY_Pset3_FILTERS [=[{}]=])
endif ()
# Polygon mesh processing
if (CGALPY_POLYGON_MESH_PROCESSING_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_mesh_processing_bindings.cpp)
    if (CGALPY_PMP_COMBINATORIAL_REPAIR_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_pmp_combinatorial_repair.cpp)
    endif ()
    if (CGALPY_PMP_CONNECTED_COMPONENTS_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_pmp_connected_components.cpp)
    endif ()
    if (CGALPY_PMP_COREFINEMENT_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_pmp_corefinement.cpp)
    endif ()
    if (CGALPY_PMP_CORRECTED_CURVATURE_COMPUTATION_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_pmp_corrected_curvature_computation.cpp)
    endif ()
    if (CGALPY_PMP_DISTANCE_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_pmp_distance.cpp)
    endif ()
    if (CGALPY_PMP_FEATURE_DETECTION_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_pmp_feature_detection.cpp)
    endif ()
    if (CGALPY_PMP_GEOMETRIC_MEASURE_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_pmp_geometric_measure.cpp)
    endif ()
    if (CGALPY_PMP_GEOMETRIC_REPAIR_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_pmp_geometric_repair.cpp)
    endif ()
    if (CGALPY_PMP_HOLE_FILLING_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_pmp_hole_filling.cpp)
    endif ()
    if (CGALPY_PMP_INTERSECTION_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_pmp_intersection.cpp)
    endif ()
    if (CGALPY_PMP_LOCATION_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_pmp_location.cpp)
    endif ()
    if (CGALPY_PMP_NORMAL_COMPUTATION_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_pmp_normal_computation.cpp)
    endif ()
    if (CGALPY_PMP_ORIENTATION_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_pmp_orientation.cpp)
    endif ()
    if (CGALPY_PMP_MESHING_BINDINGS)
        set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_pmp_meshing.cpp)
    endif ()
    list(APPEND CGALPY_MODULES Pmp)
    set(CGALPY_Pmp_FILTERS [=[{}]=])
endif ()
# Triangulated Surface Mesh Segmentation
if (CGALPY_TRIANGULATED_SURFACE_MESH_SEGMENTATION_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/triangulated_surface_mesh_segmentation_bindings.cpp)
endif ()
# Triangulated Surface Mesh Simplification
if (CGALPY_TRIANGULATED_SURFACE_MESH_SIMPLIFICATION_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/triangulated_surface_mesh_simplification_bindings.cpp)
    list(APPEND CGALPY_MODULES Sms)
    set(CGALPY_Sms_FILTERS [=[{}]=])
endif ()
# Triangulated Surface Mesh Skeletonization
if (CGALPY_TRIANGULATED_SURFACE_MESH_SKELETONIZATION_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/triangulated_surface_mesh_skeletonization_bindings.cpp)
    list(APPEND CGALPY_MODULES Smsk)
    set(CGALPY_Smsk_FILTERS [=[{}]=])
endif ()
# Shape Detection
if (CGALPY_SHAPE_DETECTION_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_efficient_ransac.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_region_growing.cpp)
    list(APPEND CGALPY_MODULES Sd)
    set(CGALPY_Sd_FILTERS [=[{}]=])
endif ()
# Point Set Processing
if (CGALPY_POINT_SET_PROCESSING_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/point_set_processing_bindings.cpp)
endif ()
# 2D polygon partitioning
if (CGALPY_POLYGON_PARTITIONING_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/polygon_partitioning_bindings.cpp)
    list(APPEND CGALPY_MODULES Pp2)
    set(CGALPY_Pp2_FILTERS [=[{}]=])
endif ()
# Spatial searching
if (CGALPY_SPATIAL_SEARCHING_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/spatial_searching_bindings.cpp)
    list(APPEND CGALPY_MODULES Ss)
    set(CGALPY_Ss_FILTERS [=[{}]=])
endif ()
# Surface mesh
if (CGALPY_SURFACE_MESH_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/surface_mesh_bindings.cpp)
    list(APPEND CGALPY_MODULES Sm)
    set(CGALPY_Sm_FILTERS [=[{}]=])
endif ()
# 2D Surface Sweep
if (CGALPY_SURFACE_SWEEP_2_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/surface_sweep_2_bindings.cpp)
    list(APPEND CGALPY_MODULES Ss2)
    set(CGALPY_Ss2_FILTERS [=[{}]=])
endif ()
# 2D triangulation
if (CGALPY_TRIANGULATION_2_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/triangulation_2_bindings.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/tri2/export_tri2_face.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/tri2/export_tri2_plain.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/tri2/export_tri2_vertex.cpp)
    if (${CGALPY_TRI2} EQUAL ${CGALPY_TRI2_REGULAR})
      set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/tri2/export_tri2_regular.cpp)
    elseif (${CGALPY_TRI2} EQUAL ${CGALPY_TRI2_CONSTRAINED})
      set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/tri2/export_tri2_constrained.cpp)
    elseif (${CGALPY_TRI2} EQUAL ${CGALPY_TRI2_DELAUNAY})
      set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/tri2/export_tri2_delaunay.cpp)
    elseif (${CGALPY_TRI2} EQUAL ${CGALPY_TRI2_CONSTRAINED_DELAUNAY})
      set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/tri2/export_tri2_constrained_delaunay.cpp)
    endif ()
    list(APPEND CGALPY_MODULES Tri2)
    set(CGALPY_Tri2_FILTERS [=[{}]=])
endif ()
# 3D triangulation
if (CGALPY_TRIANGULATION_3_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/triangulation_3_bindings.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/tri3/export_tri3_vertex.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/tri3/export_tri3_cell.cpp)
    if (${CGALPY_TRI3} EQUAL ${CGALPY_TRI3_REGULAR})
      set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/tri3/export_tri3_regular.cpp)
    elseif (${CGALPY_TRI3} EQUAL ${CGALPY_TRI3_DELAUNAY})
      set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/tri3/export_tri3_delaunay.cpp)
    endif ()
    list(APPEND CGALPY_MODULES Tri3)
    set(CGALPY_Tri3_FILTERS [=[{}]=])
endif ()
# dD triangulation
if (CGALPY_TRIANGULATION_D_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/triangulation_d_bindings.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_trid_full_cell.cpp)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/export_trid_vertex.cpp)
    list(APPEND CGALPY_MODULES Trid)
    set(CGALPY_Trid_FILTERS [=[{}]=])
endif ()
# 2D Visibility
if (CGALPY_VISIBILITY_2_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/visibility_2_bindings.cpp)
    list(APPEND CGALPY_MODULES Vis2)
    set(CGALPY_Vis2_FILTERS [=[{}]=])
endif ()
# Geometric object generators
if (CGALPY_GEOMETRIC_OBJECT_GENERATORS_BINDINGS)
    set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/geometric_object_generators_bindings.cpp)
    list(APPEND CGALPY_MODULES Gog)
    set(CGALPY_Gog_FILTERS [=[{}]=])
endif ()

# Add all include directories
include_directories(include)

# Internal dependencies
set(CGALPY_SRC_FILES ${CGALPY_SRC_FILES} lib/parse_named_parameters.cpp)

# Add dependencies
link_directories(${Boost_LIBRARY_DIRS})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/wd4251)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fvisibility=hidden)
endif ()
if (NOT DEFINED CGALPY_LIB_NAME)
    set(CGALPY_LIB_NAME "CGALPY")
endif ()
if (NOT CGALPY_FIXED_LIBRARY_NAME)
    if (CGALPY_KERNEL_BINDINGS)
        get_kernel_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # dD Kernel
    if (CGALPY_KERNEL_D_BINDINGS)
        get_kernel_d_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 2D Alpha shape
    if (CGALPY_ALPHA_SHAPE_2_BINDINGS)
        get_alpha_shape_2_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 3D Alpha shape
    if (CGALPY_ALPHA_SHAPE_3_BINDINGS)
        get_alpha_shape_3_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 2D Arrangement
    if (CGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS)
        get_arrangement_on_surface_2_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 2D Regularized Boolean operations
    if (CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS)
        get_boolean_set_operations_2_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # Bounding volumes
    if (CGALPY_BOUNDING_VOLUMES_BINDINGS)
        get_bounding_volumes_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 2D convex hull
    if (CGALPY_CONVEX_HULL_2_BINDINGS)
        get_convex_hull_2_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 3D convex hull
    if (CGALPY_CONVEX_HULL_3_BINDINGS)
        get_convex_hull_3_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 2D envelopes
    if (CGALPY_ENVELOPE_2_BINDINGS)
        get_envelope_2_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 3D envelopes
    if (CGALPY_ENVELOPE_3_BINDINGS)
        get_envelope_3_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 2D Minkowski sum
    if (CGALPY_MINKOWSKI_SUM_2_BINDINGS)
        get_minkowski_sum_2_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 3D Nef
    if (CGALPY_NEF_3_BINDINGS)
        get_nef_3_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # Point Set Processing
    if (CGALPY_POINT_SET_PROCESSING_BINDINGS)
      get_point_set_processing_lib_name(ret)
      set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 3D Polyhedron
    if (CGALPY_POLYHEDRON_3_BINDINGS)
        get_polyhedron_3_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 2D Polygon
    if (CGALPY_POLYGON_2_BINDINGS)
        get_polygon_2_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 2D polygon partitioning
    if (CGALPY_POLYGON_PARTITIONING_BINDINGS)
        get_polygon_partitioning_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # Spatial searching
    if (CGALPY_SPATIAL_SEARCHING_BINDINGS)
        get_spatial_searching_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # Surface mesh
    if (CGALPY_SURFACE_MESH_BINDINGS)
        get_surface_mesh_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 2D Surface sweep
    if (CGALPY_SURFACE_SWEEP_2_BINDINGS)
        get_surface_sweep_2_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 2D triangulation
    if (CGALPY_TRIANGULATION_2_BINDINGS)
        get_triangulation_2_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 3D triangulation
    if (CGALPY_TRIANGULATION_3_BINDINGS)
        get_triangulation_3_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # dD triangulation
    if (CGALPY_TRIANGULATION_D_BINDINGS)
        get_triangulation_d_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # Kinetic Surface Reconstruction
    if (CGALPY_KINETIC_SURFACE_RECONSTRUCTION_BINDINGS)
        kinetic_surface_reconstruction_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # Polygon mesh processing
    if (CGALPY_POLYGON_MESH_PROCESSING_BINDINGS)
        get_polygon_mesh_processing_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # Triangulated Surface Mesh Simplification
  if (CGALPY_TRIANGULATED_SURFACE_MESH_SIMPLIFICATION_BINDINGS)
        get_triangulated_surface_mesh_simplification_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # 2D Visibility
    if (CGALPY_VISIBILITY_2_BINDINGS)
        get_visibility_2_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # Basic viewer
    if (CGALPY_BASIC_VIEWER_BINDINGS)
        get_basic_viewer_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
    # Geometric object generators
    if (CGALPY_GEOMETRIC_OBJECT_GENERATORS_BINDINGS)
        get_geometric_object_generators_lib_name(ret)
        set(CGALPY_LIB_NAME "${CGALPY_LIB_NAME}_${ret}")
    endif ()
endif ()

set(CGALPY_LIB_DIR "${CMAKE_BINARY_DIR}/src/libs/cgalpy")

function(add_module name)
  set(MODULE_OUTPUT_PATH ${CGALPY_DST_STUBS_DIR}/${name})
  set(MODULE_OUTPUT ${MODULE_OUTPUT_PATH}/__init__.pyi)
  set(MODULE_INPUT ${CGALPY_MODULES_DIR}/${name}.json)
  set(MODULE_ARGS
    --filters ${CGALPY_${name}_FILTERS}
    --name ${name}
    --input-path ${CGALPY_MODULES_DIR}
    --output-path ${CGALPY_DST_STUBS_DIR})

  # There is no need to generate the directory, as the Python script is
  # responsible for this task.
  file(MAKE_DIRECTORY ${MODULE_OUTPUT_PATH})

  # add_custom_command(
  #   OUTPUT ${MODULE_OUTPUT}
  #   COMMAND ${CGALPY_STUBS_GENERATOR_CMD}
  #   ARGS ${MODULE_ARGS}
  #   DEPENDS ${CGALPY_STUBS_GENERATOR} ${MODULE_INPUT})
  # add_custom_target(CGALPY_${name}_STUBS ALL DEPENDS ${MODULE_OUTPUT})

  nanobind_add_stub(
    CGALPY_${name}_STUBS
    MODULE cgalpy.${CGALPY_LIB_NAME}.${name}
    OUTPUT ${name}/__init__.pyi
    DEPENDS CGALPY CGALPY_INIT
    PYTHON_PATH ${CMAKE_BINARY_DIR}/src/libs
  )

  set(STUBS_DEPENDS ${CGALPY_STUBS_DEPENDS})
  list(APPEND STUBS_DEPENDS CGALPY_${name}_STUBS)
  set(CGALPY_STUBS_DEPENDS "${STUBS_DEPENDS}" PARENT_SCOPE)
endfunction()

add_compile_definitions(CGALPY_MODULE_NAME=${CGALPY_LIB_NAME})
# Add the library target
nanobind_add_module(CGALPY STABLE_ABI NB_STATIC ${CGALPY_SRC_FILES})
set(CGALPY_BUILD_DEPENDS CGALPY)

if (NOT SKBUILD)
    set(CGALPY_PACKAGE_NAME ${CGALPY_LIB_NAME})

    target_link_libraries(CGALPY PRIVATE
      CGAL::CGAL
      CGAL::Data
      ${PYTHON_LIBRARIES})

    if (CGAL_Qt6_FOUND)
      target_link_libraries(CGALPY PRIVATE
        CGAL::CGAL_Qt6 Qt6::Core Qt6::Gui Qt6::Widgets)
    endif()

    # EFEF: Only certain packages need Eigen
    if (TARGET CGAL::Eigen3_support)
      target_link_libraries(CGALPY PRIVATE CGAL::Eigen3_support)
    endif ()

    # EFEF: Only certain packages need METIS
    find_package(METIS QUIET)
    include(CGAL_METIS_support)
    if (TARGET CGAL::METIS_support)
      target_link_libraries(CGALPY PRIVATE CGAL::METIS_support)
    endif()

    if (NOT APPLE)
      set_target_properties(CGALPY PROPERTIES
        VERSION "${CMAKE_PROJECT_VERSION}"
        SOVERSION "${CMAKE_PROJECT_VERSION_MAJOR}")
    endif ()

    set_target_properties(CGALPY PROPERTIES OUTPUT_NAME ${CGALPY_LIB_NAME})
    # Use a generator expression to get rid of per-configuration subdirectory appended by Multi-configuration generators (e.g Visual Studio)
    # https://cmake.org/cmake/help/latest/prop_tgt/LIBRARY_OUTPUT_DIRECTORY.html
    set_target_properties(CGALPY PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CGALPY_LIB_DIR}/$<0:>)
    reportWithEmphasis(${CGALPY_LIB_NAME} ${CGALPY_LIB_DIR} ${CGALPY_INSTALL_PYTHON_DIR})

    # Don't prepend wrapper library name with lib
    set_target_properties(CGALPY PROPERTIES PREFIX "")

    set(CGALPY_LIBRARY_SUFFIX ${NB_SUFFIX})
    # if(WIN32)
    #   set (CGALPY_LIBRARY_SUFFIX ".pyd")
    #   set_target_properties(CGALPY PROPERTIES SUFFIX ".pyd")
    # elseif(APPLE)
    #   set (CMAKE_SHARED_LIBRARY_SUFFIX ".so")
    #   set_target_properties(CGALPY PROPERTIES BUNDLE ON)
    # endif()

    #
    # Add __init__.py
    if (WIN32)
      # 1. The policy of loading shared libraries (DLL files) by Python on
      # Windows changed in Python 3.8 to enhance security and avoid potential
      # issues with DLL hijacking. Specifically, the directory of the script
      # being executed and the directories specified by the PATH environment
      # variable are no longer added to the DLL search path. Instead, Python
      # 3.8 introduced the os.add_dll_directory() method, which enables the
      # explicit addition of directories to the DLL search path at runtime.
      # We exploit this addition, and add several calls to the
      # os.add_dll_directory() method in the __init__.py file to specify all
      # needed directories.
      #
      # The stubs are generated by nanobinds. In particular, we use the
      # the cmake method nanobind_add_stub() once to to generate a stub file
      # for the main module and as many as needed to generate a stub file
      # for each sub-module. The generation must import the bunding library
      # and thus also depends on the generation of the __init__.py file.

      # Configure the __init__.py file to automatically load the dependant
      # shared ibraries, when the binding library is loaded (upon "import").

      # only boost headers are used; no need for libraries
      #if (NOT Boost_LIBRARY_DIRS)
      #  message(FATAL_ERROR "Boost libraries not found")
      #endif ()
      if (NOT GMP_LIBRARY_DIR)
        message(FATAL_ERROR "Gmp library not found")
      endif ()
      if (NOT MPFR_LIBRARY_DIR)
        message(FATAL_ERROR "Mpfr library not found")
      endif ()
      if (CGAL_Qt6_FOUND)
        set(CGALPY_SRC_INIT "${CMAKE_CURRENT_LIST_DIR}/Windows/Qt/__init__.py")
        # The directory that contains the Qt6 shared libraries seems not to be
	# exported by the find_package(Qt6..) call; thus, we set it ourselves,
	# but let the users also set explicitly it.
        if (NOT Qt6_LIBRARY_DIR)
          set(Qt6_LIBRARY_DIR "${QT6_INSTALL_PREFIX}/bin")
        endif()
        if (NOT Qt6_LIBRARY_DIR)
          message(FATAL_ERROR "Qt6 library not found")
        endif ()
      else ()
        set(CGALPY_SRC_INIT "${CMAKE_CURRENT_LIST_DIR}/Windows/__init__.py")
      endif ()
    else ()
      set(CGALPY_SRC_INIT "${CMAKE_CURRENT_LIST_DIR}/__init__.py")
    endif ()
    set(CGALPY_OUT_INIT "${CGALPY_LIB_DIR}/__init__.py")
    add_custom_command(
        OUTPUT ${CGALPY_OUT_INIT}
        COMMAND "${CMAKE_COMMAND}"
        "-DFILE_IN=${CGALPY_SRC_INIT}"
        "-DFILE_OUT=${CGALPY_OUT_INIT}"
        "-DBoost_LIBRARY_DIRS=${Boost_LIBRARY_DIRS}"
        "-DGMP_LIBRARY_DIR=${GMP_LIBRARY_DIR}"
        "-DMPFR_LIBRARY_DIR=${MPFR_LIBRARY_DIR}"
        "-DQt6_LIBRARY_DIR=${Qt6_LIBRARY_DIR}"
        "-DCGALPY_LIB_NAME=${CGALPY_LIB_NAME}"
        -P "${CGALPY_MODULE_DIR}/BuildTimeFile.cmake"
        DEPENDS ${CGALPY_SRC_INIT})
    add_custom_target(CGALPY_INIT ALL DEPENDS ${CGALPY_OUT_INIT})
    list(APPEND CGALPY_BUILD_DEPENDS CGALPY_INIT)

else ()
    set(CGALPY_SRC_INIT "${CMAKE_CURRENT_LIST_DIR}/__init__.py")
    set(CGALPY_OUT_INIT "${CGALPY_LIB_DIR}/__init__.py")
    add_custom_command(
        OUTPUT ${CGALPY_OUT_INIT}
        COMMAND "${CMAKE_COMMAND}"
        "-DFILE_IN=${CGALPY_SRC_INIT}"
        "-DFILE_OUT=${CGALPY_OUT_INIT}"
        "-DCGALPY_LIB_NAME=${CGALPY_LIB_NAME}"
        -P "${CGALPY_MODULE_DIR}/BuildTimeFile.cmake"
        DEPENDS ${CGALPY_SRC_INIT})
    add_custom_target(CGALPY_INIT ALL DEPENDS ${CGALPY_OUT_INIT})
    list(APPEND CGALPY_BUILD_DEPENDS CGALPY_INIT)

    target_link_libraries(CGALPY PRIVATE
        CGAL::CGAL
        ${PYTHON_LIBRARIES})

    # Currently, the following has no effect as all comoponents of CGAL (e.g.,
    # CGAL::CGAL_Qt6 CGAL::Data are not recognized by Conan.
    # \todo Fix the CGAL installation process of Conan
    if (CGAL_Qt6_FOUND)
        target_link_libraries(CGALPY PRIVATE
        CGAL::CGAL_Qt6 Qt6::Core Qt6::Gui Qt6::Widgets)
    endif()

    set_target_properties(CGALPY PROPERTIES OUTPUT_NAME ${CGALPY_LIB_NAME})
    # Use a generator expression to get rid of per-configuration subdirectory appended by Multi-configuration generators (e.g Visual Studio)
    # https://cmake.org/cmake/help/latest/prop_tgt/LIBRARY_OUTPUT_DIRECTORY.html
    set_target_properties(CGALPY PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CGALPY_LIB_DIR}/$<0:>)

    if (NOT APPLE)
        set_target_properties(CGALPY PROPERTIES
                VERSION "${CMAKE_PROJECT_VERSION}"
                SOVERSION "${CMAKE_PROJECT_VERSION_MAJOR}")
    endif ()
endif()


################################################################################
# Stubs (Type Annotation)
################################################################################
set(CGALPY_SRC_STUBS_DIR "${CMAKE_CURRENT_LIST_DIR}/stubs")
set(CGALPY_MODULES_DIR "${CGALPY_SRC_STUBS_DIR}/modules")
set(CGALPY_STUBS_GENERATOR "${CGALPY_SRC_STUBS_DIR}/generate.py")
if (WIN32)
    set(CGALPY_STUBS_GENERATOR_CMD ${Python_EXECUTABLE} ${CGALPY_STUBS_GENERATOR})
else ()
    set(CGALPY_STUBS_GENERATOR_CMD ${CGALPY_STUBS_GENERATOR})
endif ()

set(CGALPY_DST_STUBS_DIR "${CMAKE_BINARY_DIR}/src/libs/cgalpy")

set(CGALPY_MODULE_OUTPUT ${CGALPY_DST_STUBS_DIR}/${CGALPY_LIB_NAME}.pyi)
set(CGALPY_MODULE_INPUT ${CGALPY_MODULES_DIR}/CGALPY.json)
string(REPLACE ";" " " STR_MODULES "${CGALPY_MODULES}")
set(CGALPY_MODULE_ARGS
        --output-file ${CGALPY_LIB_NAME}.pyi
        --input-path ${CGALPY_MODULES_DIR}
        --output-path ${CGALPY_DST_STUBS_DIR}
        --imports "${STR_MODULES}"
        CGALPY.json)

# add_custom_command(
#         OUTPUT ${CGALPY_MODULE_OUTPUT}
#         COMMAND ${CGALPY_STUBS_GENERATOR_CMD}
#         ARGS ${CGALPY_MODULE_ARGS}
#         DEPENDS ${CGALPY_STUBS_GENERATOR} ${CGALPY_MODULE_INPUT}
#         VERBATIM)
# add_custom_target(CGALPY_STUBS ALL DEPENDS ${CGALPY_MODULE_OUTPUT})

nanobind_add_stub(
  CGALPY_STUBS
  MODULE cgalpy.${CGALPY_LIB_NAME}
  OUTPUT ${CGALPY_LIB_NAME}.pyi
  DEPENDS CGALPY CGALPY_INIT
  PYTHON_PATH ${CMAKE_BINARY_DIR}/src/libs
  MARKER_FILE py.typed
  )

list(APPEND CGALPY_STUBS_DEPENDS CGALPY_STUBS)

foreach (MODULE IN LISTS CGALPY_MODULES)
  add_module(${MODULE})
  endforeach ()

add_custom_target(STUBS ALL DEPENDS ${CGALPY_STUBS_DEPENDS})
list(APPEND CGALPY_BUILD_DEPENDS ${CGALPY_STUBS_DEPENDS})

if (NOT SKBUILD)

    ################################################################################
    # Post build
    ################################################################################

    # Setup generation
    set(CGALPY_SRC_SETUP "${CMAKE_CURRENT_LIST_DIR}/setup.py")
    set(CGALPY_OUT_SETUP "${CGALPY_LIB_DIR}/setup.py")
    set(CGALPY_TARGET_LINKER_FILE ${CGALPY_LIB_NAME}${CGALPY_LIBRARY_SUFFIX})
    set(CGALPY_SETUP_DATA "'${CGALPY_TARGET_LINKER_FILE}'")
    list(APPEND CGALPY_SETUP_DATA "'__init__.py'")
    list(APPEND CGALPY_SETUP_DATA "'${CGALPY_LIB_NAME}.pyi'")
    foreach (MODULE IN LISTS CGALPY_MODULES)
      list(APPEND CGALPY_SETUP_DATA "'${MODULE}/__init__.pyi'")
    endforeach ()
    list(APPEND CGALPY_SETUP_DATA "'py.typed'")
    string(REPLACE ";" "," CGALPY_SETUP_DATA "${CGALPY_SETUP_DATA}")

    add_custom_command(
            OUTPUT ${CGALPY_OUT_SETUP}
            COMMAND "${CMAKE_COMMAND}"
            "-DFILE_IN=${CGALPY_SRC_SETUP}"
            "-DFILE_OUT=${CGALPY_OUT_SETUP}"
            "-DTARGET_NAME=${CGALPY_LIB_NAME}"
            "-DTARGET_VERSION=${PROJECT_VERSION}"
            "-DTARGET_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}"
            "-DTARGET_VERSION_MINOR=${PROJECT_VERSION_MINOR}"
            "-DTARGET_VERSION_PATCH=${PROJECT_VERSION_PATCH}"
            "-DTARGET_DATA='${CGALPY_SETUP_DATA}'"
            "-DTARGET_PACKAGE_NAME='${CGALPY_PACKAGE_NAME}'"
            "-DTARGET_LINKER_FILE=${CGALPY_TARGET_LINKER_FILE}"
            "-DTARGET_MODULES=${CGALPY_LIB_NAME}"
            "-DTARGET_AUTHOR=${CGALPY_PROJECT_AUTHORS}"
            "-DTARGET_EMAIL=${CGALPY_PROJECT_EMAILS}"
            "-DTARGET_URL=${CGALPY_PROJECT_URL}"
            -P "${CGALPY_MODULE_DIR}/BuildTimeFile.cmake"
            VERBATIM
            DEPENDS ${CGALPY_SRC_SETUP})
    add_custom_target(CGALPY_SETUP ALL DEPENDS ${CGALPY_OUT_SETUP})
    list(APPEND CGALPY_BUILD_DEPENDS CGALPY_SETUP)

    # Add README.rst
    set(CGALPY_SRC_README "${CMAKE_CURRENT_LIST_DIR}/README.rst")
    set(CGALPY_OUT_README "${CGALPY_LIB_DIR}/README.rst")
    add_custom_command(
            OUTPUT ${CGALPY_OUT_README}
            COMMAND "${CMAKE_COMMAND}"
            "-DFILE_IN=${CGALPY_SRC_README}"
            "-DFILE_OUT=${CGALPY_OUT_README}"
            "-DTARGET_NAME=${CGALPY_LIB_NAME}"
            -P "${CGALPY_MODULE_DIR}/BuildTimeFile.cmake"
            DEPENDS ${CGALPY_SRC_README})

    add_custom_target(CGALPY_README ALL DEPENDS ${CGALPY_OUT_README})
    list(APPEND CGALPY_BUILD_DEPENDS CGALPY_README)

    # Setup application
    set(CGALPY_BUILD_OUTPUT_DIR "${CGALPY_LIB_DIR}/dist")
    string(TOLOWER "${CGALPY_PACKAGE_NAME}-${cgalpy_VERSION}" CGALPY_TAR_BASENAME)
    set (CGALPY_TAR ${CGALPY_BUILD_OUTPUT_DIR}/${CGALPY_TAR_BASENAME}.tar.gz)

    set (CGALPY_PYTHON_TAG "cp${CGALPY_PY_MAJOR_MINOR_VERSION}")
    # set (CGALPY_PYTHON_TAG "py3")

    set (CGALPY_ABI_TAG "cp${CGALPY_PY_MAJOR_MINOR_VERSION}")
    # set (CGALPY_ABI_TAG "none")
    execute_process(COMMAND ${Python_EXECUTABLE} -c "import sysconfig; print(sysconfig.get_platform().replace('-','_').replace('.','_'),end='')" OUTPUT_VARIABLE CGALPY_PLATFORM_TAG)
    # set (CGALPY_PLATFORM_TAG "any")

    set (CGALPY_WHL ${CGALPY_BUILD_OUTPUT_DIR}/${CGALPY_PACKAGE_NAME}-${cgalpy_VERSION}-${CGALPY_PYTHON_TAG}-${CGALPY_ABI_TAG}-${CGALPY_PLATFORM_TAG}.whl)
    string(TOLOWER ${CGALPY_WHL} CGALPY_WHL_LOWER)
    set(CGALPY_BUILD_OUTPUT ${CGALPY_TAR} ${CGALPY_WHL_LOWER})
    set(CGALPY_BUILD_CMD ${Python_EXECUTABLE})
    set(CGALPY_BUILD_ARGS -m build --config-setting ${CGALPY_PYTHOM_BUILD_CONFIG_SETTING} ${CGALPY_LIB_DIR})
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
    foreach (MODULE IN LISTS CGALPY_MODULES)
        set(MODULE_OUTPUT_PATH ${CGALPY_DST_STUBS_DIR}/${name})
        install(DIRECTORY ${MODULE_OUTPUT_PATH} DESTINATION ${CGALPY_INSTALL_PYTHON_DIR})
    endforeach ()

else ()

    # Install the generated stubs
    install(FILES ${CGALPY_DST_STUBS_DIR}/${CGALPY_LIB_NAME}.pyi DESTINATION ${CGALPY_LIB_NAME})
    install(FILES ${CGALPY_DST_STUBS_DIR}/py.typed DESTINATION ${CGALPY_LIB_NAME})

    foreach (MODULE IN LISTS CGALPY_MODULES)
        install(DIRECTORY ${CGALPY_DST_STUBS_DIR}/${MODULE} DESTINATION ${CGALPY_LIB_NAME})
    endforeach ()

    install(TARGETS CGALPY DESTINATION ${CGALPY_LIB_NAME})
    install(FILES ${CGALPY_OUT_INIT} DESTINATION ${CGALPY_LIB_NAME})
endif ()

################################################################################
# Documentation
################################################################################

if (CGALPY_BUILD_DEV_DOC)
    # Doxygen
    find_package(Doxygen)

    set(CGALPY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

    if (DOXYGEN_FOUND)
        add_custom_command(
                OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                COMMAND "${CMAKE_COMMAND}"
                "-DFILE_IN=${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
                "-DFILE_OUT=${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
                "-DCGALPY_SOURCE_DIR=${CGALPY_SOURCE_DIR}"
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
        if (NOT TARGET doc)
            add_custom_target(doc)
        endif ()
        add_dependencies(doc CGALPY_CPP_DOC)
    endif (DOXYGEN_FOUND)
endif ()

if (CGALPY_BUILD_DOC)
    # Sphinx
    find_package(Sphinx)

    if (Sphinx_FOUND)
        add_sphinx_document(${CGALPY_LIB_NAME}
                MODULES "${CGALPY_MODULES}"
                AUTHORS "${CGALPY_PROJECT_AUTHORS}"
                CONF_FILE "${CMAKE_CURRENT_LIST_DIR}/doc/source/conf.py"
                INDEX_FILE "${CMAKE_CURRENT_LIST_DIR}/doc/source/index.rst")
    else ()
        message(WARNING "No Sphinx found. Documentation target not available.")
    endif ()
endif ()
