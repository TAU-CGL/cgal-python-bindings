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

#include <CGAL/basic.h>
#include <CGAL/IO/io.h>

template <typename PyClass, bool pretty = false>
void add_insertion(PyClass& cls, const char* method) {
  cls.def(method, [](typename PyClass::Type const& self) {
                    std::ostringstream os;
                    if (pretty) CGAL::IO::set_pretty_mode(os);
                    os << self;
                    return os.str();
                  });
}

#endif
