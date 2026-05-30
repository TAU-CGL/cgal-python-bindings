// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com>
//            Efi Fogel          <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Memory_sizer.h>

#include "cgalpy/add_attr.hpp"

namespace py = nanobind;

void export_tools(py::module_& m) {
  if (! add_attr<CGAL::Memory_sizer>(m, "CGAL::Memory_sizer"))
    py::class_<CGAL::Memory_sizer>(m, "Memory_sizer")
      .def(py::init<>())
      .def("virtual_size", &CGAL::Memory_sizer::virtual_size,
           "Returns the virtual memory size in bytes.")
      .def("resident_size", &CGAL::Memory_sizer::resident_size,
           "Returns the resident memory size in bytes.")
      ;
}
