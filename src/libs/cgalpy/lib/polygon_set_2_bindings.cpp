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

#include "CGALPY/types.hpp"
#include "CGALPY/polygon_set_2_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/append_iterator.hpp"

namespace py = nanobind;

namespace bso2 {

}

void export_polygon_set_2(py::module_& m) {
  using Gps2 = bso2::General_polygon_set_2;
  using Ps2 = bso2::Polygon_set_2;
  using GT = bso2::Geometry_traits_2;
  using Pgn = bso2::General_polygon_2;
  using Pwh = bso2::General_polygon_with_holes_2;
  using Arrangement_2 = bso2::Arrangement_2;

  py::class_<Ps2, Gps2> ps2_co(m, "Polygon_set_2");
  ps2_co.def(py::init<>())
    .def(py::init<const Pgn&>())
    .def(py::init<const Pwh&>())
    .def(py::init<const Ps2&>())
    ;
}
