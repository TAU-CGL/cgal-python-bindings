// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_POINT_2_HPP
#define CGALPY_EXPORT_POINT_2_HPP

#include <boost/python.hpp>

#include "CGALPY/hash_rational_point.hpp"

namespace bp = boost::python;

template <typename Point>
void export_point_2() {
  bp::class_<Point>("Point_2")
    .def(bp::init<>())
    .def(bp::init<double, double>())
    .def(bp::init<FT&, FT&>())
    .def(bp::init<RT&, RT&>())
    .def(bp::init<Point&>())
    .def("x", &Point::x, Kernel_return_value_policy())
    .def("y", &Point::y, Kernel_return_value_policy())
    .def("hx", &Point::hx, Kernel_return_value_policy())
    .def("hy", &Point::hy, Kernel_return_value_policy())
    .def("hw", &Point::hw, Kernel_return_value_policy())
    .def("bbox", &Point::bbox)
    .def("cartesian", &Point::cartesian, Kernel_return_value_policy())
    .def("__getitem__", &Point::operator[], Kernel_return_value_policy())
#if CGALPY_KERNEL == CGALPY_KERNEL_EPIC
    // TODO: Returning address of local variable or temporary with EPEC kernel
    .def("coordinates", bp::range<>(&Point::cartesian_begin, &Point::cartesian_end))
#endif
    .def("dimension", &Point::dimension)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self > bp::self)
    .def(bp::self < bp::self)
    .def(bp::self <= bp::self)
    .def(bp::self >= bp::self)
    .def(bp::self - bp::self)
    .def(bp::self += Vector_2())
    .def(bp::self -= Vector_2())
    .def(bp::self + Vector_2())
    .def(bp::self - Vector_2())
    .setattr("__hash__", &hash_rational_point<Point>)
    ;
}

#endif
