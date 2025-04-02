// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#include <nanobind/nanobind.h>

//! \todo remove
#include "CGALPY/pmp_np_parser.hpp"

#include "CGALPY/polygon_mesh_processing_types.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {
}

//!
void export_pmp_feature_detection(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;
}
