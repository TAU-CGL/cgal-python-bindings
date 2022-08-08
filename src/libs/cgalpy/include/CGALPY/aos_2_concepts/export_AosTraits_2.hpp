// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSTRAITS_2_HPP
#define CGALPY_EXPORT_AOSTRAITS_2_HPP

#include <boost/iterator/function_output_iterator.hpp>

#include "CGALPY/aos_2_concepts/export_AosXMonotoneTraits_2.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/add_class_object.hpp"

//! Apply the make_x_monotone operator and append the resulting X-monotone
// elements to the returned Python list.
template <typename T>
bp::list export_Make_x_monotone_2_call_operator(typename T::Make_x_monotone_2 m,
                                                typename T::Curve_2& c) {
  typedef typename T::X_monotone_curve_2                X_monotone_curve_2;
  typedef typename T::Point_2                           Point_2;
  typedef boost::variant<Point_2, X_monotone_curve_2>   Result;
  bp::list lst;
  auto op =
    [&] (const Result& o) mutable {
      if (auto* point = boost::get<Point_2>(&o)) lst.append(*point);
      else if (auto* cv = boost::get<X_monotone_curve_2>(&o)) lst.append(*cv);
    };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  m(c, it);
  return lst;
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

  static const char curve_2[] = "Curve_2";
  add_class_object<Curve_2, curve_2>(traits_scope, classes.m_curve_2);

  classes.m_make_x_monotone_2 =
    new bp::class_<Make_x_monotone_2>("Make_x_monotone_2", bp::no_init);
  classes.m_make_x_monotone_2->def("__call__", &export_Make_x_monotone_2_call_operator<T>);

  c.def("make_x_monotone_2_object", &T::make_x_monotone_2_object);

  exported = true;
}

#endif
