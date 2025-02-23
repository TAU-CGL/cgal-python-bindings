// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Polyhedron_traits_with_normals_3.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/polyhedron_3_types.hpp"

namespace py = nanobind;

/*!
 */
void export_polyhedron_traits_with_normals_3(py::module_& m) {
  using Traits = CGAL::Polyhedron_traits_with_normals_3<Kernel>;
  if (! add_attr<Traits>(m, "Polyhedron_traits_with_normals_3")) {
    py::class_<Traits>(m, "Polyhedron_traits_with_normals_3")
      .def(py::init<>())
      .def(py::init<const Kernel&>())
      ;
  }
}
