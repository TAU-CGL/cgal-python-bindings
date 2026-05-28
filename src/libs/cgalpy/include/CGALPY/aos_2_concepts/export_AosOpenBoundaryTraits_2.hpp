// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSOPENBOUNDARYTRAITS_2_HPP
#define CGALPY_EXPORT_AOSOPENBOUNDARYTRAITS_2_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/aos_2_concepts/export_AosBasicTraits_2.hpp"

#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_open_boundary_doc = cgalpy::aos2::docstrings;

//
template <typename T1, typename T2>
struct approach_side {
  using type = CGAL::Tag_false;
};

//
template<>
struct approach_side<CGAL::Arr_open_side_tag, CGAL::Arr_oblivious_side_tag> {
  using type = CGAL::Tag_true;
};

//
template<>
struct approach_side<CGAL::Arr_oblivious_side_tag, CGAL::Arr_open_side_tag> {
  using type = CGAL::Tag_true;
};

//
template<>
struct approach_side<CGAL::Arr_open_side_tag, CGAL::Arr_open_side_tag> {
  using type = CGAL::Tag_true;
};

//
template<typename T, typename C, typename Classes>
void export_Compare_x_near_boundary_2(C& c, Classes& classes, CGAL::Tag_true) {
  using Cmp_x_near_bnd = typename T::Compare_x_near_boundary_2;
  classes.m_compare_x_near_boundary_2 =
    new py::class_<Cmp_x_near_bnd>(
      c, "Compare_x_near_boundary_2",
      aos2_open_boundary_doc::AosOpenBoundaryTraits_2_Compare_x_near_boundary_2);
  classes.m_compare_x_near_boundary_2->def("__call__",
                                           &Cmp_x_near_bnd::operator(),
                                           py::arg("xcv1"), py::arg("xcv2"),
                                           py::arg("curve_end"),
                                           aos2_open_boundary_doc::AosTraits_CompareXNearBoundary_2_operator_call);
  c.def("compare_x_near_boundary_2_object",
        &T::compare_x_near_boundary_2_object,
        aos2_open_boundary_doc::AosOpenBoundaryTraits_2_compare_x_near_boundary_2_object);
}

//
template<typename T, typename C, typename Classes>
void export_Compare_x_near_boundary_2(C& c, Classes& classes, CGAL::Tag_false) {}

template<typename T, typename C, typename Classes>
void export_Compare_x_on_boundary_2(C& c, Classes& classes, CGAL::Tag_true) {
  using Pnt = typename T::Point_2;
  using Xcv = typename T::X_monotone_curve_2;
  using Cmp_x_on_bnd = typename T::Compare_x_on_boundary_2;

  classes.m_compare_x_on_boundary_2 =
    new py::class_<Cmp_x_on_bnd>(
      c, "Compare_x_on_boundary_2",
      aos2_open_boundary_doc::AosOpenBoundaryTraits_2_Compare_x_on_boundary_2);
  using overload1 = CGAL::Comparison_result(Cmp_x_on_bnd::*)
    (const Xcv&, CGAL::Arr_curve_end, const Xcv&, CGAL::Arr_curve_end) const;
  using overload2 = CGAL::Comparison_result (Cmp_x_on_bnd::*)
    (const Pnt&, const Xcv&, CGAL::Arr_curve_end) const;

  auto cmp_x_on_bnd_c = *(classes.m_compare_x_on_boundary_2);
  cmp_x_on_bnd_c.def("__call__",
                     static_cast<overload1>(&Cmp_x_on_bnd::operator()),
                     py::arg("xcv1"), py::arg("curve_end1"),
                     py::arg("xcv2"), py::arg("curve_end2"),
                     aos2_open_boundary_doc::AosTraits_CompareXOnBoundaryOfCurveEnd_2_operator_call_1);
  cmp_x_on_bnd_c.def("__call__",
                     static_cast<overload2>(&Cmp_x_on_bnd::operator()),
                     py::arg("point"), py::arg("xcv"), py::arg("curve_end"),
                     aos2_open_boundary_doc::AosTraits_CompareXOnBoundaryOfCurveEnd_2_operator_call);
  c.def("compare_x_on_boundary_2_object", &T::compare_x_on_boundary_2_object,
        aos2_open_boundary_doc::AosOpenBoundaryTraits_2_compare_x_on_boundary_2_object);
}

//
template<typename T, typename C, typename Classes>
void export_Compare_x_on_boundary_2(C& c, Classes& classes, CGAL::Tag_false) {}

