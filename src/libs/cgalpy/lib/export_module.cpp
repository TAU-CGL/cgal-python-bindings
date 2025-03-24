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

#include "CGALPY/arrangement_on_surface_2_config.hpp"
#include "CGALPY/config.hpp"

namespace py = nanobind;

void export_2d_range_and_neighbor_search(py::module_&);
void export_3d_point_set(py::module_&);
void export_alpha_shape_2(py::module_&);
void export_alpha_shape_3(py::module_&);
void export_approximate_kernel(py::module_& m);
void export_arrangement_on_surface_2(py::module_&);
void export_basic_viewer(py::module_& m);
void export_boolean_set_operations_2(py::module_&);
void export_bounding_volumes(py::module_&);
void export_cgal(py::module_& m);
void export_connected_components(py::module_&);
void export_convex_hull_2(py::module_&);
void export_convex_hull_3(py::module_&);
void export_corefinement(py::module_&);
void export_efficient_ransac(py::module_&);
void export_envelope_2(py::module_&);
void export_envelope_3(py::module_&);
void export_geometric_object_generators(py::module_& m);
void export_general_polygon_set_2(py::module_&);
void export_intersections(py::module_&);
void export_kernel_module(py::module_&);
void export_kernel_d(py::module_&);
void export_kinetic_surface_reconstruction(py::module_&);
void export_meshing(py::module_&);
void export_minkowski_sum_2(py::module_&);
void export_object(py::module_&);
void export_orientation_functions(py::module_&);
void export_point_location(py::module_&);
void export_point_set_processing(py::module_&);
void export_nef_3(py::module_&);
void export_polyhedron_3(py::module_&);
void export_polygon_2(py::module_&);
void export_polygon_mesh_processing(py::module_&);
void export_polygon_partition_2(py::module_&);
void export_polygon_set_2(py::module_&);
void export_polygon_with_holes_2(py::module_&);
void export_region_growing(py::module_&);
void export_spatial_searching(py::module_&);
void export_surface_mesh(py::module_&);
void export_surface_sweep_2(py::module_&);
void export_tools(py::module_& m);
void export_triangulated_surface_mesh_segmentation(py::module_&);
void export_triangulated_surface_mesh_simplification(py::module_&);
void export_triangulated_surface_mesh_skeletonization(py::module_&);
void export_triangulation_2(py::module_&);
void export_triangulation_3(py::module_&);
void export_triangulation_d(py::module_&);
void export_visibility_2(py::module_&);

#define MY_PYTHON_MODULE(name, m) NB_MODULE(name, m)

