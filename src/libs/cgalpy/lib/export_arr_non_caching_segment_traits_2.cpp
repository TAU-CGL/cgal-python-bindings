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
#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_doc = cgalpy::aos2::docstrings;

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
  py::class_<Gt, Bgt> traits_c(m, "Arr_non_caching_segment_traits_2",
                                  aos2_doc::Arr_non_caching_segment_traits_2_class);

  add_attr<Cv>(traits_c, "Curve_2");
  add_attr<Ctr_cv>(traits_c, "Construct_curve_2");

  py::class_<Intersect_2>(traits_c, "Intersect_2",
                            aos2_doc::AosTraits_Intersect_2_class)
    .def("__call__", &intersect_2_call_operator<Gt>,
         py::arg("xcv1"), py::arg("xcv2"),
         aos2_doc::AosTraits_Intersect_2_operator_call)
    ;

  py::class_<Split_2>(traits_c, "Split_2",
                        aos2_doc::AosTraits_Split_2_class)
    .def("__call__", &Split_2::operator(),
         py::arg("xcv"), py::arg("point"),
         py::arg("xcv1"), py::arg("xcv2"),
         aos2_doc::AosTraits_Split_2_operator_call)
    ;

  py::class_<Are_mergeable_2>(traits_c, "Are_mergeable_2",
                                aos2_doc::AosTraits_AreMergeable_2_class)
    .def("__call__", &Are_mergeable_2::operator(),
         py::arg("xcv1"), py::arg("xcv2"),
         aos2_doc::AosTraits_AreMergeable_2_operator_call)
    ;

  py::class_<Merge_2>(traits_c, "Merge_2",
                        aos2_doc::AosTraits_Merge_2_class)
    .def("__call__", &Merge_2::operator(),
         py::arg("xcv1"), py::arg("xcv2"), py::arg("merged"),
         aos2_doc::AosTraits_Merge_2_merge)
    ;

  py::class_<Make_x_monotone_2>(traits_c, "Make_x_monotone_2",
                                  aos2_doc::AosTraits_MakeXMonotone_2_class)
    .def("__call__", &make_x_monotone_2_call_operator<Gt>,
         py::arg("curve"),
         aos2_doc::AosTraits_MakeXMonotone_2_operator_call)
    ;

  using Ctr_opposite = Gt::Construct_opposite_2;
  using Cmp_endpoints_xy = Gt::Compare_endpoints_xy_2;

  py::class_<Ctr_opposite>(traits_c, "Construct_opposite_2",
                             "Functor that constructs the opposite-direction x-monotone curve.")
    .def("__call__", [](const Ctr_opposite& ctr_opp, const Xcv& xcv)->Xcv
                     { return ctr_opp(xcv); },
         py::arg("xcv"),
         "Construct the oppositely directed x-monotone curve.")
    ;

  py::class_<Cmp_endpoints_xy>(traits_c, "Compare_endpoints_xy_2",
                                 "Functor that compares the endpoints of an x-monotone curve.")
    .def("__call__", &Cmp_endpoints_xy::operator(),
         py::arg("xcv"),
         "Compare the endpoints of the x-monotone curve.")
    ;

  traits_c.def(py::init<>(),
                 "Construct a default non-caching segment traits object.")
    .def(py::init<const Gt&>(), py::arg("traits"),
         "Copy-construct a non-caching segment traits object.")
    .def("intersect_2_object", &Gt::intersect_2_object,
         aos2_doc::AosXMonotoneTraits_2_intersect_2_object)
    .def("split_2_object", &Gt::split_2_object,
         aos2_doc::AosXMonotoneTraits_2_split_2_object)
    .def("are_mergeable_2_object", &Gt::are_mergeable_2_object,
         aos2_doc::AosXMonotoneTraits_2_are_mergeable_2_object)
    .def("merge_2_object", &Gt::merge_2_object,
         aos2_doc::AosXMonotoneTraits_2_merge_2_object)
    .def("make_x_monotone_2_object", &Gt::make_x_monotone_2_object,
         aos2_doc::AosTraits_2_make_x_monotone_2_object)
    .def("construct_opposite_2_object", &Gt::construct_opposite_2_object,
         "Return the construct-opposite functor.")
    .def("compare_endpoints_xy_2_object", &Gt::compare_endpoints_xy_2_object,
         "Return the compare-endpoints functor.")
    .def("construct_curve_2_object", &Gt::construct_curve_2_object,
         aos2_doc::AosConstructCurveTraits_2_construct_curve_2_object)
    ;
}