//
template<typename T, typename C, typename Classes>
void export_Compare_y_near_boundary_2(C& c, Classes& classes, CGAL::Tag_true) {
  using Cmp_y_near_bnd = typename T::Compare_y_near_boundary_2;

  classes.m_compare_y_near_boundary_2 =
    new py::class_<Cmp_y_near_bnd>(
      c, "Compare_y_near_boundary_2",
      aos2_open_boundary_doc::AosOpenBoundaryTraits_2_Compare_y_near_boundary_2);
  auto cmp_y_near_bnd_c = *(classes.m_compare_y_near_boundary_2);
  cmp_y_near_bnd_c.def("__call__", &Cmp_y_near_bnd::operator(),
                         py::arg("xcv1"), py::arg("xcv2"),
                         py::arg("curve_end"),
                         aos2_open_boundary_doc::AosTraits_CompareYNearBoundary_2_operator_call);
  c.def("compare_y_near_boundary_2_object",
        &T::compare_y_near_boundary_2_object,
        aos2_open_boundary_doc::AosOpenBoundaryTraits_2_compare_y_near_boundary_2_object);
}

//
template<typename T, typename C, typename Classes>
void export_Compare_y_near_boundary_2(C& c, Classes& classes, CGAL::Tag_false) {}

//
template<typename T, typename C, typename Classes>
void export_Parameter_space_in_x_2(C& c, Classes& classes, CGAL::Tag_true) {
  using overload = CGAL::Arr_parameter_space (T::Parameter_space_in_x_2::*)
    (const typename T::X_monotone_curve_2&, CGAL::Arr_curve_end) const;
  classes.m_parameter_space_in_x_2 =
    new py::class_<typename T::Parameter_space_in_x_2>(
      c, "Parameter_space_in_x_2",
      aos2_open_boundary_doc::AosOpenBoundaryTraits_2_Parameter_space_in_x_2);
  classes.m_parameter_space_in_x_2->def("__call__",
                                         static_cast<overload>(&T::Parameter_space_in_x_2::operator()),
                                         py::arg("xcv"), py::arg("curve_end"),
                                         aos2_open_boundary_doc::AosTraits_ParameterSpaceInX_2_operator_call);
  c.def("parameter_space_in_x_2_object", &T::parameter_space_in_x_2_object,
        aos2_open_boundary_doc::AosOpenBoundaryTraits_2_parameter_space_in_x_2_object);
}

//
template<typename T, typename C, typename Classes>
void export_Parameter_space_in_x_2(C& c, Classes& classes, CGAL::Tag_false) {}

//
template<typename T, typename C, typename Classes>
void export_Parameter_space_in_y_2(C& c, Classes& classes, CGAL::Tag_true) {
  using overload = CGAL::Arr_parameter_space (T::Parameter_space_in_y_2::*)
    (const typename T::X_monotone_curve_2&, CGAL::Arr_curve_end) const;
  using Prm_space_in_y_2 = typename T::Parameter_space_in_y_2;

  classes.m_parameter_space_in_y_2 =
    new py::class_<Prm_space_in_y_2>(
      c, "Parameter_space_in_y_2",
      aos2_open_boundary_doc::AosOpenBoundaryTraits_2_Parameter_space_in_y_2);
  classes.m_parameter_space_in_y_2->def("__call__",
                                         static_cast<overload>(&Prm_space_in_y_2::operator()),
                                         py::arg("xcv"), py::arg("curve_end"),
                                         aos2_open_boundary_doc::AosTraits_ParameterSpaceInY_2_operator_call);
  c.def("parameter_space_in_y_2_object", &T::parameter_space_in_y_2_object,
        aos2_open_boundary_doc::AosOpenBoundaryTraits_2_parameter_space_in_y_2_object);
}

//
template<typename T, typename C, typename Classes>
void export_Parameter_space_in_y_2(C& c, Classes& classes, CGAL::Tag_false) {}

//
template <typename T, typename C, typename Concepts>
void export_AosOpenBoundaryTraits_2 (C& c, Concepts& concepts) {
  using Bottom_side_category = typename T::Bottom_side_category;
  using Top_side_category = typename T::Top_side_category;
  using Bs_cat = Bottom_side_category;
  using Ts_cat = Top_side_category;
  using Bt_approach_side = typename approach_side<Bs_cat, Ts_cat>::type;

  static bool exported = false;
  if (exported) return;

  auto& classes = concepts.m_aos_open_boundary_traits_2_classes;

  export_AosBasicTraits_2<T>(c, concepts);
  export_Compare_x_on_boundary_2<T>(c, classes, Bt_approach_side());
  export_Compare_x_near_boundary_2<T>(c, classes, Bt_approach_side());
  export_Compare_y_near_boundary_2<T>(c, classes, Bt_approach_side());
  export_Parameter_space_in_x_2<T>(c, classes, Bt_approach_side());
  export_Parameter_space_in_y_2<T>(c, classes, Bt_approach_side());

  exported = true;
}

#endif
