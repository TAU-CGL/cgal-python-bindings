// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include <boost/python.hpp>

#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>

namespace bp = boost::python;

void export_gmpz() {
  bp::class_<CGAL::Gmpz>("Gmpz")
    .def(bp::init<int>())
    .def(bp::init<CGAL::Gmpz&>())
    .def("to_double", &CGAL::Gmpz::to_double)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self < bp::self)
    .def(bp::self > bp::self)
    .def(bp::self <= bp::self)
    .def(bp::self >= bp::self)
    .def(bp::self + bp::self)
    .def(bp::self += bp::self)
    .def(bp::self - bp::self)
    .def(bp::self -= bp::self)
    .def(bp::self * bp::self)
    .def(bp::self *= bp::self)
    .def(bp::self / bp::self)
    .def(bp::self /= bp::self)
    ;
}

void export_gmpq() {
  bp::class_<CGAL::Gmpq>("Gmpq")
    .def(bp::init<CGAL::Gmpz, CGAL::Gmpz>())
    .def(bp::init<unsigned long, unsigned long>())
    .def(bp::init<const std::string&>())
    .def(bp::init<CGAL::Gmpq&>())
    .def(bp::init<double>())
    .def("to_double", &CGAL::Gmpq::to_double)
    .def("numerator", &CGAL::Gmpq::numerator)
    .def("denominator", &CGAL::Gmpq::denominator)
    .def("size", &CGAL::Gmpq::size)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self < bp::self)
    .def(bp::self > bp::self)
    .def(bp::self <= bp::self)
    .def(bp::self >= bp::self)
    .def(bp::self + bp::self)
    .def(bp::self += bp::self)
    .def(bp::self - bp::self)
    .def(bp::self -= bp::self)
    .def(bp::self * bp::self)
    .def(bp::self *= bp::self)
    .def(bp::self / bp::self)
    .def(bp::self /= bp::self)
    .def(-bp::self)
    ;
}
