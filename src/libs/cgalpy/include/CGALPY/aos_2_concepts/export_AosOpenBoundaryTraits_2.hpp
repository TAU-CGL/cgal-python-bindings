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

#include "CGALPY/aos_2_concepts/export_AosBasicTraits.hpp"

namespace py = nanobind;

template <typename T1, typename T2>
struct approach_side {
  typedef CGAL::Tag_false type;
};

template<>
struct approach_side<CGAL::Arr_open_side_tag, CGAL::Arr_oblivious_side_tag> {
  typedef CGAL::Tag_true type;
};

template<>
struct approach_side<CGAL::Arr_oblivious_side_tag, CGAL::Arr_open_side_tag> {
  typedef CGAL::Tag_true type;
};

template<>
struct approach_side<CGAL::Arr_open_side_tag, CGAL::Arr_open_side_tag> {
  typedef CGAL::Tag_true type;
};

template<typename T, typename C, typename Classes>
void export_Compare_x_near_limit_2(C c, Classes& classes, CGAL::Tag_true) {
  classes.m_compare_x_near_limit_2 = new py::class_<typename T::Compare_x_near_limit_2>(c, "Compare_x_near_boundary_2");
  classes.m_compare_x_near_limit_2->def("__call__", &T::Compare_x_near_limit_2::operator());
  c.def("compare_x_near_limit_2_object", &T::compare_x_near_limit_2_object);
}

template<typename T, typename C, typename Classes>
void export_Compare_x_near_limit_2(C c, Classes& classes, CGAL::Tag_false) {}

template<typename T, typename C, typename Classes>
void export_Compare_x_at_limit_2(C c, Classes& classes, CGAL::Tag_true) {
  classes.m_compare_x_at_limit_2 =
    new py::class_<typename T::Compare_x_at_limit_2>(c, "Compare_x_at_limit_2");
  using overload1 = CGAL::Comparison_result(T::Compare_x_at_limit_2::*)
    (const typename T::X_monotone_curve_2&, CGAL::Arr_curve_end,
    const typename T::X_monotone_curve_2&, CGAL::Arr_curve_end) const;
  using overload2 = CGAL::Comparison_result (T::Compare_x_at_limit_2::*)
    (const typename T::Point_2&, const typename T::X_monotone_curve_2&,
      CGAL::Arr_curve_end) const;
  classes.m_compare_x_at_limit_2->def("__call__", static_cast<overload1>(&T::Compare_x_at_limit_2::operator()));
  classes.m_compare_x_at_limit_2->def("__call__", static_cast<overload2>(&T::Compare_x_at_limit_2::operator()));
  c.def("compare_x_at_limit_2_object", &T::compare_x_at_limit_2_object);
}

template<typename T, typename C, typename Classes>
void export_Compare_x_at_limit_2(C c, Classes& classes, CGAL::Tag_false) {}

template<typename T, typename C, typename Classes>
void export_Compare_y_near_boundary_2(C c, Classes& classes, CGAL::Tag_true) {
  classes.m_compare_y_near_boundary_2 =
    new py::class_<typename T::Compare_y_near_boundary_2>(c, "Compare_y_near_boundary_2");
  classes.m_compare_y_near_boundary_2->def("__call__", &T::Compare_y_near_boundary_2::operator());
  c.def("compare_y_near_boundary_2_object", &T::compare_y_near_boundary_2_object);
}

template<typename T, typename C, typename Classes>
void export_Compare_y_near_boundary_2(C c, Classes& classes, CGAL::Tag_false) {}

template<typename T, typename C, typename Classes>
void export_Parameter_space_in_x_2(C c, Classes& classes, CGAL::Tag_true) {
  using overload = CGAL::Arr_parameter_space (T::Parameter_space_in_x_2::*)
    (const typename T::X_monotone_curve_2&, CGAL::Arr_curve_end) const;
  classes.m_parameter_space_in_x_2 =
    new py::class_<typename T::Parameter_space_in_x_2>(c, "Parameter_space_in_x_2");
  classes.m_parameter_space_in_x_2->def("__call__", static_cast<overload>(&T::Parameter_space_in_x_2::operator()));
  c.def("parameter_space_in_x_2_object", &T::parameter_space_in_x_2_object);
}

template<typename T, typename C, typename Classes>
void export_Parameter_space_in_x_2(C c, Classes& classes, CGAL::Tag_false) {}

template<typename T, typename C, typename Classes>
void export_Parameter_space_in_y_2(C c, Classes& classes, CGAL::Tag_true) {
  using overload = CGAL::Arr_parameter_space (T::Parameter_space_in_y_2::*)
    (const typename T::X_monotone_curve_2&, CGAL::Arr_curve_end) const;
  classes.m_parameter_space_in_y_2 =
    new py::class_<typename T::Parameter_space_in_y_2>(c, "Parameter_space_in_y_2");
  classes.m_parameter_space_in_y_2->def("__call__", static_cast<overload>(&T::Parameter_space_in_y_2::operator()));
  c.def("parameter_space_in_y_2_object", &T::parameter_space_in_y_2_object);
}

template<typename T, typename C, typename Classes>
void export_Parameter_space_in_y_2(C c, Classes& classes, CGAL::Tag_false) {}

template <typename T, typename C, typename Concepts>
void export_AosOpenBoundaryTraits_2 (C c, Concepts& concepts) {
  typedef typename T::Bottom_side_category Bottom_side_category;
  typedef typename T::Top_side_category Top_side_category;
  static bool exported = false;
  if (exported) return;

  auto& classes = concepts.m_open_boundary_traits_classes;

  export_AosBasicTraits<T>(c, concepts);
  export_Compare_x_at_limit_2<T>(c, classes,
                                 typename approach_side<Bottom_side_category,
                                 Top_side_category>::type());
  export_Compare_x_near_limit_2<T>(c, classes,
                                   typename approach_side<Bottom_side_category,
                                   Top_side_category>::type());
  export_Compare_y_near_boundary_2<T>(c, classes,
                                      typename approach_side<Bottom_side_category,
                                      Top_side_category>::type());
  export_Parameter_space_in_x_2<T>(c, classes,
                                   typename approach_side<Bottom_side_category,
                                   Top_side_category>::type());
  export_Parameter_space_in_y_2<T>(c, classes,
                                   typename approach_side<Bottom_side_category,
                                   Top_side_category>::type());

  exported = true;
}

#endif
