// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Cartesian.h>
#include <CGAL/Simple_cartesian.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/Kernel/export_kernel.hpp"

namespace py = nanobind;

void export_approximate_kernel(py::module_& m) {
  using Cdk = CGAL::Cartesian<double>;

  if (! add_attr<Cdk>(m, "Cartesian_double_kernel")) {
    py::class_<Cdk> ker_c(m, "Cartesian_double_kernel");
    export_kernel<Cdk>(ker_c);
  }

  using Scdk = CGAL::Simple_cartesian<double>;

  if (! add_attr<Scdk>(m, "Simple_cartesian_double_kernel")) {
    py::class_<Scdk> ker_c(m, "Simple_cartesian_double_kernel");
    export_kernel<Scdk>(ker_c);
  }

  // Some default (also for backward compatibility).
  add_attr<Cdk>(m, "Approximate_kernel");
}
