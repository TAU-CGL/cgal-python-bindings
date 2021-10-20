// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSTRAITS_2_HPP
#define CGALPY_EXPORT_AOSTRAITS_2_HPP

#include "CGALPY/aos_2_concepts/export_AosXMonotoneTraits_2.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"

template <typename T>
void export_Make_x_monotone_2_call_operator(typename T::Make_x_monotone_2 m,
                                            typename T::Curve_2& c,
                                            bp::list& res) {
  typedef typename T::X_monotone_curve_2 X_monotone_curve_2;
  typedef typename T::Point_2 Point_2;
  auto v = std::vector<boost::variant<Point_2, X_monotone_curve_2>>();
  m(c, std::back_inserter(v));
  for (auto o : v) {
    if (Point_2* point = boost::get<Point_2>(&o)) {
      res.append(*point);
    } else if (X_monotone_curve_2* curve = boost::get<X_monotone_curve_2>(&o)) {
      res.append(*curve);
    }
  }
}

template <typename T, typename RVP, typename C, typename Concepts>
void export_AosTraits_2(C c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  export_AosXMonotoneTraits_2<T, RVP>(c, concepts);

  // bp::class_<typename T::Curve_2>("Curve_2")
  //  .def(bp::init<>())
  //  .def(bp::init<Curve_2&>())
  //  ;
  auto& classes = concepts.m_traits_classes;

  classes.m_make_x_monotone_2 =
    &bp::class_<typename T::Make_x_monotone_2>("Make_x_monotone_2", bp::no_init)
    .def("__call__", &export_Make_x_monotone_2_call_operator<T>);

  c.def("make_x_monotone_2_object", &T::make_x_monotone_2_object);

  exported = true;
}

#endif
