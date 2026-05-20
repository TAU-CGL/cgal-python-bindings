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

namespace py = nanobind;

extern void export_arr_non_caching_segment_basic_traits_2(py::module_& m);

void export_arr_non_caching_segment_traits_2(py::module_& m) {
  using Bgt = CGAL::Arr_non_caching_segment_basic_traits_2<Kernel>;
  using Gt = CGAL::Arr_non_caching_segment_traits_2<Kernel>;

  using Xcv = Bgt::X_monotone_curve_2;
  using Cv = Gt::Curve_2;
  using Ctr_cv = Gt::Construct_curve_2;
  using Intersect_2 = Gt::Intersect_2;
  using Split_2 = Gt::Split_2;
  using Merge_2 = Gt::Merge_2;
  using Are_mergeable_2 = Gt::Are_mergeable_2;
  using Make_x_monotone_2 = Gt::Make_x_monotone_2;

  export_arr_non_caching_segment_basic_traits_2(m);

  if (add_attr<Gt>(m, "Arr_non_caching_segment_traits_2")) return;
  py::class_<Gt, Bgt> traits_c(m, "Arr_non_caching_segment_traits_2");

  add_attr<Cv>(traits_c, "Curve_2");
  add_attr<Ctr_cv>(traits_c, "Construct_curve_2");

  py::class_<Intersect_2>(traits_c, "Intersect_2")
    .def("__call__", &intersect_2_call_operator<Gt>)
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
    .def("__call__", &make_x_monotone_2_call_operator<Gt>)
    ;

  using Ctr_opposite = Gt::Construct_opposite_2;
  using Cmp_endpoints_xy = Gt::Compare_endpoints_xy_2;

  py::class_<Ctr_opposite>(traits_c, "Construct_opposite_2")
    .def("__call__", [](const Ctr_opposite& ctr_opp, const Xcv& xcv)->Xcv
                     { return ctr_opp(xcv); })
    ;

  py::class_<Cmp_endpoints_xy>(traits_c, "Compare_endpoints_xy_2")
    .def("__call__", &Cmp_endpoints_xy::operator())
    ;

  traits_c.def(py::init<>())
    .def(py::init<const Gt&>())
    .def("intersect_2_object", &Gt::intersect_2_object)
    .def("split_2_object", &Gt::split_2_object)
    .def("are_mergeable_2_object", &Gt::are_mergeable_2_object)
    .def("merge_2_object", &Gt::merge_2_object)
    .def("make_x_monotone_2_object", &Gt::make_x_monotone_2_object)
    .def("construct_opposite_2_object", &Gt::construct_opposite_2_object)
    .def("compare_endpoints_xy_2_object", &Gt::compare_endpoints_xy_2_object)
    .def("construct_curve_2_object", &Gt::construct_curve_2_object)
    ;
}
