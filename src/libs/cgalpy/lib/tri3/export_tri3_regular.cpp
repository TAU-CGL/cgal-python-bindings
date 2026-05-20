// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include "cgalpy/Triangulation_3_docstrings.hpp"

namespace py = nanobind;

namespace tri3 {

}

//!
void export_tri3_regular(py::module_& m) {
  namespace doc = cgalpy::docstrings::Triangulation_3;
}
