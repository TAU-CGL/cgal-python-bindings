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

  typedef typename T::Curve_2                   Curve_2;
  typedef typename T::Make_x_monotone_2         Make_x_monotone_2;

  export_AosXMonotoneTraits_2<T, RVP>(c, concepts);

  bp::scope traits_scope(c);
  auto& classes = concepts.m_traits_classes;

  bp::handle<> cv_co(bp::objects::registered_class_object(bp::type_id<Curve_2>()));
  if (cv_co.get() != 0) traits_scope.attr("Curve_2") = cv_co;
  else {
    classes.m_curve_2 = new bp::class_<Curve_2>("Curve_2");
    classes.m_curve_2->def(bp::init<>());
    classes.m_curve_2->def(bp::init<Curve_2&>());
  }

  classes.m_make_x_monotone_2 =
    new bp::class_<Make_x_monotone_2>("Make_x_monotone_2", bp::no_init);
  classes.m_make_x_monotone_2->def("__call__", &export_Make_x_monotone_2_call_operator<T>);

  c.def("make_x_monotone_2_object", &T::make_x_monotone_2_object);

  exported = true;
}

#endif
