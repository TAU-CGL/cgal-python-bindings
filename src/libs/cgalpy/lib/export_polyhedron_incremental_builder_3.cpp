// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Polyhedron_incremental_builder_3.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/polyhedron_3_types.hpp"

namespace py = nanobind;

/*!
 */
void export_polyhedron_incremental_builder_3(py::module_& m) {
  using Hds = pol3::Halfedge_ds;
  using Pib = CGAL::Polyhedron_incremental_builder_3<Hds>;
  if (! add_attr<Pib>(m, "Polyhedron_incremental_builder_3")) {
    py::class_<Pib>(m, "Polyhedron_incremental_builder_3")
      .def(py::init<Hds&, bool>())
      .def("begin_surface", &Pib::begin_surface)
      .def("end_surface", &Pib::begin_surface)
      .def("begin_facet", &Pib::begin_facet)
      .def("end_facet", &Pib::begin_facet)
      .def("add_vertex", &Pib::add_vertex)
      .def("add_vertex_to_facet", &Pib::add_vertex_to_facet)
      ;
  }
}
