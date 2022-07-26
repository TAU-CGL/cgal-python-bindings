// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>

#include "CGALPY/config.hpp"
#include "CGALPY/kernel_types.hpp"

namespace bp = boost::python;

void export_vector_2() {
  bp::class_<Vector_2>("Vector_2")
    .def(bp::init<Point_2&, Point_2&>())
    .def(bp::init<Line_2>())
    .def(bp::init<Ray_2>())
    .def(bp::init<Segment_2>())
    .def(bp::init<FT&, FT&, FT&>())
    .def(bp::init<FT&, FT&>())
    .def(bp::init<double, double>())
    .def("hx", &Vector_2::hx)
    .def("hy", &Vector_2::hy)
    .def("hw", &Vector_2::hw)
    .def("x", &Vector_2::x)
    .def("y", &Vector_2::y)
    .def("squared_length", &Vector_2::squared_length)
    .def("homogeneous", &Vector_2::homogeneous)
    .def("cartesian", &Vector_2::cartesian)
    .def("__getitem__", &Vector_2::operator[])
#if CGALPY_KERNEL == CGALPY_KERNEL_EPIC
    // TODO: Returning address of local variable or temporary with EPEC kernel
    .def("cartesian_coordinates", bp::range<>(&Vector_2::cartesian_begin, & Vector_2::cartesian_end))
#endif
    .def("dimension", &Vector_2::dimension)
    .def("direction", &Vector_2::direction)
    .def("transform", &Vector_2::transform)
    .def("perpendicular", &Vector_2::perpendicular)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self != bp::self)
    .def(bp::self + bp::self)
    .def(bp::self += bp::self)
    .def(bp::self - bp::self)
    .def(bp::self -= bp::self)
    .def(-bp::self)
    .def(bp::self * bp::self)
    //.def(self*RT())
    .def(bp::self * FT())
    //.def(RT() * bp::self)
    .def(FT()*bp::self)
    //.def(bp::self *= RT())
    .def(bp::self*=FT())
    //.def(bp::self / RT())
    .def(bp::self / FT())
    //.def(bp::self /= RT())
    .def(bp::self /= FT())
    //.setattr("__hash__", &hash<Vector_2>)
    ;
}
