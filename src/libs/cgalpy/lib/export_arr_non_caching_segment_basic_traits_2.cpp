// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_non_caching_segment_basic_traits_2.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/Kernel/export_point_2.hpp"
#include "CGALPY/Kernel/export_segment_2.hpp"

namespace py = nanobind;

//
void export_arr_non_caching_segment_basic_traits_2(py::module_& m) {
  using Bgt = CGAL::Arr_non_caching_segment_basic_traits_2<Kernel>;

  if (add_attr<Bgt>(m, "Arr_non_caching_segment_basic_traits_2")) return;

  py::class_<Bgt, Kernel> bt_c(m, "Arr_non_caching_segment_basic_traits_2");

  /// \name AosBaiscTraits
  /// @{
  using Pnt = Bgt::Point_2;
  using Xcv = Bgt::X_monotone_curve_2;
  using Ctr_xcv = Bgt::Construct_x_monotone_curve_2;
  using Compare_x_2 = Bgt::Compare_x_2;
  using Compare_xy_2 = Bgt::Compare_xy_2;
  using Construct_min_vertex_2 = Bgt::Construct_min_vertex_2;
  using Construct_max_vertex_2 = Bgt::Construct_max_vertex_2;
  using Is_vertical_2 = Bgt::Is_vertical_2;
  using Compare_y_at_x_2 = Bgt::Compare_y_at_x_2;
  using Compare_y_at_x_right_2 = Bgt::Compare_y_at_x_right_2;
  using Compare_y_at_x_left_2 = Bgt::Compare_y_at_x_left_2;
  using Equal_2 = Bgt::Equal_2;

  // Point
  if (! add_attr<Pnt>(bt_c, "Point_2")) {
    py::class_<Pnt> pnt_c(bt_c, "Point_2");
    export_point_2<Kernel>(pnt_c);
  }

  // X-monotone curve
  if (! add_attr<Xcv>(bt_c, "X_monotone_curve_2")) {
    py::class_<Xcv> xcv_c(bt_c, "X_monotone_curve_2");
    export_segment_2<Kernel>(xcv_c);
  }

  add_attr<Ctr_xcv>(bt_c, "Construct_x_monotone_curve_2");

  py::class_<Compare_y_at_x_right_2>(bt_c, "Compare_y_at_x_right_2")
    .def("__call__", &Compare_y_at_x_right_2::operator())
    ;

  py::class_<Compare_y_at_x_left_2>(bt_c, "Compare_y_at_x_left_2")
    .def("__call__", &Compare_y_at_x_left_2::operator())
    ;

  using Ant = Bgt::Approximate_number_type;
  using Approx = Bgt::Approximate_2;
  using ovld1 = Ant(Approx::*)(const Pnt&, int i) const;
  py::class_<Approx>(bt_c, "Approximate_2")
    .def("__call__", static_cast<ovld1>(&Approx::operator()))
    ;

  bt_c.def(py::init<>())
    .def(py::init<const Bgt&>())
    .def("compare_y_at_x_right_2_object", &Bgt::compare_y_at_x_right_2_object)
    .def("compare_y_at_x_left_2_object", &Bgt::compare_y_at_x_left_2_object)
    .def("construct_x_monotone_curve_2_object",
         &Bgt::construct_x_monotone_curve_2_object)
    .def("approximate_2_object", &Bgt::approximate_2_object)
    ;
}
