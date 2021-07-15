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

template <typename T>
void add_attr(const char* name, bp::scope& my_scope) {
  const bp::type_info info = bp::type_id<T>();
  const bp::converter::registration* reg = bp::converter::registry::query(info);
  BOOST_ASSERT((reg != nullptr) && ((*reg).m_to_python != nullptr));
  my_scope.attr(name) = bp::handle<>(reg->m_class_object);
}

#endif
