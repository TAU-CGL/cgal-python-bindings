// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <CGAL/convex_hull_3.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/convex_hull_3_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

//! convex_hull_2
template <typename PolygonMesh>
PolygonMesh convex_hull_3(py::list& lst) {
  using Pm = PolygonMesh;

  Pm pm;
  auto begin = stl_input_iterator<Point_3>(lst);
  auto end = stl_input_iterator<Point_3>(lst, false);
  CGAL::convex_hull_3(begin, end, pm);
  return pm;
}

void export_convex_hull_3_bindings(py::module_& m) {
  using Pm = ch3::Polygonal_mesh;

  m.def("convex_hull_3", &convex_hull_3<Pm>);
}
