// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Polygon_set_2.h>
#include <CGAL/IO/Gps_iostream.h>

#include "CGALPY/types.hpp"
#include "CGALPY/polygon_set_2_types.hpp"
#include "CGALPY/add_extraction.hpp"
#include "generated/cgalpy/Boolean_set_operations_2_docstrings.hpp"
#ifdef CGALPY_HAS_VISUAL
#define CGAL_USE_BASIC_VIEWER
#include <CGAL/draw_polygon_set_2.h>
#endif

namespace py = nanobind;
namespace doc = cgalpy::docstrings::Boolean_set_operations_2;

void export_polygon_set_2(py::module_& m) {
  using Gps2 = bso2::General_polygon_set_2;
  using Ps2 = bso2::Polygon_set_2;
  using Gt = bso2::Geometry_traits_2;
  using Pgn = bso2::General_polygon_2;
  using Pwh = bso2::General_polygon_with_holes_2;

  py::class_<Ps2, Gps2> ps2_c(m, "Polygon_set_2", doc::Polygon_set_2__class__);
  ps2_c.def(py::init<>())
    .def(py::init<const Pgn&>())
    .def(py::init<const Pwh&>())
    .def(py::init<const Ps2&>())
    .def(py::init<const Gt&>())
    ;

#ifdef CGALPY_HAS_VISUAL
  using Draw = void(*)(const Ps2&, const char*);
  m.def("draw", static_cast<Draw>(CGAL::draw));
#endif

  add_extraction(ps2_c);
}
