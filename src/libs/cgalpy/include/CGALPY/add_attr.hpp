// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ADD_ATTR_HPP
#define CGALPY_ADD_ATTR_HPP

#include <boost/python.hpp>
#include <boost/static_assert.hpp>

namespace bp = boost::python;

template <typename Type>
bool add_attr(const char* name, bp::scope& my_scope) {
  bp::handle<> tco(bp::objects::registered_class_object(bp::type_id<Type>()));
  if (tco.get() == 0) return false;
  my_scope.attr(name) = tco;
  return true;
}

#endif
