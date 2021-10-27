// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>

#include "CGALPY/polygon_2_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"

namespace bp = boost::python;

namespace pol2 {

Polygon_with_holes_2* init_polygon_with_holes_2(Polygon_2& p, bp::list& lst) {
  auto begin = bp::stl_input_iterator< Polygon_2 >(lst);
  auto end = bp::stl_input_iterator< Polygon_2 >();
  return new Polygon_with_holes_2(p, begin, end);
}

Polygon_with_holes_2::Hole_const_iterator holes_begin(Polygon_with_holes_2& p)
{ return p.holes_begin(); }

Polygon_with_holes_2::Hole_const_iterator holes_end(Polygon_with_holes_2& p)
{ return p.holes_end(); }

Polygon_2& outer_boundary(Polygon_with_holes_2& p)
{ return p.outer_boundary(); }

}

void export_polygon_with_holes_2() {
  typedef pol2::Polygon_2               Polygon_2;
  typedef pol2::Polygon_with_holes_2    Polygon_with_holes_2;

  bp::class_<Polygon_with_holes_2>("Polygon_with_holes_2")
    .def(bp::init<Polygon_2&>())
    .def("__init__", make_constructor(&pol2::init_polygon_with_holes_2))
    .def("is_unbounded", &Polygon_with_holes_2::is_unbounded)
    .def("outer_boundary", &pol2::outer_boundary,
         bp::return_internal_reference<>())
    .def("holes", bp::range<bp::return_internal_reference<>>(&pol2::holes_begin,
                                                             &pol2::holes_end))
    .def("number_of_holes", &Polygon_with_holes_2::number_of_holes)
    .def("bbox", &Polygon_with_holes_2::bbox)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    ;
}
