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
#include "CGALPY/stl_forward_iterator.hpp"
#include "CGALPY/make_iterator.hpp"

#include "cgalpy/Env2_docstrings.hpp"

namespace py = nanobind;
namespace env2_doc = cgalpy::docstrings::Envelope_2;

namespace env2 {

//
template <typename Cell>
py::object curves(const Cell& c)
{ return make_iterator(c.curves_begin(), c.curves_end()); }

//
Envelope_diagram_1 lower_envelope_2(const py::list& curvess) {
  Envelope_diagram_1 ed;
  auto begin = stl_forward_iterator<Curve_2>(curvess);
  auto end = stl_forward_iterator<Curve_2>(curvess, false);
  CGAL::lower_envelope_2(begin, end, ed);
  return ed;
}

//
Envelope_diagram_1 lower_envelope_x_monotone_2(const py::list& curves) {
  Envelope_diagram_1 ed;
  auto begin = stl_forward_iterator<X_monotone_curve_2>(curves);
  auto end = stl_forward_iterator<X_monotone_curve_2>(curves, false);
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
  auto begin = stl_forward_iterator<Curve_2>(curves);
  auto end = stl_forward_iterator<Curve_2>(curves, false);
  CGAL::upper_envelope_2(begin, end, ed);
  return ed;
}

//
Envelope_diagram_1 upper_envelope_x_monotone_2(const py::list& curves) {
  Envelope_diagram_1 ed;
  auto begin = stl_forward_iterator<X_monotone_curve_2>(curves);
  auto end = stl_forward_iterator<X_monotone_curve_2>(curves, false);
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
  using Xcv = Ed::X_monotone_curve_2;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (! add_attr<Vertex>(m, "Vertex")) {
    py::class_<Vertex> vertex_c(m, "Vertex",
                                env2_doc::EnvelopeDiagramVertex_class);
    vertex_c.def(py::init<>(),
                 env2_doc::EnvelopeDiagramVertex_EnvelopeDiagramVertex)
      .def(py::init<const Pnt&>(), py::arg("point"),
           env2_doc::EnvelopeDiagramVertex_EnvelopeDiagramVertex_2)
      .def("point", &Vertex::point, ri,
           env2_doc::EnvelopeDiagramVertex_point)
      .def("number_of_curves", &Vertex::number_of_curves,
           env2_doc::EnvelopeDiagramVertex_number_of_curves)
      .def("left", [](const Vertex& v)->const Edge& { return *(v.left()); },
           ri, env2_doc::EnvelopeDiagramVertex_left)
      .def("right", [](const Vertex& v)->const Edge& { return *(v.right()); },
           ri, env2_doc::EnvelopeDiagramVertex_right)
      .def("curves", &env2::curves<Vertex>, py::keep_alive<0, 1>(),
           env2_doc::EnvelopeDiagramVertex_curves_begin)
      ;

    using Cci = Ed::Curve_const_iterator;
    add_iterator<Cci, Cci>("Curve_iterator", vertex_c);
  }

  if (! add_attr<Edge>(m, "Edge")) {
    py::class_<Edge> edge_c(m, "Edge",
                            env2_doc::EnvelopeDiagramEdge_class);
    edge_c.def(py::init<>(),
               env2_doc::EnvelopeDiagramEdge_EnvelopeDiagramEdge)
      .def("is_empty", &Edge::is_empty,
           env2_doc::EnvelopeDiagramEdge_is_empty)
      .def("number_of_curves", &Edge::number_of_curves,
           env2_doc::EnvelopeDiagramEdge_number_of_curves)
      .def("curve", &Edge::curve, ri,
           env2_doc::EnvelopeDiagramEdge_curve)
      .def("left", [](const Edge& e)->const Vertex& { return *(e.left()); },
           ri, env2_doc::EnvelopeDiagramEdge_left)
      .def("right", [](const Edge& e)->const Vertex& { return *(e.right()); },
           ri, env2_doc::EnvelopeDiagramEdge_right)
      .def("curves", &env2::curves<Edge>, py::keep_alive<0, 1>(),
           env2_doc::EnvelopeDiagramEdge_curves_begin)
      ;

    using Cci = Ed::Curve_const_iterator;
    add_iterator<Cci, Cci>("Curve_iterator", edge_c);
  }

  if (! add_attr<Ed>(m, "Envelope_diagram_1")) {
    py::class_<Ed>(m, "Envelope_diagram_1",
                   env2_doc::Envelope_diagram_1_class)
      .def(py::init<>(), env2_doc::EnvelopeDiagram_1_EnvelopeDiagram_1)
      .def("leftmost",
           [](const Ed& ed)->const Edge& { return *(ed.leftmost()); }, ri,
           env2_doc::EnvelopeDiagram_1_leftmost)
      .def("rightmost",
           [](const Ed& ed)->const Edge& { return *(ed.rightmost()); }, ri,
           env2_doc::EnvelopeDiagram_1_rightmost)
      ;
  }

  m.def("lower_envelope_2", &env2::lower_envelope_2,
        py::arg("curves"), env2_doc::lower_envelope_2);
  m.def("lower_envelope_x_monotone_2", &env2::lower_envelope_x_monotone_2,
        py::arg("curves"), env2_doc::lower_envelope_x_monotone_2);
  m.def("upper_envelope_2", &env2::upper_envelope_2,
        py::arg("curves"), env2_doc::upper_envelope_2);
  m.def("upper_envelope_x_monotone_2", &env2::upper_envelope_x_monotone_2,
        py::arg("curves"), env2_doc::upper_envelope_x_monotone_2);
}