MY_PYTHON_MODULE(CGALPY_MODULE_NAME, m) {
  // http://isolation-nation.blogspot.com/2008/09/packages-in-python-extension-modules.html
  m.attr("__path__") = XSTR(CGALPY_MODULE_NAME);

  export_approximate_kernel(m);
  export_cgal(m);
  export_tools(m);

#ifdef CGALPY_KERNEL_BINDINGS
  auto ker_m = m.def_submodule("Ker", kernel_doc());
  export_kernel_module(ker_m);
#ifdef CGALPY_KERNEL_INTERSECTION_BINDINGS
  export_intersections(ker_m);
#endif // CGALPY_KERNEL_INTERSECTION_BINDINGS
#endif // CGALPY_KERNEL_BINDINGS

#ifdef CGALPY_2D_RANGE_AND_NEIGHBOR_SEARCH_BINDINGS
    export_2d_range_and_neighbor_search(m); // currently under CGALPY
#endif // CGALPY_2D_RANGE_AND_NEIGHBOR_SEARCH_BINDINGS
#ifdef CGALPY_3D_POINT_SET_BINDINGS
    export_3d_point_set(m); // currently under CGALPY
#endif // CGALPY_3D_POINT_SET_BINDINGS

#ifdef CGALPY_KINETIC_SURFACE_RECONSTRUCTION_BINDINGS
    export_kinetic_surface_reconstruction(m); // currently under CGALPY
#endif // CGALPY_KINETIC_SURFACE_RECONSTRUCTION_BINDINGS

#ifdef CGALPY_KERNEL_D_BINDINGS
  auto kerd_m = m.def_submodule("Kerd");
  export_kernel_d(kerd_m);
#endif // CGALPY_KERNEL_D_BINDINGS

#ifdef CGALPY_POLYGON_2_BINDINGS
  auto pol2_m = m.def_submodule("Pol2");
  export_polygon_2(pol2_m);
  export_polygon_with_holes_2(pol2_m);
#endif

#ifdef CGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS
  // Aos2 must succeed Pol2
  auto aos2_m = m.def_submodule("Aos2");
  export_arrangement_on_surface_2(aos2_m);
#ifdef CGALPY_AOS2_POINT_LOCATION_BINDINGS
  export_point_location(aos2_m);
#endif
#endif

#ifdef CGALPY_CONVEX_HULL_2_BINDINGS
  auto ch2_m = m.def_submodule("Ch2");
  export_convex_hull_2(ch2_m);
#endif

#ifdef CGALPY_TRIANGULATION_2_BINDINGS
  auto tri2_m = m.def_submodule("Tri2");
  export_triangulation_2(tri2_m);
#endif

  // 2D Alpha shape must be bound after 2D triangulation!
#ifdef CGALPY_ALPHA_SHAPE_2_BINDINGS
  // AS2 must scceed Tri2
  auto as2_m = m.def_submodule("As2");
  export_alpha_shape_2(as2_m);
#endif

#ifdef CGALPY_SPATIAL_SEARCHING_BINDINGS
  auto ss_m = m.def_submodule("Ss");
  export_spatial_searching(ss_m);
#endif

#ifdef CGALPY_BOUNDING_VOLUMES_BINDINGS
  auto bv_m = m.def_submodule("Bv");
  export_bounding_volumes(bv_m);
#endif

#ifdef CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS
  // BSO2 must succeed Aos2
  auto bso2_m = m.def_submodule("Bso2");
  export_boolean_set_operations_2(bso2_m);
  export_general_polygon_set_2(bso2_m);
#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
  export_polygon_set_2(bso2_m);
#endif
#endif

#ifdef CGALPY_POINT_SET_PROCESSING_BINDINGS
  export_point_set_processing(m);
#endif

#ifdef CGALPY_POLYGON_PARTITIONING_BINDINGS
  auto pp2_m = m.def_submodule("Pp2");
  export_polygon_partition_2(pp2_m);
#endif

#ifdef CGALPY_MINKOWSKI_SUM_2_BINDINGS
  auto ms2_m = m.def_submodule("Ms2");
  export_minkowski_sum_2(ms2_m);
#endif

#ifdef CGALPY_CONVEX_HULL_3_BINDINGS
  auto ch3_m = m.def_submodule("Ch3");
  export_convex_hull_3(ch3_m);
#endif

#ifdef CGALPY_ENVELOPE_2_BINDINGS
  auto env2_m = m.def_submodule("Env2");
  export_envelope_2(env2_m);
#endif

#ifdef CGALPY_ENVELOPE_3_BINDINGS
  auto env3_m = m.def_submodule("Env3");
  export_envelope_3(env3_m);
#endif

#ifdef CGALPY_TRIANGULATION_3_BINDINGS
  auto tri3_m = m.def_submodule("Tri3");
  export_triangulation_3(tri3_m);
#endif

#ifdef CGALPY_TRIANGULATION_D_BINDINGS
  auto trid_m = m.def_submodule("Trid");
  export_triangulation_d(trid_m);
#endif

// 3D Alpha shape must be bound after 3D triangulation!
#ifdef CGALPY_ALPHA_SHAPE_3_BINDINGS
  auto as3_m = m.def_submodule("As3");
  export_alpha_shape_3(as3_m);
#endif

#ifdef CGALPY_NEF_3_BINDINGS
  auto nef3_m = m.def_submodule("Nef3");
  export_nef_3(nef3_m);
#endif

#ifdef CGALPY_POLYHEDRON_3_BINDINGS
  auto pol3_m = m.def_submodule("Pol3");
  export_polyhedron_3(pol3_m);
#endif

#ifdef CGALPY_SURFACE_MESH_BINDINGS
  auto sm_m = m.def_submodule("Sm");
  export_surface_mesh(sm_m);
#endif

#ifdef CGALPY_SURFACE_SWEEP_2_BINDINGS
  auto ss2_m = m.def_submodule("Ss2");
  export_surface_sweep_2(ss2_m);
#endif

#ifdef CGALPY_POLYGON_MESH_PROCESSING_BINDINGS
  auto pmp_m = m.def_submodule("Pmp");
  export_connected_components(pmp_m);
  export_corefinement(pmp_m);
  export_meshing(pmp_m);
  export_orientation_functions(pmp_m);
  export_polygon_mesh_processing(pmp_m);
#endif

#ifdef CGALPY_TRIANGULATED_SURFACE_MESH_SEGMENTATION_BINDINGS
  export_triangulated_surface_mesh_segmentation(m);
#endif

#ifdef CGALPY_TRIANGULATED_SURFACE_MESH_SIMPLIFICATION_BINDINGS
  auto tsms_m = m.def_submodule("Sms");
  export_triangulated_surface_mesh_simplification(tsms_m);
#endif

#ifdef CGALPY_TRIANGULATED_SURFACE_MESH_SKELETONIZATION_BINDINGS
  auto tsmsk_m = m.def_submodule("Smsk");
  export_triangulated_surface_mesh_skeletonization(tsmsk_m);
#endif

#ifdef CGALPY_SHAPE_DETECTION_BINDINGS
  auto sd_m = m.def_submodule("Sd",
    "This CGAL component implements two algorithms for shape detection:\n\n"
    "• the Efficient RANSAC (RANdom SAmple Consensus) method, contributed by Schnabel et al. [2];\n"
    "• the Region Growing method, contributed by Lafarge and Mallet [1].");
  export_efficient_ransac(sd_m);
  export_region_growing(sd_m);
#endif

#ifdef CGALPY_VISIBILITY_2_BINDINGS
  auto vis2_m = m.def_submodule("Vis2");
  export_visibility_2(vis2_m);
#endif

#ifdef CGALPY_BASIC_VIEWER_BINDINGS
  auto bvr_m = m.def_submodule("Bvr");
  export_basic_viewer(bvr_m);
#endif

#ifdef CGALPY_GEOMETRIC_OBJECT_GENERATORS_BINDINGS
  auto gog_m = m.def_submodule("Gog");
  export_geometric_object_generators(gog_m);
#endif
}
