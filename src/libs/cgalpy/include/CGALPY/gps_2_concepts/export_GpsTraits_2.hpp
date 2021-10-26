// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_GPSTRAITS_2_HPP
#define CGALPY_EXPORT_GPSTRAITS_2_HPP

#include <boost/python.hpp>

#include "CGALPY/gps_2_concepts/Gps_traits_classes.hpp"
#include "CGALPY/add_class_object.hpp"

namespace bp = boost::python;

namespace bso2 {

template <typename T>
//! This is completely messed up. The constructor should return a Polygon_2.
// If it is just for initializing, then it's redundant!
void export_ctr_gp_2_op(typename T::Construct_polygon_2 m,
                        bp::list& curves, Polygon_2& pgn) {
  auto begin = bp::stl_input_iterator<typename T::X_monotone_curve_2>(curves);
  auto end = bp::stl_input_iterator<typename T::X_monotone_curve_2>();
  return m(begin, end, pgn);
}

template <typename T>
Polygon_with_holes_2
export_ctr_gpwh_2_op(typename T::Construct_general_polygon_with_holes_2 m,
                     const typename T::Polygon_2& boundary, bp::list& holes) {
  auto begin = bp::stl_input_iterator<typename T::Polygon_2>(holes);
  auto end = bp::stl_input_iterator<typename T::Polygon_2>();
  return m(boundary, begin, end);
}

template <typename T>
typename T::Polygon_2* init_from_list(bp::list& lst) {
  auto begin = bp::stl_input_iterator<typename T::X_monotone_curve_2>(lst);
  auto end = bp::stl_input_iterator<typename T::X_monotone_curve_2>();
  return new typename T::Polygon_2(begin, end);
}

template <typename T>
typename T::Polygon_2::Curve_iterator curves_begin(typename T::Polygon_2& p)
{ return p.curves_begin(); }

template <typename T>
typename T::Polygon_2::Curve_iterator curves_end(typename T::Polygon_2& p)
{ return p.curves_end(); }

template <typename T>
typename T::Polygon_with_holes_2*
init_polygon_with_holes_2(typename T::Polygon_2& p, bp::list& lst) {
  auto begin = bp::stl_input_iterator<typename T::Polygon_2>(lst);
  auto end = bp::stl_input_iterator<typename T::Polygon_2>();
  return new typename T::Polygon_with_holes_2(p, begin, end);
}

template <typename T>
typename T::Polygon_with_holes_2::Hole_iterator
holes_begin(typename T::Polygon_with_holes_2& p) { return p.holes_begin(); }

template <typename T>
typename T::Polygon_with_holes_2::Hole_iterator
holes_end(typename T::Polygon_with_holes_2& p) { return p.holes_end(); }

template <typename T>
typename T::Polygon_2& outer_boundary(typename T::Polygon_with_holes_2& p)
{ return p.outer_boundary(); }

}

