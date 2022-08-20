// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_GENERAL_POLYGON_WITH_HOLES_2_HPP
#define CGALPY_EXPORT_GENERAL_POLYGON_WITH_HOLES_2_HPP

#include <type_traits>

#include <nanobind/nanobind.h>

#include "CGALPY/if_.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

template <typename T, typename = void>
struct target { typedef typename T::General_polygon_2 type; };

template <typename T>
struct target<T, typename if_<false, typename T::Polygon_2>::type> {
  typedef typename T::Polygon_2         type;
};

template <typename GeneralPolygonWithHoles_2>
void
ctr_polygon_with_holes_2(GeneralPolygonWithHoles_2& pwh,
                         typename target<GeneralPolygonWithHoles_2>::type& p,
                         py::list& lst) {
  using Polygon_2 = typename target<GeneralPolygonWithHoles_2>::type;
  auto begin = stl_input_iterator<Polygon_2>(lst);
  auto end = stl_input_iterator<Polygon_2>(lst, false);
  new (&pwh) GeneralPolygonWithHoles_2(p, begin, end);
}

template <typename GeneralPolygonWithHoles_2>
typename GeneralPolygonWithHoles_2::Hole_iterator
holes_begin(GeneralPolygonWithHoles_2& p) { return p.holes_begin(); }

template <typename GeneralPolygonWithHoles_2>
typename GeneralPolygonWithHoles_2::Hole_iterator
holes_end(GeneralPolygonWithHoles_2& p) { return p.holes_end(); }

template <typename Type>
void export_general_polygon_with_holes_2(py::class_<Type>& co) {
  using Pgn = typename target<Type>::type;

  co.def(py::init<Type&>());
  co.def(py::init<Pgn&>());
  co.def("__init__", &ctr_polygon_with_holes_2<Type>);
  co.def("is_unbounded", &Type::is_unbounded);

  // Use `py::overload_cast` to cast overloaded functions.
  // 1. As a convention, add the suffix `_mutable` to the mutable version.
  // 2. Wrap the mutable method with the `reference_internal` call policy.
  // 3. Add the `const_` tag to the overloaded const function, as the
  //    overloading is based on constness.
  co.def("outer_boundary_mutable",
         py::overload_cast<>(&Type::outer_boundary),
         py::rv_policy::reference_internal);
  co.def("outer_boundary",
         py::overload_cast<>(&Type::outer_boundary, py::const_));

  // co->def("holes", py::range<py::return_internal_reference<>>
  //         (&holes_begin<Type>, &holes_end<Type>)); NB
  co.def("number_of_holes", &Type::number_of_holes);

  add_insertion(co, "__str__");
  add_insertion(co, "__repr__");
}

#endif
