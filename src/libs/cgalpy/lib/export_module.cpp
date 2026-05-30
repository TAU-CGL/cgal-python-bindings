// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <CGAL/config.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include "cgalpy/arrangement_on_surface_2_values.hpp"
#include "cgalpy/kernel_types.hpp"
#include "cgalpy/triangulation_3_values.hpp"
#include "cgalpy/triangulation_2_values.hpp"
#include "cgalpy/config.hpp"

namespace py = nanobind;

void export_2d_range_and_neighbor_search(py::module_&);
void export_3d_point_set(py::module_&);
void export_alpha_shape_2(py::module_&);
void export_alpha_shape_3(py::module_&);
void export_approximate_kernel(py::module_& m);
void export_arrangement_on_surface_2(py::module_&);
void export_basic_viewer(py::module_& m);
void export_bgl(py::module_&);
void export_boolean_set_operations_2(py::module_&);
void export_bounding_volumes(py::module_&);
void export_cgal(py::module_& m);
void export_convex_hull_2(py::module_&);
void export_convex_hull_3(py::module_&);
void export_efficient_ransac(py::module_&);
void export_envelope_2(py::module_&);
void export_envelope_3(py::module_&);
void export_geometric_object_generators(py::module_& m);
void export_general_polygon_set_2(py::module_&);
void export_intersections(py::module_&);
void export_kernel_module(py::module_&);
void export_kernel_d(py::module_&);
void export_kinetic_surface_reconstruction(py::module_&);
void export_minkowski_sum_2(py::module_&);
void export_nef_3(py::module_&);
void export_point_location(py::module_&);
void export_point_set_processing(py::module_&);

// Polygon Mesh Processing
void export_polygon_mesh_processing(py::module_&);
void export_pmp_combinatorial_repair(py::module_&);
void export_pmp_connected_components(py::module_&);
void export_pmp_corefinement(py::module_&);
void export_pmp_corrected_curvature_computation(py::module_&);
void export_pmp_distance(py::module_&);
void export_pmp_feature_detection(py::module_&);
void export_pmp_geometric_measure(py::module_&);
void export_pmp_geometric_repair(py::module_&);
void export_pmp_hole_filling(py::module_&);
void export_pmp_intersection(py::module_&);
void export_pmp_location(py::module_&);
void export_pmp_normal_computation(py::module_&);
void export_pmp_orientation(py::module_&);
void export_pmp_meshing(py::module_&);
void export_pmp_io(py::module_&);

// Polyhedron_3
void export_polyhedron_3(py::module_&);
void export_pol3_bgl(py::module_& m);

void export_polygon_2(py::module_&);
void export_polygon_partition_2(py::module_&);
void export_polygon_set_2(py::module_&);
void export_polygon_with_holes_2(py::module_&);
void export_region_growing(py::module_&);
void export_spatial_searching(py::module_&);
void export_spatial_sorting(py::module_&);
void export_straight_skeleton_2(py::module_&);
void export_surface_mesh(py::module_&);
void export_surface_sweep_2(py::module_&);
void export_tools(py::module_& m);
void export_triangulated_surface_mesh_segmentation(py::module_&);
void export_triangulated_surface_mesh_simplification(py::module_&);
void export_triangulated_surface_mesh_skeletonization(py::module_&);

void export_triangulation_2(py::module_&);
void export_tri2_plain(py::module_&);
void export_tri2_regular(py::module_&);
void export_tri2_constrained(py::module_&);
void export_tri2_constrained_delaunay(py::module_&);
void export_tri2_delaunay(py::module_&);

void export_triangulation_3(py::module_&);
void export_tri3_plain(py::module_& m);
void export_tri3_regular(py::module_& m);
void export_tri3_delaunay(py::module_& m);

void export_triangulation_d(py::module_&);

void export_visibility_2(py::module_&);

#define MY_PYTHON_MODULE(name, m) NB_MODULE(name, m)

