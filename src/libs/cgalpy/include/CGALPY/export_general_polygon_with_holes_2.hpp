// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#ifndef CGALPY_EXPORT_GENERAL_POLYGON_WITH_HOLES_2_HPP
#define CGALPY_EXPORT_GENERAL_POLYGON_WITH_HOLES_2_HPP

#include <type_traits>

#include <nanobind/nanobind.h>

#include "CGALPY/if_.hpp"
#include "CGALPY/add_class_object.hpp"

namespace py = nanobind;

template <typename T, typename = void>
struct target { typedef typename T::General_polygon_2 type; };

template <typename T>
struct target<T, typename if_<false, typename T::Polygon_2>::type> {
  typedef typename T::Polygon_2         type;
};

template <typename GeneralPolygonWithHoles_2>
GeneralPolygonWithHoles_2*
ctr_polygon_with_holes_2(typename target<GeneralPolygonWithHoles_2>::type& p,
                         py::list& lst) {
  typedef typename target<GeneralPolygonWithHoles_2>::type   Polygon_2;
  auto begin = py::stl_input_iterator<Polygon_2>(lst);
  auto end = py::stl_input_iterator<Polygon_2>();
  return new GeneralPolygonWithHoles_2(p, begin, end);
}

template <typename GeneralPolygonWithHoles_2>
typename GeneralPolygonWithHoles_2::Hole_iterator
holes_begin(GeneralPolygonWithHoles_2& p) { return p.holes_begin(); }

template <typename GeneralPolygonWithHoles_2>
typename GeneralPolygonWithHoles_2::Hole_iterator
holes_end(GeneralPolygonWithHoles_2& p) { return p.holes_end(); }

template <typename GeneralPolygonWithHoles_2>
typename target<GeneralPolygonWithHoles_2>::type&
outer_boundary(GeneralPolygonWithHoles_2& p) { return p.outer_boundary(); }

template <typename Type, const char* Name>
void export_general_polygon_with_holes_2(py::scope& my_scope,
                                         py::class_<Type>*& co) {
  typedef typename target<Type>::type   Polygon_2;

  if (! add_class_object<Type, Name>(my_scope, co)) return;

  co->def(py::init<Polygon_2&>());
  co->def("__init__", make_constructor(&ctr_polygon_with_holes_2<Type>));
  co->def("is_unbounded", &Type::is_unbounded);
  co->def("outer_boundary", &outer_boundary<Type>,
          py::return_internal_reference<>());
  co->def("holes", py::range<py::return_internal_reference<>>
          (&holes_begin<Type>, &holes_end<Type>));
  co->def("number_of_holes", &Type::number_of_holes);
  co->def(py::self_ns::str(py::self_ns::self));
  co->def(py::self_ns::repr(py::self_ns::self));
}

#endif
