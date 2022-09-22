// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_GENERAL_POLYGON_2_HPP
#define CGALPY_EXPORT_GENERAL_POLYGON_2_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

namespace bso2 {

// Initialize a general polygon from a list of x-monotone curves.
template <typename GeneralPolygon_2>
void init_polygon_2(GeneralPolygon_2& pgn, py::list& lst) {
  using Gpgn = GeneralPolygon_2;
  using Xcv = typename Gpgn::X_monotone_curve_2;
  auto begin = stl_input_iterator<Xcv>(lst);
  auto end = stl_input_iterator<Xcv>(lst, false);
  new (&pgn) Gpgn(begin, end);
}

}

// Export the attributes of General_polygon_2.
template <typename GeneralPolygon_2>
inline void
export_general_polygon_2(py::class_<GeneralPolygon_2>& pgn_c) {
  using Gpgn = GeneralPolygon_2;
  pgn_c.def(py::init<>())
    .def(py::init<const Gpgn&>())
    .def("__init__", &bso2::init_polygon_2<Gpgn>)
    .def("push_back", &Gpgn::push_back)
    .def("orientation", &Gpgn::orientation)
    .def("is_empty", &Gpgn::is_empty)
    .def("size", &Gpgn::size)
    .def("bbox", &Gpgn::bbox)
    .def("clear", &Gpgn::clear)
    .def("reverse_orientation", &Gpgn::reverse_orientation)
    ;

  add_insertion(pgn_c, "__str__");
  add_insertion(pgn_c, "__repr__");
  add_extraction(pgn_c);
}

/*! Capture the call to export a Polygon_2<> and ensure that it is not invoked.
 */
template <typename Kernel, typename Container>
inline void export_general_polygon_2
(py::class_<CGAL::Polygon_2<Kernel, Container>>& /* pgn_c */) {
  throw std::runtime_error("Attempting to export Polygon_2 as General_polygon_2!");
}

#endif
