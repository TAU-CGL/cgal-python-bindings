// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <vector>

#include <boost/iterator/function_output_iterator.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/tuple.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/export_circulator.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/stl_dereference_forward_iterator.hpp"
#include "CGALPY/stl_forward_iterator.hpp"
#include "CGALPY/triangulation_3_types.hpp"
#include "CGALPY/types.hpp"

namespace py = nanobind;

namespace tri3 {

} // End of namespace tri3

//!
void export_triangulation_3(py::module_& m) {
  using Tri = tri3::Triangulation_3;
  using Edge = tri3::Edge;
  using Cell = tri3::Cell;
  using Face = tri3::Facet;
  using Facet = tri3::Facet;
  using Pnt = tri3::Point;
  using Vertex = tri3::Vertex;

  constexpr auto ri(py::rv_policy::reference_internal);

  using Tds = tri3::Triangulation_data_structure_3;
  if (add_attr<Tds>(m, "Triangulation_data_structure_3")) return;

  py::class_<Tds> tds_c(m, "Triangulation_data_structure_3");

  tds_c.def(py::init<>())
    ;
}
