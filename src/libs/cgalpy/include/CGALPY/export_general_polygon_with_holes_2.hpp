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
#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

template <typename T, typename = void>
struct target { typedef typename T::General_polygon_2 type; };

template <typename T>
struct target<T, typename if_<false, typename T::Polygon_2>::type> {
  typedef typename T::Polygon_2         type;
};

// Initialize a general polygon with hole from the outer boundary and a list
// of holes.
template <typename GeneralPolygonWithHoles_2>
void
init_polygon_with_holes_2(GeneralPolygonWithHoles_2& pwh,
                          typename target<GeneralPolygonWithHoles_2>::type& p,
                          py::list& lst) {
  using Polygon_2 = typename target<GeneralPolygonWithHoles_2>::type;
  auto begin = stl_input_iterator<Polygon_2>(lst);
  auto end = stl_input_iterator<Polygon_2>(lst, false);
  new (&pwh) GeneralPolygonWithHoles_2(p, begin, end);
}

template <typename Type>
void export_general_polygon_with_holes_2(py::class_<Type>& pwh_c) {
  using Pwh = Type;
  using Pgn = typename target<Type>::type;

  pwh_c.def(py::init<Pwh&>())
    .def(py::init<Pgn&>())
    .def("__init__", &init_polygon_with_holes_2<Pwh>)
    .def("is_unbounded", &Pwh::is_unbounded)

    // Use `py::overload_cast` to cast overloaded functions.
    // 1. As a convention, add the suffix `_mutable` to the mutable version.
    // 2. Wrap the mutable method with the `reference_internal` call policy.
    // 3. Add the `const_` tag to the overloaded const function, as the
    //    overloading is based on constness.
    .def("outer_boundary_mutable", py::overload_cast<>(&Pwh::outer_boundary),
         py::rv_policy::reference_internal)
    .def("outer_boundary",
         py::overload_cast<>(&Pwh::outer_boundary, py::const_))
    .def("add_hole", py::overload_cast<const Pgn&>(&Pwh::add_hole))
    .def("erase_hole", &Pwh::erase_hole)
    .def("has_holes", &Pwh::has_holes)
    .def("number_of_holes", &Pwh::number_of_holes)
    .def("clear", &Pwh::clear)
    .def("is_plane", &Pwh::is_plane)
    ;

  using Hci = typename Pwh::Hole_const_iterator;
  add_iterator<Hci, Hci, const Pgn&>("Hole_iterator", pwh_c);
  pwh_c.def("holes",
            [] (const Pwh& pwh)
            { return make_iterator(pwh.holes_begin(), pwh.holes_end()); },
            py::keep_alive<0, 1>());

  add_insertion(pwh_c, "__str__");
  add_insertion(pwh_c, "__repr__");
}

#endif
