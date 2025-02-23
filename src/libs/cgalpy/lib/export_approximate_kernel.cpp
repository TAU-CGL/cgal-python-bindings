// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Simple_cartesian.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/Kernel/export_kernel.hpp"

namespace py = nanobind;

void export_approximate_kernel(py::module_& m) {
  using Ak = CGAL::Simple_cartesian<double>;

  if (! add_attr<Ak>(m, "Approximate_kernel")) {
    py::class_<Ak> ker_c(m, "Approximate_kernel");
    export_kernel<Ak>(ker_c);
  }
}
