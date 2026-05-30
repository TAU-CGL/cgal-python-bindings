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

#include "cgalpy/pmp_helpers.hpp"

#include <CGAL/Polyhedral_envelope.h>

namespace py = nanobind;

namespace cgalpy {
namespace pmp {

template <typename TriangleMesh, typename GeomTraits>
struct Polyhedral_envelope : CGAL::Polyhedral_envelope<GeomTraits> {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Fd = typename Gt::face_descriptor;
  Polyhedral_envelope(TriangleMesh& tmesh, double epsilon, const py::dict& np = py::dict()) :
    CGAL::Polyhedral_envelope<GeomTraits>(tmesh, epsilon)
  {
    // the map isn't deleted! and there is no way to delete it
  }

  Polyhedral_envelope(const std::vector<Fd>& face_range, TriangleMesh& tmesh, double epsilon,
                      const py::dict& np = py::dict()) :
    CGAL::Polyhedral_envelope<GeomTraits>(face_range, tmesh, epsilon)
  {
    // the map isn't deleted! and there is no way to delete it
  }

  Polyhedral_envelope(const std::vector<Point_3> points, const std::vector<std::vector<size_t>>& polygons,
                      double epsilon, const py::dict& np = py::dict()) :
    CGAL::Polyhedral_envelope<GeomTraits>(points, polygons, epsilon)
  {
  }
};

}
} // namespace cgalpy // namespace pmp


#endif
