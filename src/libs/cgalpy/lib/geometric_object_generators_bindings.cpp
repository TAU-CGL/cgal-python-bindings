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
#include <CGAL/Random.h>
#include <CGAL/function_objects.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/point_generators_d.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/kernel_types.hpp"
#if defined(CGALPY_KERNEL_BINDINGS)
#include "CGALPY/kernel_d_types.hpp"
#endif

// #include "CGALPY/geometric_object_generators_types.hpp"

namespace cgalpy {
namespace gog {

//!
using Creator_2 = CGAL::Creator_uniform_2<double, Point_2>;
using Random_points_in_disc_2 =
  CGAL::Random_points_in_disc_2<Point_2, Creator_2>;

//!
using Creator_3 = CGAL::Creator_uniform_3<double, Point_3>;
using Random_points_in_sphere_3 =
  CGAL::Random_points_in_sphere_3<Point_3, Creator_3>;

#if defined(CGALPY_KERNEL_BINDINGS)

//!
using Random_points_in_cube_d = CGAL::Random_points_in_cube_d<Point_d>;

#endif

//!
// py::list generate(Random_points_in_disc_2& generator, std::size_t num) {
//   py::list res;
//   for (auto i = 0; i < num; ++i) res.append(*generator++);
//   return res;
// }

}
} // namespace cgalpy

namespace py = nanobind;

//!
void export_geometric_object_generators(py::module_& m) {
  using Rpid = cgalpy::gog::Random_points_in_disc_2;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (! add_attr<Rpid>(m, "Random_points_in_disc_2")) {
    py::class_<Rpid>(m, "Random_points_in_disc_2")
      .def(py::init<double>())
      .def("__iter__", [](Rpid& g) -> Rpid& { return g; }, ri)
      .def("__next__", [](Rpid& g) -> Point_2 { return *g++; })
      ;
  }

  using Rpis = cgalpy::gog::Random_points_in_sphere_3;
  if (! add_attr<Rpis>(m, "Random_points_in_sphere_3")) {
    py::class_<Rpis>(m, "Random_points_in_sphere_3")
      .def(py::init<double, CGAL::Random&>(),
           py::arg("radius") = 1.0, py::arg("rnd") = CGAL::get_default_random())
      .def("__iter__", [](Rpis& g) -> Rpis& { return g; }, ri)
      .def("__next__", [](Rpis& g) -> Point_3 { return *g++; })
      ;
  }

#if defined(CGALPY_KERNEL_BINDINGS)
  using Rpicd = cgalpy::gog::Random_points_in_cube_d;
  if (! add_attr<Rpicd>(m, "Random_points_in_cube_d")) {
    py::class_<Rpicd>(m, "Random_points_in_cube_d")
      .def(py::init<int, double, CGAL::Random&>(),
           py::arg("dim"), py::arg("radius") = 1.0, py::arg("rnd") = CGAL::get_default_random())
      .def("__iter__", [](Rpicd& g) -> Rpicd& { return g; }, ri)
      .def("__next__", [](Rpicd& g) -> Point_d { return *g++; })
      ;
  }
#endif
}
