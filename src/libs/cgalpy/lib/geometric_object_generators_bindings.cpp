// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

// #include <algorithm>

#include <nanobind/nanobind.h>

#include <CGAL/Cartesian.h>
#include <CGAL/function_objects.h>
#include <CGAL/point_generators_2.h>
// #include <CGAL/algorithm.h>
// #include <CGAL/random_selection.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/kernel_types.hpp"
// #include "CGALPY/geometric_object_generators_types.hpp"

namespace gog {

using Ak = CGAL::Cartesian<double>;
using Approximate_point_2 = Ak::Point_2;
using Approximate_ft = Ak::FT;
using Creator = CGAL::Creator_uniform_2<Approximate_ft, Approximate_point_2>;
using Random_points_in_disc_2 =
  CGAL::Random_points_in_disc_2<Approximate_point_2, Creator>;

py::list generate(Random_points_in_disc_2& generator, std::size_t num) {
  py::list res;
  for (auto i = 0; i < num; ++i) res.append(*generator++);
  return res;
}

}

namespace py = nanobind;

void export_geometric_object_generators(py::module_& m) {
  using Rpid = gog::Random_points_in_disc_2;
  using Pt = gog::Approximate_point_2;
  constexpr auto ri(py::rv_policy::reference_internal);
  if (! add_attr<Rpid>(m, "Random_points_in_disc_2")) {
    py::class_<Rpid>(m, "Random_points_in_disc_2")
      .def(py::init<double>())
      .def("__iter__", [](Rpid& g) -> Rpid& { return g; }, ri)
      .def("__next__", [](Rpid& g) -> Pt { return *g++; })
      ;
  }
}