MY_PYTHON_MODULE(CGALPY_MODULE_NAME, m) {
  // http://isolation-nation.blogspot.com/2008/09/packages-in-python-extension-modules.html
  m.attr("__path__") = XSTR(CGALPY_MODULE_NAME);

  export_approximate_kernel(m);
  export_cgal(m);
  export_tools(m);

#if defined(CGALPY_KERNEL_BINDINGS)
  auto ker_m = m.def_submodule("Ker", kernel_doc());
  export_kernel_module(ker_m);
#if defined(CGALPY_KERNEL_INTERSECTION_BINDINGS)
  export_intersections(ker_m);
#endif
#endif

#if defined(CGALPY_2D_RANGE_AND_NEIGHBOR_SEARCH_BINDINGS)
  export_2d_range_and_neighbor_search(m); // currently under CGALPY
#endif

#ifdef CGALPY_3D_POINT_SET_BINDINGS
  export_3d_point_set(m); // currently under CGALPY
#endif

#if defined(CGALPY_KINETIC_SURFACE_RECONSTRUCTION_BINDINGS)
    export_kinetic_surface_reconstruction(m); // currently under CGALPY
#endif

#if defined(CGALPY_KERNEL_D_BINDINGS)
  auto kerd_m = m.def_submodule("Kerd");
  export_kernel_d(kerd_m);
#endif

#if defined(CGALPY_POLYGON_2_BINDINGS)
  auto pol2_m = m.def_submodule("Pol2");
  export_polygon_2(pol2_m);
  export_polygon_with_holes_2(pol2_m);
#endif

#if defined(CGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS)
  // Aos2 must succeed Pol2
  auto aos2_m = m.def_submodule("Aos2");
  export_arrangement_on_surface_2(aos2_m);
#ifdef CGALPY_AOS2_POINT_LOCATION_BINDINGS
  export_point_location(aos2_m);
#endif
#endif

#if defined(CGALPY_CONVEX_HULL_2_BINDINGS)
  auto ch2_m = m.def_submodule("Ch2");
  export_convex_hull_2(ch2_m);
#endif

#ifdef CGALPY_TRIANGULATION_2_BINDINGS
  auto tri2_m = m.def_submodule("Tri2");
  export_triangulation_2(tri2_m);
  export_tri2_plain(tri2_m);

#if (CGALPY_TRI2 == CGALPY_TRI2_REGULAR)
  export_tri2_regular(tri2_m);
#elif (CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED)
  export_tri2_constrained(tri2_m);
#elif (CGALPY_TRI2 == CGALPY_TRI2_DELAUNAY)
  export_tri2_delaunay(tri2_m);
#elif (CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED_DELAUNAY)
  export_tri2_constrained_delaunay(tri2_m);
#endif

#endif

  // 2D Alpha shape must be bound after 2D triangulation!
#if defined(CGALPY_ALPHA_SHAPE_2_BINDINGS)
  // AS2 must scceed Tri2
  auto as2_m = m.def_submodule("As2");
  export_alpha_shape_2(as2_m);
#endif

#if defined(CGALPY_SPATIAL_SEARCHING_BINDINGS)
  auto ss_m = m.def_submodule("Ss");
  export_spatial_searching(ss_m);
#endif

#if defined(CGALPY_SPATIAL_SORTING_BINDINGS)
  auto st_m = m.def_submodule("St");
  export_spatial_sorting(st_m);
#endif

#if defined(CGALPY_STRAIGHT_SKELETON_2_BINDINGS)
  auto sn2_m = m.def_submodule("Sn2");
  export_straight_skeleton_2(sn2_m);
#endif

#ifdef CGALPY_BGL_BINDINGS
  auto bgl_m = m.def_submodule("Bgl");
  export_bgl(bgl_m);
#endif

#ifdef CGALPY_BOUNDING_VOLUMES_BINDINGS
  auto bv_m = m.def_submodule("Bv");
  export_bounding_volumes(bv_m);
#endif

#if defined(CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS)
  // BSO2 must succeed Aos2
  auto bso2_m = m.def_submodule("Bso2");
  export_boolean_set_operations_2(bso2_m);
  export_general_polygon_set_2(bso2_m);
#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
  export_polygon_set_2(bso2_m);
#endif
#endif

#ifdef CGALPY_POINT_SET_PROCESSING_BINDINGS
  auto psp_m = m.def_submodule("Psp");
  export_point_set_processing(psp_m);
#endif

#if defined(CGALPY_POLYGON_PARTITIONING_BINDINGS)
  auto pp2_m = m.def_submodule("Pp2");
  export_polygon_partition_2(pp2_m);
#endif

#if defined(CGALPY_MINKOWSKI_SUM_2_BINDINGS)
  auto ms2_m = m.def_submodule("Ms2");
  export_minkowski_sum_2(ms2_m);
#endif

#if defined(CGALPY_CONVEX_HULL_3_BINDINGS)
  auto ch3_m = m.def_submodule("Ch3");
  export_convex_hull_3(ch3_m);
#endif

#if defined(CGALPY_ENVELOPE_2_BINDINGS)
  auto env2_m = m.def_submodule("Env2");
  export_envelope_2(env2_m);
#endif

#if defined(CGALPY_ENVELOPE_3_BINDINGS)
  auto env3_m = m.def_submodule("Env3");
  export_envelope_3(env3_m);
#endif

#if defined(CGALPY_TRIANGULATION_3_BINDINGS)
  auto tri3_m = m.def_submodule("Tri3");
  export_tri3_plain(tri3_m);
  export_triangulation_3(tri3_m);
#if (CGALPY_TRI3 == CGALPY_TRI3_REGULAR)
  export_tri3_regular(tri3_m);
#elif (CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY)
  export_tri3_delaunay(tri3_m);
#endif
#endif

#if defined(CGALPY_TRIANGULATION_D_BINDINGS)
  auto trid_m = m.def_submodule("Trid");
  export_triangulation_d(trid_m);
#endif

// 3D Alpha shape must be bound after 3D triangulation!
#ifdef CGALPY_ALPHA_SHAPE_3_BINDINGS
  auto as3_m = m.def_submodule("As3");
  export_alpha_shape_3(as3_m);
#endif

#if defined(CGALPY_NEF_3_BINDINGS)
  auto nef3_m = m.def_submodule("Nef3");
  export_nef_3(nef3_m);
#endif

#if defined(CGALPY_POLYHEDRON_3_BINDINGS)
  auto pol3_m = m.def_submodule("Pol3");
  export_polyhedron_3(pol3_m);
  export_pol3_bgl(pol3_m);
#endif

#if defined(CGALPY_SURFACE_MESH_BINDINGS)
  auto sm_m = m.def_submodule("Sm");
  export_surface_mesh(sm_m);
#endif

#if defined(CGALPY_SURFACE_SWEEP_2_BINDINGS)
  auto ss2_m = m.def_submodule("Ss2");
  export_surface_sweep_2(ss2_m);
#endif

#if defined(CGALPY_POLYGON_MESH_PROCESSING_BINDINGS)
  auto pmp_m = m.def_submodule("Pmp");
  export_polygon_mesh_processing(pmp_m);

#if defined(CGALPY_PMP_COMBINATORIAL_REPAIR_BINDINGS)
  export_pmp_combinatorial_repair(pmp_m);
#endif
#if defined(CGALPY_PMP_CONNECTED_COMPONENTS_BINDINGS)
  export_pmp_connected_components(pmp_m);
#endif
#if defined(CGALPY_PMP_COREFINEMENT_BINDINGS)
  export_pmp_corefinement(pmp_m);
#endif
#if defined(CGALPY_PMP_CORRECTED_CURVATURE_COMPUTATION_BINDINGS)
  export_pmp_corrected_curvature_computation(pmp_m);
#endif
#if defined(CGALPY_PMP_DISTANCE_BINDINGS)
  export_pmp_distance(pmp_m);
#endif
#if defined(CGALPY_PMP_FEATURE_DETECTION_BINDINGS)
  export_pmp_feature_detection(pmp_m);
#endif
#if defined(CGALPY_PMP_GEOMETRIC_MEASURE_BINDINGS)
  export_pmp_geometric_measure(pmp_m);
#endif
#if defined(CGALPY_PMP_GEOMETRIC_REPAIR_BINDINGS)
  export_pmp_geometric_repair(pmp_m);
#endif
#if defined(CGALPY_PMP_HOLE_FILLING_BINDINGS)
  export_pmp_hole_filling(pmp_m);
#endif
#if defined(CGALPY_PMP_INTERSECTION_BINDINGS)
  export_pmp_intersection(pmp_m);
#endif
#if defined(CGALPY_PMP_LOCATION_BINDINGS)
  export_pmp_location(pmp_m);
#endif
#if defined(CGALPY_PMP_NORMAL_COMPUTATION_BINDINGS)
  export_pmp_normal_computation(pmp_m);
#endif
#if defined(CGALPY_PMP_ORIENTATION_BINDINGS)
  export_pmp_orientation(pmp_m);
#endif
#if defined(CGALPY_PMP_MESHING_BINDINGS)
  export_pmp_meshing(pmp_m);
#endif
#if defined(CGALPY_PMP_IO_BINDINGS)
  export_pmp_io(pmp_m);
#endif
#endif

#if defined(CGALPY_TRIANGULATED_SURFACE_MESH_SEGMENTATION_BINDINGS)
  export_triangulated_surface_mesh_segmentation(m);
#endif

#if defined(CGALPY_TRIANGULATED_SURFACE_MESH_SIMPLIFICATION_BINDINGS)
  auto tsms_m = m.def_submodule("Sms");
  export_triangulated_surface_mesh_simplification(tsms_m);
#endif

#if defined(CGALPY_TRIANGULATED_SURFACE_MESH_SKELETONIZATION_BINDINGS)
  auto tsmsk_m = m.def_submodule("Smsk");
  export_triangulated_surface_mesh_skeletonization(tsmsk_m);
#endif

#if defined(CGALPY_SHAPE_DETECTION_BINDINGS)
  auto sd_m = m.def_submodule("Sd",
    "This CGAL component implements two algorithms for shape detection:\n\n"
    "• the Efficient RANSAC (RANdom SAmple Consensus) method, contributed by Schnabel et al. [2];\n"
    "• the Region Growing method, contributed by Lafarge and Mallet [1].");
  export_efficient_ransac(sd_m);
  export_region_growing(sd_m);
#endif

#if defined(CGALPY_VISIBILITY_2_BINDINGS)
  auto vis2_m = m.def_submodule("Vis2");
  export_visibility_2(vis2_m);
#endif

#if defined(CGALPY_BASIC_VIEWER_BINDINGS)
  auto bvr_m = m.def_submodule("Bvr");
  export_basic_viewer(bvr_m);
#endif

#if defined(CGALPY_GEOMETRIC_OBJECT_GENERATORS_BINDINGS)
  auto gog_m = m.def_submodule("Gog");
  export_geometric_object_generators(gog_m);
#endif
}
