// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#ifndef CGALPY_EXPORT_GENERAL_POLYGON_2_HPP
#define CGALPY_EXPORT_GENERAL_POLYGON_2_HPP

#include <boost/python.hpp>

#include "CGALPY/add_class_object.hpp"

namespace bp = boost::python;

template <typename Type, const char* Name>
void export_general_polygon_2(bp::scope& my_scope, bp::class_<Type>*& co) {
  if (! add_class_object<Type, Name>(my_scope, co)) return;

  co->def(bp::init<>());
  co->def(bp::init<Type>());
  co->def("push_back", &Type::push_back);
  co->def("orientation", &Type::orientation);
  co->def("is_empty", &Type::is_empty);
  co->def("size", &Type::size);
  co->def("bbox", &Type::bbox);
  co->def("clear", &Type::clear);
  co->def("reverse_orientation", &Type::reverse_orientation);
  co->def(bp::self_ns::str(bp::self_ns::self));
  co->def(bp::self_ns::repr(bp::self_ns::self));
}

#endif
