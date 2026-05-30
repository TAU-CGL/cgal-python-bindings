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

#include "cgalpy/add_attr.hpp"
#include "cgalpy/bounding_volumes_config.hpp"
#include "cgalpy/kernel_types.hpp"
#include "cgalpy/add_insertion.hpp"
#include "cgalpy/stl_forward_iterator.hpp"
#include "cgalpy/Bv_docstrings.hpp"

namespace py = nanobind;
namespace bv_doc = cgalpy::bv::docstrings;

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
    py::class_<Oc2> oc_co(m, "Optimization_circle_2",
                            bv_doc::Circle_class);
    oc_co.def(py::init<>())
      .def("set", py::overload_cast<>(&Oc2::set),
           bv_doc::Circle_set)
      .def("set", py::overload_cast<const Point_2&>(&Oc2::set),
           py::arg("p"), bv_doc::Circle_set_1)
      .def("set", py::overload_cast<const Point_2&, const Point_2&>(&Oc2::set),
           py::arg("p"), py::arg("q"), bv_doc::Circle_set_2)
      .def("set", py::overload_cast<const Point_2&, const Point_2&, const Point_2&>(&Oc2::set),
           py::arg("p"), py::arg("q"), py::arg("r"),
           bv_doc::Circle_set_3)
      .def("set", py::overload_cast<const Point_2&, const FT&>(&Oc2::set),
           py::arg("center"), py::arg("squared_radius"))
      .def("center", &Oc2::center, bv_doc::Circle_center)
      .def("squared_radius", &Oc2::squared_radius,
           bv_doc::Circle_squared_radius)
      .def("is_empty", &Oc2::is_empty, bv_doc::Circle_is_empty)
      .def("is_degenerate", &Oc2::is_degenerate,
           bv_doc::Circle_is_degenerate)
      .def("bounded_side", &Oc2::bounded_side,
           py::arg("p"), bv_doc::Circle_bounded_side)
      .def("has_on_bounded_side", &Oc2::has_on_bounded_side,
           py::arg("p"), bv_doc::Circle_has_on_bounded_side)
      .def("has_on_boundary", &Oc2::has_on_boundary,
           py::arg("p"), bv_doc::Circle_has_on_boundary)
      .def("has_on_unbounded_side", &Oc2::has_on_unbounded_side,
           py::arg("p"), bv_doc::Circle_has_on_unbounded_side)
      .def("__eq__", [](const Oc2& a, const Oc2& b) { return a == b; },
           py::arg("other"))
      .def("__ne__", [](const Oc2& a, const Oc2& b) { return a != b; },
           py::arg("other"))
      ;

    add_insertion(oc_co, "__str__");
    add_insertion(oc_co, "__repr__");
  }

  if (! add_attr<Mc2>(m, "Min_circle_2")) {
    py::class_<Mc2>(m, "Min_circle_2",
                    bv_doc::Min_circle_2_class)
      .def(py::init<>(), bv_doc::Min_circle_2_Min_circle_2_1)
      .def(py::init<const Point_2&>(),
           py::arg("p"), bv_doc::Min_circle_2_Min_circle_2_2)
      .def(py::init<const Point_2&, const Point_2&>(),
           py::arg("p1"), py::arg("p2"),
           bv_doc::Min_circle_2_Min_circle_2_3)
      .def(py::init<const Point_2&, const Point_2&, const Point_2&>(),
           py::arg("p1"), py::arg("p2"), py::arg("p3"),
           bv_doc::Min_circle_2_Min_circle_2_4)
      .def("__init__", &init_min_circle_2_from_list,
           py::arg("points"), py::arg("random") = false,
           bv_doc::Min_circle_2_Min_circle_2)
      .def("number_of_points", &Mc2::number_of_points,
           bv_doc::Min_circle_2_number_of_points)
      .def("number_of_support_points", &Mc2::number_of_support_points,
           bv_doc::Min_circle_2_number_of_support_points)
      .def("points",
           [](const Mc2& mc) {
             py::list lst;
             for (auto it = mc.points_begin(); it != mc.points_end(); ++it) {
               lst.append(*it);
             }
             return lst;
           },
           bv_doc::Min_circle_2_points_begin)
      .def("support_points",
           [](const Mc2& mc) {
             py::list lst;
             for (auto it = mc.support_points_begin();
                  it != mc.support_points_end(); ++it) {
               lst.append(*it);
             }
             return lst;
           },
           bv_doc::Min_circle_2_support_points_begin)
      .def("support_point",
           [](const Mc2& mc, int i) { return mc.support_point(i); },
           py::arg("i"), bv_doc::Min_circle_2_support_point)
      .def("circle", &Mc2::circle, bv_doc::Min_circle_2_circle)
      .def("bounded_side", &Mc2::bounded_side,
           py::arg("p"), bv_doc::Min_circle_2_bounded_side)
      .def("has_on_bounded_side", &Mc2::has_on_bounded_side,
           py::arg("p"), bv_doc::Min_circle_2_has_on_bounded_side)
      .def("has_on_boundary", &Mc2::has_on_boundary,
           py::arg("p"), bv_doc::Min_circle_2_has_on_boundary)
      .def("has_on_unbounded_side", &Mc2::has_on_unbounded_side,
           py::arg("p"), bv_doc::Min_circle_2_has_on_unbounded_side)
      .def("is_empty", &Mc2::is_empty, bv_doc::Min_circle_2_is_empty)
      .def("is_degenerate", &Mc2::is_degenerate,
           bv_doc::Min_circle_2_is_degenerate)
      .def("insert",
           [](Mc2& mc, const Point_2& p) { mc.insert(p); },
           py::arg("p"), bv_doc::Min_circle_2_insert)
      .def("insert", &insert_list,
           py::arg("points"), bv_doc::Min_circle_2_insert_1)
      .def("clear", &Mc2::clear, bv_doc::Min_circle_2_clear)
      .def("is_valid", &Mc2::is_valid,
           py::arg("verbose") = false, py::arg("level") = 0,
           bv_doc::Min_circle_2_is_valid)
      ;
  }
}
