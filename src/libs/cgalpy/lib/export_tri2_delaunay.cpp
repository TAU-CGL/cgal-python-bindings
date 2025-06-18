// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/triangulation_2_types.hpp"

namespace py = nanobind;

namespace tri2 {

//!
void dt2_init(tri2::Delaunay_triangulation_2* tri, py::list& lst) {
  auto begin = stl_input_iterator<tri2::Point>(lst);
  auto end = stl_input_iterator<tri2::Point>(lst, false);
  new (tri) tri2::Delaunay_triangulation_2(begin, end);  // placement new
}

}

//!
void export_tri2_delaunay(py::module_& m) {
  using Dtri = tri2::Delaunay_triangulation_2;
  using Tri = tri2::Triangulation_2;
  using Pnt = tri2::Point;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Dtri>(m, "Delaunay_triangulation_2")) return;

  py::class_<Dtri, Tri> tri_c(m, "Delaunay_triangulation_2");

  tri_c.def(py::init<>())
    .def(py::init<const tri2::Traits&>())
    .def("__init__", &tri2::dt2_init)
    // draw_dual
    // dual
    // dual
    // dual
    // dual
    // get_boundary_of_conflicts
    // get_conflicts
    // get_conflicts_and_boundary
    .def("insert", &tri2::insert_point1<Dtri>, ri, py::arg("p"),
         "Parameters:\n"
         "  p (Point_2): The point\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert", &tri2::insert_point2<Dtri>, ri, py::arg("p"), py::arg("start"),
         "Parameters:\n"
         "  p (Point_2): The point\n"
         "  start (Face): Start the search at this face\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert", &tri2::insert_point3<Dtri>, ri, py::arg("p"), py::arg("lt"), py::arg("loc"), py::arg("li"),
         "Insert a point into the triangulation using the values returned from a previous location query\n"
         "Parameters:\n"
         "  p (Point_3): The point\n"
         "  lt (Locate_type): together with loc and li the return values of a previous location query\n"
         "  loc (Face)\n"
         "  li (int)\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert", &tri2::insert_points<Dtri>, ri, py::arg("points"),
         "Insert a list of points\n"
         "Parameters:\n"
         "  points (list) the list of points\n"
         "Return:\n"
         "  The number of inserted points\n")
    // is_valid
    // move
    // move_if_no_collision
    // nearest_vertex
    // push_back
    // remove
    // side_of_oriented_circle
    ;
}
