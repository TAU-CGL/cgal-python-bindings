// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#ifndef CGALPY_POLYHEDRAL_ENVELOPE_HPP
#define CGALPY_POLYHEDRAL_ENVELOPE_HPP

#include <nanobind/nanobind.h>
#include "CGALPY/pmp_np_parser.hpp"
#include "CGALPY/pmp_helpers.hpp"
#include <CGAL/Polyhedral_envelope.h>

namespace py = nanobind;

namespace pmp {

template <typename TriangleMesh, typename GeomTraits>
struct Polyhedral_envelope : CGAL::Polyhedral_envelope<GeomTraits> {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Fd = typename Gt::face_descriptor;
  Polyhedral_envelope(TriangleMesh& tmesh, double epsilon, const py::dict& np = py::dict())// {
    : CGAL::Polyhedral_envelope<GeomTraits>
    (tmesh, epsilon,
     internal::parse_pmp_np<TriangleMesh>(np)
     .face_epsilon_map(
     get_face_prop_map<TriangleMesh, double>
     (tmesh, "INTERNAL_MAPA",
      np.contains("face_epsilon_map") ? np["face_epsilon_map"] : py::none(),
      epsilon)))
  {
    // the map isn't deleted! and there is no way to delete it
  }

  Polyhedral_envelope(const std::vector<Fd>& face_range, TriangleMesh& tmesh, double epsilon, const py::dict& np = py::dict())// {
    : CGAL::Polyhedral_envelope<GeomTraits>
     (face_range, tmesh, epsilon,
      internal::parse_pmp_np<TriangleMesh>(np)
      .face_epsilon_map(
      get_face_prop_map<TriangleMesh, double>
      (tmesh, "INTERNAL_MAP0",
       np.contains("face_epsilon_map") ? np["face_epsilon_map"] : py::none(),
       epsilon)))
  {
    // the map isn't deleted! and there is no way to delete it
  }

  Polyhedral_envelope(const std::vector<Point_3> points, const std::vector<std::vector<size_t>>& polygons, double epsilon, const py::dict& np = py::dict())// {
    : CGAL::Polyhedral_envelope<GeomTraits>
     (points, polygons, epsilon)
  {
  }
};

} // namespace pmp


#endif // CGALPY_POLYHEDRAL_ENVELOPE_HPP
