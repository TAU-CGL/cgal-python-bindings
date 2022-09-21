// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/polygon_2_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

namespace pol2 {

Point_2& left_vertex(Polygon_2& pgn) { return *(pgn.left_vertex()); }
Point_2& right_vertex(Polygon_2& pgn) { return *(pgn.right_vertex()); }
Point_2& top_vertex(Polygon_2& pgn) { return *(pgn.top_vertex()); }
Point_2& bottom_vertex(Polygon_2& pgn) { return *(pgn.bottom_vertex()); }

// Initialize a polygon from a list of vertices.
void init_polygon_2(Polygon_2& pgn, py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  new (&pgn) Polygon_2(begin, end);
}

}

// Export Polygon_2.
void export_polygon_2(py::module_& m) {
  constexpr auto ri(py::rv_policy::reference_internal);
  using Pgn = pol2::Polygon_2;
  using Pnt = Pgn::Point_2;

  if (! add_attr<Pgn>(m, "Polygon_2")) {
    py::class_<Pgn> pgn_c(m, "Polygon_2");
    pgn_c.def(py::init<>())
      .def(py::init<const Pgn&>())
      .def("__init__", &pol2::init_polygon_2)
      .def("push_back", &Pgn::push_back)
      .def("is_simple", &Pgn::is_simple)
      .def("is_convex", &Pgn::is_convex)
      .def("orientation", &Pgn::orientation)
      .def("oriented_side", &Pgn::oriented_side)
      .def("bounded_side", &Pgn::bounded_side)
      .def("is_empty", &Pgn::is_empty)
      .def("is_counterclockwise_oriented", &Pgn::is_counterclockwise_oriented)
      .def("is_clockwise_oriented", &Pgn::is_clockwise_oriented)
      .def("is_collinear_oriented", &Pgn::is_collinear_oriented)
      .def("has_on_positive_side", &Pgn::has_on_positive_side)
      .def("has_on_negative_side", &Pgn::has_on_negative_side)
      .def("has_on_boundary", &Pgn::has_on_boundary)
      .def("has_on_bounded_side", &Pgn::has_on_bounded_side)
      .def("has_on_unbounded_side", &Pgn::has_on_unbounded_side)
      .def("size", &Pgn::size)
      .def("area", &Pgn::area)
      .def("bbox", &Pgn::bbox)
      .def("__getitem__",
           static_cast<const Pnt&(Pgn::*)(std::size_t)const>(&Pgn::operator[]))
      .def("left_vertex", &pol2::left_vertex)
      .def("right_vertex", &pol2::right_vertex)
      .def("top_vertex", &pol2::top_vertex)
      .def("bottom_vertex", &pol2::bottom_vertex)

      // Use `py::overload_cast` to cast overloaded functions.
      // 1. As a convention, add the suffix `_mutable` to the mutable version.
      // 2. Wrap the mutable method with the `reference_internal` call policy.
      // 3. Add the `const_` tag to the overloaded const function, as the
      //    overloading is based on constness.
      .def("vertex_mutable", py::overload_cast<size_t>(&Pgn::vertex), ri)
      .def("vertex", py::overload_cast<size_t>(&Pgn::vertex, py::const_))

      .def("edge", &Pgn::edge)
      .def("clear", &Pgn::clear)
      .def("reverse_orientation", &Pgn::reverse_orientation)
      .def(py::self == py::self)
      .def(py::self != py::self)
      ;

    using Eci = Pgn::Edge_const_iterator;
    add_iterator<Eci, Eci>("Edge_iterator", pgn_c);
    pgn_c.def("edges",
              [] (const Pgn& pgn)
              { return make_iterator(pgn.edges_begin(), pgn.edges_end()); },
              py::keep_alive<0, 1>());
    using Vci = Pgn::Vertex_const_iterator;
    add_iterator<Vci, Vci>("Vertex_iterator", pgn_c);
    pgn_c.def("vertices",
              [] (const Pgn& pgn)
              { return make_iterator(pgn.vertices_begin(),
                                     pgn.vertices_end()); },
              py::keep_alive<0, 1>());

    add_insertion(pgn_c, "__str__");
    add_insertion(pgn_c, "__repr__");
    add_extraction(pgn_c);
  }
}
