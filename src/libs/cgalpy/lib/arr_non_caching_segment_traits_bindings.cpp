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
#include "CGALPY/aos_2_concepts/export_AosBasicTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosLandmarkTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosDirectionalXMonotoneTraits_2.hpp"

#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_landmark_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_directional_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_approximate_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_construct_x_monotone_curve_traits_classes.hpp"

namespace py = nanobind;

//
py::object export_arr_non_caching_segment_traits(py::module_& m) {
  using BGT = CGAL::Arr_non_caching_segment_basic_traits_2<Kernel>;
  using GT = CGAL::Arr_non_caching_segment_traits_2<Kernel>;

  py::class_<BGT, Kernel> bt_c(m, "Arr_non_caching_segment_basic_traits_2");

  /// \name AosBaiscTraits
  /// @{
  using Point_2 = BGT::Point_2;
  using X_monotone_curve_2 = BGT::X_monotone_curve_2;
  using Compare_x_2 = BGT::Compare_x_2;
  using Compare_xy_2 = BGT::Compare_xy_2;
  using Construct_min_vertex_2 = BGT::Construct_min_vertex_2;
  using Construct_max_vertex_2 = BGT::Construct_max_vertex_2;
  using Is_vertical_2 = BGT::Is_vertical_2;
  using Compare_y_at_x_2 = BGT::Compare_y_at_x_2;
  using Compare_y_at_x_right_2 = BGT::Compare_y_at_x_right_2;
  using Compare_y_at_x_left_2 = BGT::Compare_y_at_x_left_2;
  using Equal_2 = BGT::Equal_2;

  add_attr<X_monotone_curve_2>(bt_c, "X_monotone_curve_2");

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
    ;
  /// @}

  /// \name AosXMonotoneTraits & AosTraits
  /// @{

  using Curve_2 = GT::Curve_2;
  using Intersect_2 = GT::Intersect_2;
  using Split_2 = GT::Split_2;
  using Merge_2 = GT::Merge_2;
  using Are_mergeable_2 = GT::Are_mergeable_2;
  using Make_x_monotone_2 = GT::Make_x_monotone_2;

  py::class_<GT, BGT> traits_c(m, "Arr_non_caching_segment_traits_2");

  add_attr<Curve_2>(traits_c, "Curve_2");

  py::class_<Intersect_2>(traits_c, "Intersect_2")
    // .def("__call__", &Intersect_2::operator())
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
    // .def("__call__", &Make_x_monotone_2::operator())
    ;

  traits_c.def(py::init<>())
    .def(py::init<const GT&>())
    .def("intersect_2_object", &GT::intersect_2_object)
    .def("split_2_object", &GT::split_2_object)
    .def("are_mergeable_2_object", &GT::are_mergeable_2_object)
    .def("merge_2_object", &GT::merge_2_object)
    .def("make_x_monotone_2_object", &GT::make_x_monotone_2_object);
    ;

    /// @}

  return traits_c;
}
