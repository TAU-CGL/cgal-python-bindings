// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_non_caching_segment_traits_2.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/aos_2_concepts/make_x_monotone_2_call_operator.hpp"
#include "CGALPY/aos_2_concepts/intersect_2_call_operator.hpp"
#include "CGALPY/Kernel/export_point_2.hpp"
#include "CGALPY/Kernel/export_segment_2.hpp"

namespace py = nanobind;

namespace aos2 {
}

//
py::object export_arr_non_caching_segment_traits(py::module_& m) {
  using BGT = CGAL::Arr_non_caching_segment_basic_traits_2<Kernel>;
  using GT = CGAL::Arr_non_caching_segment_traits_2<Kernel>;

  py::class_<BGT, Kernel> bt_c(m, "Arr_non_caching_segment_basic_traits_2");

  /// \name AosBaiscTraits
  /// @{
  using Pnt = BGT::Point_2;
  using Xcv = BGT::X_monotone_curve_2;
  using Ctr_xcv = BGT::Construct_x_monotone_curve_2;
  using Compare_x_2 = BGT::Compare_x_2;
  using Compare_xy_2 = BGT::Compare_xy_2;
  using Construct_min_vertex_2 = BGT::Construct_min_vertex_2;
  using Construct_max_vertex_2 = BGT::Construct_max_vertex_2;
  using Is_vertical_2 = BGT::Is_vertical_2;
  using Compare_y_at_x_2 = BGT::Compare_y_at_x_2;
  using Compare_y_at_x_right_2 = BGT::Compare_y_at_x_right_2;
  using Compare_y_at_x_left_2 = BGT::Compare_y_at_x_left_2;
  using Equal_2 = BGT::Equal_2;

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

  bt_c.def(py::init<>())
    .def(py::init<const BGT&>())
    .def("compare_y_at_x_right_2_object", &BGT::compare_y_at_x_right_2_object)
    .def("compare_y_at_x_left_2_object", &BGT::compare_y_at_x_left_2_object)
    .def("construct_x_monotone_curve_2_object",
         &BGT::construct_x_monotone_curve_2_object);
    ;
  /// @}

  /// \name AosXMonotoneTraits & AosTraits
  /// @{

  using Cv = GT::Curve_2;
  using Ctr_cv = GT::Construct_curve_2;
  using Intersect_2 = GT::Intersect_2;
  using Split_2 = GT::Split_2;
  using Merge_2 = GT::Merge_2;
  using Are_mergeable_2 = GT::Are_mergeable_2;
  using Make_x_monotone_2 = GT::Make_x_monotone_2;

  py::class_<GT, BGT> traits_c(m, "Arr_non_caching_segment_traits_2");

  add_attr<Cv>(traits_c, "Curve_2");
  add_attr<Ctr_cv>(bt_c, "Construct_curve_2");

  py::class_<Intersect_2>(traits_c, "Intersect_2")
    .def("__call__", &intersect_2_call_operator<GT>)
    ;

  py::class_<Split_2>(traits_c, "Split_2")
    .def("__call__", &Split_2::operator())
    ;

  py::class_<Are_mergeable_2>(traits_c, "Are_mergeable_2")
    .def("__call__", &Are_mergeable_2::operator())
    ;

  py::class_<Merge_2>(traits_c, "Merge_2")
    .def("__call__", &Merge_2::operator())
    ;

  py::class_<Make_x_monotone_2>(traits_c, "Make_x_monotone_2")
    .def("__call__", &make_x_monotone_2_call_operator<GT>)
    ;

  using Approx = GT::Approximate_2;
  using Ctr_opposite = GT::Construct_opposite_2;
  using Cmp_endpoints_xy = GT::Compare_endpoints_xy_2;

  py::class_<Approx>(traits_c, "Approximate_2")
    .def("__call__", &Approx::operator())
    ;

  py::class_<Ctr_opposite>(traits_c, "Construct_opposite_2")
    .def("__call__", [](const Ctr_opposite& ctr_opp, const Xcv& xcv)->Xcv
                     { return ctr_opp(xcv); })
    ;

  py::class_<Cmp_endpoints_xy>(traits_c, "Compare_endpoints_xy_2")
    .def("__call__", &Cmp_endpoints_xy::operator())
    ;

  traits_c.def(py::init<>())
    .def(py::init<const GT&>())
    .def("intersect_2_object", &GT::intersect_2_object)
    .def("split_2_object", &GT::split_2_object)
    .def("are_mergeable_2_object", &GT::are_mergeable_2_object)
    .def("merge_2_object", &GT::merge_2_object)
    .def("make_x_monotone_2_object", &GT::make_x_monotone_2_object)
    .def("approximate_2_object", &GT::approximate_2_object)
    .def("construct_opposite_2_object", &GT::construct_opposite_2_object)
    .def("compare_endpoints_xy_2_object", &GT::compare_endpoints_xy_2_object)
    .def("construct_curve_2_object", &GT::construct_curve_2_object)
    ;

    /// @}

  return traits_c;
}
