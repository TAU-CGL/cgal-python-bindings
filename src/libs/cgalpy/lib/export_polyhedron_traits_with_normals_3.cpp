// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Polyhedron_traits_with_normals_3.h>

#include "cgalpy/add_attr.hpp"
#include "cgalpy/kernel_types.hpp"
#include "cgalpy/polyhedron_3_types.hpp"
#include "cgalpy/Pol3_docstrings.hpp"

namespace py = nanobind;
namespace pol3_doc = cgalpy::pol3::docstrings;

/*!
 */
void export_polyhedron_traits_with_normals_3(py::module_& m) {
  using Traits = CGAL::Polyhedron_traits_with_normals_3<Kernel>;
  if (! add_attr<Traits>(m, "Polyhedron_traits_with_normals_3")) {
    py::class_<Traits>(m, "Polyhedron_traits_with_normals_3", pol3_doc::Polyhedron_traits_with_normals_3_class)
      .def(py::init<>(), pol3_doc::Polyhedron_traits_with_normals_3_Polyhedron_traits_with_normals_3)
      .def(py::init<const Kernel&>(), py::arg("kernel"), py::keep_alive<1, 2>(), pol3_doc::Polyhedron_traits_with_normals_3_Polyhedron_traits_with_normals_3_1)
      ;
  }
}
