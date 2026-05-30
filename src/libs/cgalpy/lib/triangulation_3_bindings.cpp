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

#include "cgalpy/add_attr.hpp"
#include "cgalpy/add_extraction.hpp"
#include "cgalpy/add_insertion.hpp"
#include "cgalpy/export_circulator.hpp"
#include "cgalpy/make_iterator.hpp"
#include "cgalpy/stl_dereference_forward_iterator.hpp"
#include "cgalpy/stl_forward_iterator.hpp"
#include "cgalpy/triangulation_3_types.hpp"
#include "cgalpy/types.hpp"

namespace py = nanobind;

namespace cgalpy {
namespace tri3 {

}
} // namespace cgalpy // End of namespace tri3

//!
void export_triangulation_3(py::module_& m) {
  using Tri = cgalpy::tri3::Triangulation_3;
  using Edge = cgalpy::tri3::Edge;
  using Cell = cgalpy::tri3::Cell;
  using Face = cgalpy::tri3::Facet;
  using Facet = cgalpy::tri3::Facet;
  using Pnt = cgalpy::tri3::Point;
  using Vertex = cgalpy::tri3::Vertex;

  constexpr auto ri(py::rv_policy::reference_internal);

  using Tds = cgalpy::tri3::Triangulation_data_structure_3;
  if (add_attr<Tds>(m, "Triangulation_data_structure_3")) return;

  py::class_<Tds> tds_c(m, "Triangulation_data_structure_3",
                          "Triangulation data structure used by Triangulation_3.");

  tds_c.def(py::init<>(),
            "Construct an empty triangulation data structure.")
    ;
}
