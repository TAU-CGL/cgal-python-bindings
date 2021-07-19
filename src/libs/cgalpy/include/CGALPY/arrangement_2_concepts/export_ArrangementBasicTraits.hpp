// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_ARRANGEMENTBASICTRAITS_HPP
#define CGALPY_EXPORT_ARRANGEMENTBASICTRAITS_HPP

template<typename T, typename C>
void export_Compare_at_x_left_2(C c, CGAL::Tag_true) {
  bp::class_<typename T::Compare_y_at_x_left_2>("Compare_y_at_x_left_2",
                                                bp::no_init)
    .def("__call__", &T::Compare_y_at_x_left_2::operator())
    ;
  c.def("compare_y_at_x_left_2_object", &T::compare_y_at_x_left_2_object);
}

template<typename T, typename C>
void export_Compare_at_x_left_2(C c, CGAL::Tag_false) {}

template <typename T, typename RVP, typename C>
void export_ArrangementBasicTraits(C c) {
  static bool exported = false;
  if (exported) return;

//  bp::class_<typename T::Point_2>("Point_2")
//    .def(bp::init<>())
//    .def(bp::init<Point_2&>())
//    ;
//  bp::class_<typename T::X_monotone_curve_2>("X_monotone_curve_2")
//    .def(bp::init<>())
//    .def(bp::init<X_monotone_curve_2&>())
//    ;
  bp::class_<typename T::Compare_x_2>("Compare_x_2", bp::no_init)
    .def("__call__", &T::Compare_x_2::operator())
    ;
  bp::class_<typename T::Compare_xy_2>("Compare_xy_2", bp::no_init)
    .def("__call__", &T::Compare_xy_2::operator())
    ;
  bp::class_<typename T::Construct_min_vertex_2>("Construct_min_vertex_2",
                                                 bp::no_init)
    .def("__call__", &T::Construct_min_vertex_2::operator(), RVP())
    ;
  bp::class_<typename T::Construct_max_vertex_2>("Construct_max_vertex_2",
                                                 bp::no_init)
    .def("__call__", &T::Construct_max_vertex_2::operator(), RVP())
    ;
  bp::class_<typename T::Is_vertical_2>("Is_vertical_2", bp::no_init)
    .def("__call__", &T::Is_vertical_2::operator())
    ;
  bp::class_<typename T::Compare_y_at_x_2>("Compare_y_at_x_2", bp::no_init)
    .def("__call__", &T::Compare_y_at_x_2::operator())
    ;
  bp::class_<typename T::Compare_y_at_x_right_2>("Compare_y_at_x_right_2",
                                           bp::no_init)
    .def("__call__", &T::Compare_y_at_x_right_2::operator())
    ;
  bp::class_<typename T::Equal_2>("Equal_2", bp::no_init)
    .def<bool (T::Equal_2::*) (const T::Point_2&, const T::Point_2&) const>
  ("__call__", &T::Equal_2::operator())
    .def<bool (T::Equal_2::*) (const T::X_monotone_curve_2&,
      const T::X_monotone_curve_2&) const> ("__call__", &T::Equal_2::operator())
    ;
  c.def(bp::init<>())
    .def(bp::init<T&>())
    .def("compare_x_2_object", &T::compare_x_2_object)
    .def("compare_xy_2_object", &T::compare_xy_2_object)
    .def("construct_min_vertex_2_object", &T::construct_min_vertex_2_object)
    .def("construct_max_vertex_2_object", &T::construct_max_vertex_2_object)
    .def("is_vertical_2_object", &T::is_vertical_2_object)
    .def("compare_y_at_x_2_object", &T::compare_y_at_x_2_object)
    .def("compare_y_at_x_right_2_object", &T::compare_y_at_x_right_2_object)
    .def("equal_2_object", &T::equal_2_object)
    ;
  export_Compare_at_x_left_2<T>(c, T::Has_left_category());

  exported = true;
}

#endif //CGALPY_EXPORT_ARRANGEMENTBASICTRAITS_HPP
