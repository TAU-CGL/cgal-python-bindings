// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "cgalpy/add_attr.hpp"
#include "cgalpy/stl_forward_iterator.hpp"
#include "cgalpy/triangulation_2_types.hpp"
#include "cgalpy/Tri2_docstrings.hpp"

namespace py = nanobind;
namespace tri2_doc = cgalpy::tri2::docstrings;

namespace cgalpy {
namespace tri2 {

//!
void dt2_init(tri2::Delaunay_triangulation_2* tri, py::list& lst) {
  auto begin = stl_forward_iterator<tri2::Point>(lst);
  auto end = stl_forward_iterator<tri2::Point>(lst, false);
  new (tri) tri2::Delaunay_triangulation_2(begin, end);  // placement new
}

}
} // namespace cgalpy

//!
void export_tri2_delaunay(py::module_& m) {
  using Dtri = cgalpy::tri2::Delaunay_triangulation_2;
  using Tri = cgalpy::tri2::Triangulation_2;
  using Pnt = cgalpy::tri2::Point;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Dtri>(m, "Delaunay_triangulation_2")) return;

  py::class_<Dtri, Tri> tri_c(m, "Delaunay_triangulation_2",
                                 tri2_doc::Delaunay_triangulation_2_class);

  tri_c.def(py::init<>(),
            "Constructs an empty Delaunay 2D triangulation.")
    .def(py::init<const cgalpy::tri2::Traits&>(),
         py::arg("traits"),
         "Constructs a Delaunay 2D triangulation with geometric traits.")
    .def("__init__", &cgalpy::tri2::dt2_init,
         py::arg("points"),
         "Constructs a Delaunay 2D triangulation from input points.")
    // draw_dual
    // dual
    // dual
    // dual
    // dual
    // get_boundary_of_conflicts
    // get_conflicts
    // get_conflicts_and_boundary
    .def("insert", &cgalpy::tri2::insert_point1<Dtri>, ri, py::arg("p"),
         tri2_doc::Delaunay_triangulation_2_insert)
    .def("insert", &cgalpy::tri2::insert_point2<Dtri>, ri, py::arg("p"), py::arg("f"),
         tri2_doc::Delaunay_triangulation_2_insert)
    .def("insert", &cgalpy::tri2::insert_point3<Dtri>, ri, py::arg("p"), py::arg("lt"), py::arg("loc"), py::arg("li"),
         tri2_doc::Delaunay_triangulation_2_insert_1)
    .def("insert", &cgalpy::tri2::insert_points<Dtri>, ri, py::arg("points"),
         tri2_doc::Delaunay_triangulation_2_insert_2)
    .def("is_valid", &Dtri::is_valid, py::arg("verbose") = false, py::arg("level") = 0,
         tri2_doc::Delaunay_triangulation_2_is_valid)
    // move
    // move_if_no_collision
    // nearest_vertex
    // push_back
    // remove
    // side_of_oriented_circle
    ;
}
