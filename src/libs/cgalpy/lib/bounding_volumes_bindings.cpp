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

#include "CGALPY/bounding_volumes_config.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

typedef typename CGAL::Min_circle_2_traits_2<Kernel>     Min_circle_2_traits_2;
typedef typename Min_circle_2_traits_2::Circle           Optimisation_circle_2;
typedef typename CGAL::Min_circle_2<Min_circle_2_traits_2> Min_circle_2;

void init_min_circle_2_from_list(Min_circle_2* mc, py::list& lst, bool random) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  new (mc) Min_circle_2(begin, end, random);    // placement new
}

void insert_list(Min_circle_2& mc, py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  mc.insert(begin, end);
}

void export_bounding_volumes(py::module_& m) {
  py::class_<Optimisation_circle_2> oc_co(m, "Optimization_circle_2");
  oc_co.def(py::init<>())
    .def("set", py::overload_cast<>(&Optimisation_circle_2::set))
    .def("set", py::overload_cast<const Point_2&>(&Optimisation_circle_2::set))
    .def("set", py::overload_cast<const Point_2&, const Point_2&>(&Optimisation_circle_2::set))
    .def("set", py::overload_cast<const Point_2&, const Point_2&, const Point_2&>(&Optimisation_circle_2::set))
    .def("set", py::overload_cast<const Point_2&, const FT&>(&Optimisation_circle_2::set))
    .def("center", &Optimisation_circle_2::center)
    .def("squared_radius", &Optimisation_circle_2::squared_radius)
    .def("is_empty", &Optimisation_circle_2::is_empty)
    .def("is_degenerate", &Optimisation_circle_2::is_degenerate)
    .def(py::self == py::self)
    .def(py::self != py::self)
    ;

  add_insertion(oc_co, "__str__");
  add_insertion(oc_co, "__repr__");

  py::class_<Min_circle_2, boost::noncopyable>(m, "Min_circle_2")
    .def(py::init<>())
    .def(py::init<const Point_2&>())
    .def(py::init<const Point_2&, const Point_2&>())
    .def(py::init<const Point_2&, const Point_2&, const Point_2&>())
    .def("__init__", &init_min_circle_2_from_list)
    .def("number_of_points", &Min_circle_2::number_of_points)
    .def("number_of_support_points", &Min_circle_2::number_of_support_points)
    .def("points", py::range<py::return_internal_reference<>>(&Min_circle_2::points_begin, &Min_circle_2::points_end))
    .def("support_points", py::range<py::return_internal_reference<>>(&Min_circle_2::support_points_begin, &Min_circle_2::support_points_end))
    .def("support_point", &Min_circle_2::support_point, py::return_internal_reference<>())
    .def("circle", &Min_circle_2::circle)
    .def("bounded_side", &Min_circle_2::bounded_side)
    .def("has_on_bounded_side", &Min_circle_2::has_on_bounded_side)
    .def("has_on_boundary", &Min_circle_2::has_on_boundary)
    .def("has_on_unbounded_side", &Min_circle_2::has_on_unbounded_side)
    .def("is_empty", &Min_circle_2::is_empty)
    .def("is_degenerate", &Min_circle_2::is_degenerate)
    .def("insert", py::overload_cast<const Point_2&>(&Min_circle_2::insert))
    .def("insert", &insert_list)
    .def("clear", &Min_circle_2::clear)
    .def("is_valid", &Min_circle_2::is_valid)
    ;
}
