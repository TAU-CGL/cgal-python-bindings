// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/arrangement_on_surface_2_config.hpp"
#include "CGALPY/config.hpp"

namespace py = nanobind;

void export_kernel(py::module_&);
void export_kernel_d(py::module_&);
void export_arrangement_on_surface_2(py::module_&);
void export_intersections_2(py::module_&);
void export_point_location(py::module_&);
void export_object(py::module_&);
void export_polygon_2(py::module_&);
void export_polygon_with_holes_2(py::module_&);
void export_polygon_partition_2(py::module_&);
void export_polygon_set_2(py::module_&);
void export_general_polygon_set_2(py::module_&);
void export_polygon_with_holes_2(py::module_&);
void export_minkowski_sum_2(py::module_&);
void export_boolean_set_operations_2(py::module_&);
void export_triangulation_2(py::module_&);
void export_convex_hull_2_bindings(py::module_&);
void export_spatial_searching(py::module_&);
void export_bounding_volumes(py::module_&);
void export_triangulation_3(py::module_&);
void export_alpha_shape_2(py::module_&);
void export_alpha_shape_3(py::module_&);

#define MY_PYTHON_MODULE(name, m) NB_MODULE(name, m)

MY_PYTHON_MODULE(CGALPY_MODULE_NAME, m) {
  // http://isolation-nation.blogspot.com/2008/09/packages-in-python-extension-modules.html
  m.attr("__path__") = XSTR(CGALPY_MODULE_NAME);

#ifdef CGALPY_KERNEL_BINDINGS
  {
    auto sub_m = m.def_submodule("Ker");
    export_kernel(sub_m);
#ifdef CGALPY_KERNEL_INTERSECTION_BINDINGS
    export_intersections_2(sub_m);
#endif
  }
#endif

#ifdef CGALPY_KERNEL_D_BINDINGS
  {
    auto sub_m = m.def_submodule("Kerd");
    export_kernel_d(sub_m);
  }
#endif

#ifdef CGALPY_POLYGON_2_BINDINGS
  {
    auto sub_m = m.def_submodule("Pol2");
    export_polygon_2(sub_m);
    export_polygon_with_holes_2(sub_m);
  };
#endif

#ifdef CGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS
  // Aos2 must succeed Pol2
  {
    auto sub_m = m.def_submodule("Aos2");
    export_arrangement_on_surface_2(sub_m);

#ifdef CGALPY_AOS2_POINT_LOCATION_BINDINGS
    export_point_location(sub_m);
#endif
  }
#endif

#ifdef CGALPY_CONVEX_HULL_2_BINDINGS
  {
    auto sub_m = m.def_submodule("Ch2");
    export_convex_hull_2_bindings(sub_m);
  };
#endif

#ifdef CGALPY_TRIANGULATION_2_BINDINGS
  {
    auto sub_m = m.def_submodule("Tri2");
    export_triangulation_2(sub_m);
  };

#endif

  // 2D Alpha shape must be bound after 2D triangulation!
#ifdef CGALPY_ALPHA_SHAPE_2_BINDINGS
  // AS2 must scceed Tri2
  {
    auto sub_m = m.def_submodule("As2");
    export_alpha_shape_2(sub_m);
  }
#endif

#ifdef CGALPY_SPATIAL_SEARCHING_BINDINGS
  {
    auto sub_m = m.def_submodule("Ss");
    export_spatial_searching(sub_m);
  };
#endif

#ifdef CGALPY_BOUNDING_VOLUMES_BINDINGS
  {
    auto sub_m = m.def_submodule("Bv");
    export_bounding_volumes(sub_m);
  };

#endif

#ifdef CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS
  // BSO2 must succeed Aos2
  {
    auto sub_m = m.def_submodule("Bso2");
    export_boolean_set_operations_2(sub_m);
#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
    export_polygon_set_2(sub_m);
#else
    export_general_polygon_set_2(sub_m);
#endif
  };
#endif

#ifdef CGALPY_POLYGON_PARTITIONING_BINDINGS
  {
    auto sub_m = m.def_submodule("Pp2");
    export_polygon_partition_2(sub_m);
  };
#endif

#ifdef CGALPY_MINKOWSKI_SUM_2_BINDINGS
  {
    auto sub_m = m.def_submodule("Ms2");
    export_minkowski_sum_2(sub_m);
  };
#endif

#ifdef CGALPY_TRIANGULATION_3_BINDINGS
  {
    auto sub_m = m.def_submodule("Tri3");
    export_triangulation_3(sub_m);
  };
#endif

  // 3D Alpha shape must be bound after 3D triangulation!
#ifdef CGALPY_ALPHA_SHAPE_3_BINDINGS
  {
    auto sub_m = m.def_submodule("As3");
    export_alpha_shape_3(sub_m);
  }
#endif
}
