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

#include "cgalpy/Bso2_docstrings.hpp"
#ifdef CGALPY_HAS_VISUAL
#define CGAL_USE_BASIC_VIEWER
#include <CGAL/draw_polygon_set_2.h>
#endif

namespace py = nanobind;
namespace bso2_doc = cgalpy::bso2::docstrings;

void export_polygon_set_2(py::module_& m) {
  using Gps2 = cgalpy::bso2::General_polygon_set_2;
  using Ps2 = cgalpy::bso2::Polygon_set_2;
  using Gt = cgalpy::bso2::Geometry_traits_2;
  using Pgn = cgalpy::bso2::General_polygon_2;
  using Pwh = cgalpy::bso2::General_polygon_with_holes_2;

  py::class_<Ps2, Gps2> ps2_c(
    m, "Polygon_set_2", bso2_doc::Polygon_set_2_class);
  ps2_c.def(py::init<>(),
            "Construct a default polygon set.")
    .def(py::init<const Pgn&>(), py::arg("polygon"),
         "Construct a polygon set from a polygon.")
    .def(py::init<const Pwh&>(), py::arg("polygon_with_holes"),
         "Construct a polygon set from a polygon with holes.")
    .def(py::init<const Ps2&>(), py::arg("other"),
         "Copy-construct a polygon set.")
    .def(py::init<const Gt&>(), py::arg("traits"),
         "Construct a polygon set from geometry traits.")
    ;

#ifdef CGALPY_HAS_VISUAL
  using Draw = void(*)(const Ps2&, const char*);
  m.def("draw", static_cast<Draw>(CGAL::draw),
        py::arg("polygon_set"), py::arg("title"),
        "Draw a polygon set in a CGAL basic viewer.");
#endif

  add_extraction(ps2_c);
}
