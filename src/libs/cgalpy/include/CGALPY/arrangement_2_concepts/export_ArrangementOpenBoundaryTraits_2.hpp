// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_ARRANGEMENTOPENBOUNDARYTRAITS_2_HPP
#define CGALPY_EXPORT_ARRANGEMENTOPENBOUNDARYTRAITS_2_HPP

#include "export_ArrangementBasicTraits.hpp"

CGAL::Tag_false approach_side(CGAL::Arr_oblivious_side_tag, CGAL::Arr_oblivious_side_tag) {
  return CGAL::Tag_false();
}

CGAL::Tag_true approach_side(CGAL::Arr_open_side_tag, CGAL::Arr_oblivious_side_tag) {
  return CGAL::Tag_true();
}

CGAL::Tag_true approach_side(CGAL::Arr_oblivious_side_tag, CGAL::Arr_open_side_tag) {
  return CGAL::Tag_true();
}

CGAL::Tag_true approach_side(CGAL::Arr_open_side_tag,
                                      CGAL::Arr_open_side_tag) {
  return CGAL::Tag_true();
}

template<typename T, typename C>
void export_Compare_x_near_limit_2(C c, CGAL::Tag_true) {
  bp::class_<typename T::Compare_x_near_limit_2>("Compare_x_near_limit_2",
                                                bp::no_init)
    .def("__call__", &T::Compare_x_near_limit_2::operator())
    ;
  c.def("compare_x_near_limit_2_object", &T::compare_x_near_limit_2_object);
}

template<typename T, typename C>
void export_Compare_x_near_limit_2(C c, CGAL::Tag_false) {}

template<typename T, typename C>
void export_Compare_x_at_limit_2(C c, CGAL::Tag_true) {
  bp::class_<typename T::Compare_x_at_limit_2>("Compare_x_at_limit_2",
                                                bp::no_init)
  .def<CGAL::Comparison_result (T::Compare_x_at_limit_2::*)
    (const typename T::X_monotone_curve_2&, CGAL::Arr_curve_end,
    const typename T::X_monotone_curve_2&, CGAL::Arr_curve_end) const>
    ("__call__", &T::Compare_x_at_limit_2::operator())
  .def<CGAL::Comparison_result (T::Compare_x_at_limit_2::*)
    (const typename T::Point_2&, const typename T::X_monotone_curve_2&,
      CGAL::Arr_curve_end) const>
  ("__call__", &T::Compare_x_at_limit_2::operator())
    ;
  c.def("compare_x_at_limit_2_object", &T::compare_x_at_limit_2_object);
}

template<typename T, typename C>
void export_Compare_x_at_limit_2(C c, CGAL::Tag_false) {}

template<typename T, typename C>
void export_Compare_y_near_boundary_2(C c, CGAL::Tag_true) {
  bp::class_<typename T::Compare_y_near_boundary_2>("Compare_y_near_boundary_2",
                                                 bp::no_init)
    .def("__call__", &T::Compare_y_near_boundary_2::operator())
    ;
  c.def("compare_y_near_boundary_2_object", &T::compare_y_near_boundary_2_object);
}

template<typename T, typename C>
void export_Compare_y_near_boundary_2(C c, CGAL::Tag_false) {}

template<typename T, typename C>
void export_Parameter_space_in_x_2(C c, CGAL::Tag_true) {
  bp::class_<typename T::Parameter_space_in_x_2>("Parameter_space_in_x_2",
                                                    bp::no_init)
    .def<CGAL::Arr_parameter_space (T::Parameter_space_in_x_2::*)
    (const T::X_monotone_curve_2&, CGAL::Arr_curve_end) const>
    ("__call__", &T::Parameter_space_in_x_2::operator())
    ;
  c.def("parameter_space_in_x_2_object", &T::parameter_space_in_x_2_object);
}

template<typename T, typename C>
void export_Parameter_space_in_x_2(C c, CGAL::Tag_false) {}

template<typename T, typename C>
void export_Parameter_space_in_y_2(C c, CGAL::Tag_true) {
  bp::class_<typename T::Parameter_space_in_y_2>("Parameter_space_in_y_2",
                                                 bp::no_init)
    .def<CGAL::Arr_parameter_space (T::Parameter_space_in_y_2::*)
    (const T::X_monotone_curve_2&, CGAL::Arr_curve_end) const>
  ("__call__", &T::Parameter_space_in_y_2::operator())
    ;
  c.def("parameter_space_in_y_2_object", &T::parameter_space_in_y_2_object);
}

template<typename T, typename C>
void export_Parameter_space_in_y_2(C c, CGAL::Tag_false) {}

template <typename T, typename RVP, typename C>
void export_ArrangementOpenBoundaryTraits_2 (C c) {
  static bool exported = false;
  if (exported) return;

  export_ArrangementBasicTraits<T, RVP>(c);
  export_Compare_x_at_limit_2<T>(c, approach_side(T::Bottom_side_category(),
                                                  T::Top_side_category()));
  export_Compare_x_near_limit_2<T>(c, approach_side(T::Bottom_side_category(),
                                                    T::Top_side_category()));
  export_Compare_y_near_boundary_2<T>(c, approach_side(T::Left_side_category(),
                                                    T::Right_side_category()));
  export_Parameter_space_in_x_2<T>(c, approach_side(T::Left_side_category(),
                                                    T::Right_side_category()));
  export_Parameter_space_in_y_2<T>(c, approach_side(T::Left_side_category(),
                                                    T::Right_side_category()));

  exported = true;
}

#endif //CGALPY_EXPORT_ARRANGEMENTOPENBOUNDARYTRAITS_2_HPP
