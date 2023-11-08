// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/envelope_2.h>

#include "CGALPY/envelope_2_types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

namespace env2 {

//
Envelope_diagram_1 lower_envelope_2(const py::list& curvess) {
  Envelope_diagram_1 ed;
  auto begin = stl_input_iterator<Curve_2>(curvess);
  auto end = stl_input_iterator<Curve_2>(curvess, false);
  CGAL::lower_envelope_2(begin, end, ed);
  return ed;
}

//
Envelope_diagram_1 lower_envelope_x_monotone_2(const py::list& curves) {
  Envelope_diagram_1 ed;
  auto begin = stl_input_iterator<X_monotone_curve_2>(curves);
  auto end = stl_input_iterator<X_monotone_curve_2>(curves, false);
  std::vector<X_monotone_curve_2> xcvs(std::distance(begin, end));
  std::copy(begin, end, xcvs.begin());
  // CGAL::lower_envelope_x_monotone_2(begin, end, ed);
  CGAL::lower_envelope_x_monotone_2(xcvs.begin(), xcvs.end(), ed);
  xcvs.clear();
  return ed;
}

//
Envelope_diagram_1 upper_envelope_2(const py::list& curves) {
  Envelope_diagram_1 ed;
  auto begin = stl_input_iterator<Curve_2>(curves);
  auto end = stl_input_iterator<Curve_2>(curves, false);
  CGAL::upper_envelope_2(begin, end, ed);
  return ed;
}

//
Envelope_diagram_1 upper_envelope_x_monotone_2(const py::list& curves) {
  Envelope_diagram_1 ed;
  auto begin = stl_input_iterator<X_monotone_curve_2>(curves);
  auto end = stl_input_iterator<X_monotone_curve_2>(curves, false);
  std::vector<X_monotone_curve_2> xcvs(std::distance(begin, end));
  std::copy(begin, end, xcvs.begin());
  // CGAL::upper_envelope_x_monotone_2(begin, end, ed);
  CGAL::upper_envelope_x_monotone_2(xcvs.begin(), xcvs.end(), ed);
  return ed;
}

}

void export_envelope_2 (py::module_& m) {
  using Ed = env2::Envelope_diagram_1;
  using Vertex = Ed::Vertex;
  using Edge = Ed::Edge;
  using Pnt = Ed::Point_2;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (! add_attr<Vertex>(m, "Vertex")) {
    py::class_<Vertex>(m, "Vertex")
      .def(py::init<>())
      .def(py::init<const Pnt&>())
      .def("point", &Vertex::point, ri)
      .def("number_of_curves", &Vertex::number_of_curves)
      .def("left", [](const Vertex& v)->const Edge& { return *(v.left()); }, ri)
      .def("right", [](const Vertex& v)->const Edge& { return *(v.right()); }, ri)
      ;
  }

  if (! add_attr<Edge>(m, "Edge")) {
    py::class_<Edge>(m, "Edge")
      .def(py::init<>())
      .def("is_empty", &Edge::is_empty)
      .def("curve", &Edge::curve, ri)
      .def("left", [](const Edge& e)->const Vertex& { return *(e.left()); }, ri)
      .def("right", [](const Edge& e)->const Vertex& { return *(e.right()); }, ri)
      ;
  }

  if (! add_attr<Ed>(m, "Envelope_diagram_1")) {
    py::class_<Ed>(m, "Envelope_diagram_1")
      .def(py::init<>())
      .def("leftmost",
           [](const Ed& ed)->const Edge& { return *(ed.leftmost()); }, ri)
      .def("rightmost",
           [](const Ed& ed)->const Edge& { return *(ed.rightmost()); }, ri)
      ;
  }

  m.def("lower_envelope_2", &env2::lower_envelope_2);
  m.def("lower_envelope_x_monotone_2", &env2::lower_envelope_x_monotone_2);
  m.def("upper_envelope_2", &env2::upper_envelope_2);
  m.def("upper_envelope_x_monotone_2", &env2::upper_envelope_x_monotone_2);
}
