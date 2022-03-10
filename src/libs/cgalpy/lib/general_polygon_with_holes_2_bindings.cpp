// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>

#include "CGALPY/polygon_2_types.hpp"
#include "CGALPY/general_polygon_set_2_types.hpp"

namespace bp = boost::python;

typedef bso2::General_polygon_with_holes_2      General_polygon_with_holes_2;

void export_general_polygon_with_holes_2() {
  bp::handle<> tco(bp::objects::registered_class_object(bp::type_id<General_polygon_with_holes_2>()));
  BOOST_ASSERT(tco.get() != 0);
  bp::scope().attr("General_polygon_with_holes_2") = tco;
}
