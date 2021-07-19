//
// Created by Nir on 12-Jul-21.
//

#ifndef CGALPY_EXPORT_ARRANGEMENTTRAITS_2_HPP
#define CGALPY_EXPORT_ARRANGEMENTTRAITS_2_HPP

#include "export_ArrangementXMonotoneTraits_2.hpp"

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

template <typename T, typename RVP, typename C>
void export_ArrangementTraits_2(C c) {
  static bool exported = false;
  if (exported) return;

  export_ArrangementXMonotoneTraits_2<T, RVP>(c);
//    bp::class_<typename T::Curve_2>("Curve_2")
//    .def(bp::init<>())
//    .def(bp::init<Curve_2&>())
//    ;
  bp::class_<typename T::Make_x_monotone_2>("Make_x_monotone_2", bp::no_init)
    .def("__call__", &export_Make_x_monotone_2_call_operator<T>);

  c.def("make_x_monotone_2_object", &T::make_x_monotone_2_object);

  exported = true;
}

#endif //CGALPY_EXPORT_ARRANGEMENTTRAITS_2_HPP
