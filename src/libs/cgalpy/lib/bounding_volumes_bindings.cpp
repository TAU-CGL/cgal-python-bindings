// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/bounding_volumes_config.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/stl_forward_iterator.hpp"

namespace py = nanobind;

using Min_circle_2_traits_2 = typename CGAL::Min_circle_2_traits_2<Kernel>;
using Optimisation_circle_2 = typename Min_circle_2_traits_2::Circle;
using Min_circle_2 = typename CGAL::Min_circle_2<Min_circle_2_traits_2>;

void init_min_circle_2_from_list(Min_circle_2* mc, py::list& lst, bool random) {
  auto begin = stl_forward_iterator<Point_2>(lst);
  auto end = stl_forward_iterator<Point_2>(lst, false);
  new (mc) Min_circle_2(begin, end, random);    // placement new
}

void insert_list(Min_circle_2& mc, py::list& lst) {
  auto begin = stl_forward_iterator<Point_2>(lst);
  auto end = stl_forward_iterator<Point_2>(lst, false);
  mc.insert(begin, end);
}

void export_bounding_volumes(py::module_& m) {
  using Oc2 = Optimisation_circle_2;
  using Mc2 = Min_circle_2;

  if (! add_attr<Oc2>(m, "Optimization_circle_2")) {
    py::class_<Oc2> oc_co(m, "Optimization_circle_2");
    oc_co.def(py::init<>())
      .def("set", py::overload_cast<>(&Oc2::set))
      .def("set", py::overload_cast<const Point_2&>(&Oc2::set), py::arg("p"))
      .def("set", py::overload_cast<const Point_2&, const Point_2&>(&Oc2::set), py::arg("p0"), py::arg("p1"))
      .def("set", py::overload_cast<const Point_2&, const Point_2&, const Point_2&>(&Oc2::set), py::arg("p0"), py::arg("p1"), py::arg("p2"))
      .def("set", py::overload_cast<const Point_2&, const FT&>(&Oc2::set))
      .def("center", &Oc2::center)
      .def("squared_radius", &Oc2::squared_radius)
      .def("is_empty", &Oc2::is_empty)
      .def("is_degenerate", &Oc2::is_degenerate)
      .def(py::self == py::self)
      .def(py::self != py::self)
      ;

    add_insertion(oc_co, "__str__");
    add_insertion(oc_co, "__repr__");
  }

  if (! add_attr<Mc2>(m, "Min_circle_2")) {
    py::class_<Mc2, boost::noncopyable>(m, "Min_circle_2")
      .def(py::init<>())
      .def(py::init<const Point_2&>(), py::arg("p"))
      .def(py::init<const Point_2&, const Point_2&>(), py::arg("p0"), py::arg("p1"))
      .def(py::init<const Point_2&, const Point_2&, const Point_2&>(), py::arg("p0"), py::arg("p1"), py::arg("p2"))
      .def("__init__", &init_min_circle_2_from_list, py::arg("points"), py::arg("random"))
      .def("number_of_points", &Mc2::number_of_points)
      .def("number_of_support_points", &Mc2::number_of_support_points)
      .def("points", py::range<py::return_internal_reference<>>(&Mc2::points_begin, &Mc2::points_end))
      .def("support_points", py::range<py::return_internal_reference<>>(&Mc2::support_points_begin, &Mc2::support_points_end))
      .def("support_point", &Mc2::support_point, py::arg("i"), py::return_internal_reference<>())
      .def("circle", &Mc2::circle)
      .def("bounded_side", &Mc2::bounded_side, py::arg("p"))
      .def("has_on_bounded_side", &Mc2::has_on_bounded_side, py::arg("p"))
      .def("has_on_boundary", &Mc2::has_on_boundary, py::arg("p"))
      .def("has_on_unbounded_side", &Mc2::has_on_unbounded_side, py::arg("p"))
      .def("is_empty", &Mc2::is_empty)
      .def("is_degenerate", &Mc2::is_degenerate)
      .def("insert", py::overload_cast<const Point_2&>(&Mc2::insert), py::arg("p"))
      .def("insert", &insert_list, py::arg("points"))
      .def("clear", &Mc2::clear)
      .def("is_valid", &Mc2::is_valid)
      ;
  }
}
