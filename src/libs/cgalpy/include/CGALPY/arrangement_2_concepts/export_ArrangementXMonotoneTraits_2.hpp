// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_ARRANGEMENTXMONOTONETRAITS_2_HPP
#define CGALPY_EXPORT_ARRANGEMENTXMONOTONETRAITS_2_HPP

#include "export_ArrangementBasicTraits.hpp"

template <typename T>
void intersect_2_call_operator(typename T::Intersect_2& i,
                               typename T::X_monotone_curve_2& xc1,
                               typename T::X_monotone_curve_2& xc2,
                               bp::list& res) {
  typedef typename T::Point_2                   Point_2;
  typedef typename T::X_monotone_curve_2        X_monotone_curve_2;
  typedef typename T::Multiplicity              Multiplicity;

  typedef std::pair<Point_2, Multiplicity> pair;
  auto v = std::vector<boost::variant<pair, X_monotone_curve_2>>();
  i(xc1, xc2, std::back_inserter(v));
  for (auto o : v) {
    if (pair* pa = boost::get<pair>(&o)) {
      bp::tuple tup = bp::make_tuple(pa->first, pa->second);
      res.append(tup);
    } else if (X_monotone_curve_2* curve = boost::get<X_monotone_curve_2>(&o)){
      res.append(*curve);
    }
  }
}

template<typename T, typename C>
void export_Merge_2(C c, CGAL::Tag_true) {
  bp::class_<typename T::Merge_2>("Merge_2", bp::no_init)
    .def("__call__", &T::Merge_2::operator())
    ;
  c.def("merge_2_object", &T::merge_2_object);
}

template<typename T, typename C>
void export_Merge_2(C c, CGAL::Tag_false) {}

template<typename T, typename C>
void export_Are_mergeable_2(C c, CGAL::Tag_true) {
  typedef typename T::Are_mergeable_2   Are_mergeable_2;
  bp::class_<typename T::Are_mergeable_2>("Are_mergeable_2", bp::no_init)
    .def("__call__", &Are_mergeable_2::operator())
    ;
  c.def("are_mergeable_2_object", &T::are_mergeable_2_object);
}

template<typename T, typename C>
void export_Are_mergeable_2(C c, CGAL::Tag_false) {}

template <typename T, typename RVP, typename C>
void export_ArrangementXMonotoneTraits_2(C c) {
  static bool exported = false;
  if (exported) return;

  export_ArrangementBasicTraits<T, RVP>(c);

  typedef typename T::Has_merge_category        Has_merge_category;
  typedef typename T::Intersect_2               Intersect_2;
  typedef typename T::Split_2                   Split_2;

  bp::class_<Intersect_2>("Intersect_2", bp::no_init)
    .def("__call__", &intersect_2_call_operator<T>)
    ;
  bp::class_<Split_2>("Split_2", bp::no_init)
    .def("__call__", &Split_2::operator())
    ;

  c.def("intersect_2_object", &T::intersect_2_object);
  c.def("split_2_object", &T::split_2_object);

  export_Are_mergeable_2<T>(c, Has_merge_category());
  export_Merge_2<T>(c, Has_merge_category());

  exported = true;
}

#endif //CGALPY_EXPORT_ARRANGEMENTXMONOTONETRAITS_2_HPP
