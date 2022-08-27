// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/polygon_2_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

namespace pol2 {

// Initialize a polygon with holes from an outer boundary and a list of holes.
void init_polygon_with_holes_2(Polygon_with_holes_2& pwh, Polygon_2& p,
                               py::list& lst) {
  auto begin = stl_input_iterator<Polygon_2>(lst);
  auto end = stl_input_iterator<Polygon_2>(lst, false);
  new (&pwh) Polygon_with_holes_2(p, begin, end);
}

}

//
void export_polygon_with_holes_2(py::module_& m) {
  using Pgn = pol2::Polygon_2;
  using Pwh = pol2::Polygon_with_holes_2;

  py::class_<Pwh> pwh_c(m, "Polygon_with_holes_2");
  pwh_c.def(py::init<>())
    .def(py::init<Pgn&>())
    .def("__init__", &pol2::init_polygon_with_holes_2)
    .def("is_unbounded", &Pwh::is_unbounded)

    // Use `py::overload_cast` to cast overloaded functions.
    // 1. As a convention, add the suffix `_mutable` to the mutable version.
    // 2. Wrap the mutable method with the `reference_internal` call policy.
    // 3. Add the `const_` tag to the overloaded const function, as the
    //    overloading is based on constness.
    .def("outer_boundary_mutable", py::overload_cast<>(&Pwh::outer_boundary),
         py::rv_policy::reference_internal)
    .def("outer_boundary", py::overload_cast<>(&Pwh::outer_boundary, py::const_))
    .def("number_of_holes", &Pwh::number_of_holes)
    .def("bbox", &Pwh::bbox)
    .def(py::self == py::self)
    .def(py::self != py::self)
    ;

  using Hci = Pwh::Hole_const_iterator;
  add_iterator<Hci, Hci, const Pgn&>("Hole_iterator", pwh_c);
  pwh_c.def("holes",
              [] (const Pwh& pwh)
              { return make_iterator(pwh.holes_begin(), pwh.holes_end()); },
              py::keep_alive<0, 1>());

  add_insertion(pwh_c, "__str__");
  add_insertion(pwh_c, "__repr__");
}