template <typename T, typename C, typename Concepts>
void export_GpsTraits_2(C c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  typedef typename T::Polygon_2                         Polygon_2;
  typedef typename T::Polygon_with_holes_2              Polygon_with_holes_2;
  typedef typename T::Construct_polygon_2               Construct_polygon_2;
  typedef typename T::Construct_general_polygon_with_holes_2
    Construct_general_polygon_with_holes_2;
  typedef typename T::Construct_outer_boundary          Construct_outer_boundary;
  typedef typename T::Construct_holes                   Construct_holes;
  typedef typename T::Is_unbounded                      Is_unbounded;

  bp::scope traits_scope(c);
  auto& classes = concepts.m_traits_classes;

  // Polygon_2
  static const char polygon_2[] = "Polygon_2";
  if (add_class_object<Polygon_2, polygon_2>(traits_scope, classes.m_polygon_2)) {
    classes.m_polygon_2->def(bp::init<>());
    classes.m_polygon_2->def(bp::init<Polygon_2>());
    classes.m_polygon_2->def("__init__", make_constructor(&bso2::init_from_list<T>));
    classes.m_polygon_2->def("push_back", &Polygon_2::push_back);
    classes.m_polygon_2->def("orientation", &Polygon_2::orientation);
    classes.m_polygon_2->def("is_empty", &Polygon_2::is_empty);
    classes.m_polygon_2->def("size", &Polygon_2::size);
    classes.m_polygon_2->def("bbox", &Polygon_2::bbox);
    classes.m_polygon_2->def("curves", bp::range<bp::return_internal_reference<>>
                             (&bso2::curves_begin<T>, &bso2::curves_end<T>));
    classes.m_polygon_2->def("clear", &Polygon_2::clear);
    classes.m_polygon_2->def("reverse_orientation", &Polygon_2::reverse_orientation);
    classes.m_polygon_2->def(bp::self_ns::str(bp::self_ns::self));
    classes.m_polygon_2->def(bp::self_ns::repr(bp::self_ns::self));
  }

  // Polygon_with_holes_2
  static const char polygon_with_holes_2[] = "Polygon_with_holes_2";
  if (add_class_object<Polygon_with_holes_2, polygon_with_holes_2>
      (traits_scope, classes.m_polygon_with_holes_2)) {
    classes.m_polygon_with_holes_2->def(bp::init<Polygon_2&>());
    classes.m_polygon_with_holes_2->def("__init__", make_constructor(&bso2::init_polygon_with_holes_2<T>));
    classes.m_polygon_with_holes_2->def("is_unbounded", &Polygon_with_holes_2::is_unbounded);
    classes.m_polygon_with_holes_2->def("outer_boundary", &bso2::outer_boundary<T>, bp::return_internal_reference<>());
    classes.m_polygon_with_holes_2->def("holes", bp::range<bp::return_internal_reference<>>
                                        (&bso2::holes_begin<T>, &bso2::holes_end<T>));
    classes.m_polygon_with_holes_2->def("number_of_holes", &Polygon_with_holes_2::number_of_holes);
    classes.m_polygon_with_holes_2->def(bp::self_ns::str(bp::self_ns::self));
    classes.m_polygon_with_holes_2->def(bp::self_ns::repr(bp::self_ns::self));
  }

  // Construct_polygon_2
  classes.m_construct_polygon_2 =
    new bp::class_<Construct_polygon_2>("Construct_polygon_2", bp::no_init);
  classes.m_construct_polygon_2->def("__call__", &bso2::export_ctr_gp_2_op<T>);

  // Construct_general_polygon_with_holes_2
  classes.m_construct_general_polygon_with_holes_2 =
    new bp::class_<Construct_general_polygon_with_holes_2>
    ("Construct_general_polygon_with_holes_2", bp::no_init);
  using Ctr_gpwh1 =
    Polygon_with_holes_2(Construct_general_polygon_with_holes_2::*)
    (const Polygon_2&) const;
  classes.m_construct_general_polygon_with_holes_2->
    def("__call__", static_cast<Ctr_gpwh1>
        (&Construct_general_polygon_with_holes_2::operator()));
  classes.m_construct_general_polygon_with_holes_2->
    def("__call__", &bso2::export_ctr_gpwh_2_op<T>);

  // Construct_outer_boundary
  classes.m_construct_outer_boundary =
    new bp::class_<Construct_outer_boundary>
    ("Construct_outer_boundary", bp::no_init);
  classes.m_construct_outer_boundary->
    def("__call__", &Construct_outer_boundary::operator());

  // Construct_holes
  classes.m_construct_holes =
    new bp::class_<Construct_holes>("Construct_holes", bp::no_init);
  classes.m_construct_holes->def("__call__", &Construct_holes::operator());

  // Is unbounded
  classes.m_is_unbounded =
    new bp::class_<Is_unbounded>("Is_unbounded", bp::no_init);
  classes.m_is_unbounded->def("__call__", &Is_unbounded::operator());

  // The following is a typo. Should be instead
  // c.def("is_unbounded_object", &T::is_unbounded_object);
  c.def("is_unbounded_object", &T::construct_is_unbounded_object);

  exported = true;
}

#endif
