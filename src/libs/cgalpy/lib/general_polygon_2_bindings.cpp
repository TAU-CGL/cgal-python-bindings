// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/polygon_2_types.hpp"
#include "CGALPY/general_polygon_set_2_types.hpp"

namespace py = nanobind;

typedef bso2::General_polygon_2 General_polygon_2;

void export_general_polygon_2(py::module_& m) {
  py::handle<> tco(py::objects::registered_class_object(py::type_id<General_polygon_2>()));
  BOOST_ASSERT(tco.get() != 0);
  py::scope().attr("General_polygon_2") = tco;
}
