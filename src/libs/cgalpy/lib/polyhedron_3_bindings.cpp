// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <nanobind/nanobind.h>

#include <CGAL/Polyhedron_3.h>
#include <CGAL/draw_polyhedron.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

namespace pol3 {

typedef CGAL::Polyhedron_3<Kernel>            Polyhedron;

// Draw a polyhedron.
void draw(const Polyhedron& prn) { CGAL::draw(prn); }

}

// Export Polyhedron.
void export_polyhedron_3(py::module_& m) {
  using Prn = pol3::Polyhedron;

  if (! add_attr<Prn>(m, "Polyhedron_3")) {
    py::class_<Prn> prn_c(m, "Polyhedron_3");
    prn_c.def(py::init<>())
      .def(py::init<const Prn&>())
      ;

    add_insertion(prn_c, "__str__");
    add_insertion(prn_c, "__repr__");
    add_extraction(prn_c);
  }

  m.def("draw", &pol3::draw);
}
