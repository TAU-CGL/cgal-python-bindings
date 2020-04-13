// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include "CGALPY/config.hpp"
#include "CGALPY/common.hpp"

Polygon_with_holes_2* init_Polygon_with_holes_2(Polygon_2& p,
                                                boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator< Polygon_2 >(lst);
  auto end = boost::python::stl_input_iterator< Polygon_2 >();
  return new Polygon_with_holes_2(p, begin, end);
}

Polygon_with_holes_2::Hole_const_iterator holes_begin(Polygon_with_holes_2& p)
{
  return p.holes_begin();
}

Polygon_with_holes_2::Hole_const_iterator holes_end(Polygon_with_holes_2& p)
{
  return p.holes_end();
}

Polygon_2& outer_boundary(Polygon_with_holes_2& p) { return p.outer_boundary(); }

void export_polygon_with_holes_2()
{
  using namespace boost::python;
  class_<Polygon_with_holes_2>("Polygon_with_holes_2")
    .def(init<Polygon_2&>())
    .def("__init__", make_constructor(&init_Polygon_with_holes_2))
    .def("is_unbounded", &Polygon_with_holes_2::is_unbounded)
    .def("outer_boundary", &outer_boundary, return_internal_reference<>())
    .def("holes", range<return_internal_reference<>>(&holes_begin, &holes_end))
    .def("number_of_holes", &Polygon_with_holes_2::number_of_holes)
    .def("bbox", &Polygon_with_holes_2::bbox)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    ;
}
