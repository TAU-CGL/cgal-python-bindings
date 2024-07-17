// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_UNIQUE_HPP
#define CGALPY_EXPORT_UNIQUE_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

template <typename T, typename Op>
void export_unique(const char* name, Op& op) {
  const py::type_info info = py::type_id<T>();
  const auto* reg = py::converter::registry::query(info);
  if ((reg == nullptr) || ((*reg).m_to_python == nullptr))
    op(name);
  else
    py::scope().attr(name) = py::handle<>(reg->m_class_object);
}

#endif
