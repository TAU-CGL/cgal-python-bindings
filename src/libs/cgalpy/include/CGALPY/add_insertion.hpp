// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ADD_INSERTION_HPP
#define CGALPY_ADD_INSERTION_HPP

#include <sstream>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

template <typename PyClass>
void add_insertion(PyClass& cls, std::string const& method) {
  cls.def(method.c_str(), [](typename PyClass::Type const& self) {
                            std::ostringstream os;
                            os << self;
                            return os.str();
                          });
}

#endif
