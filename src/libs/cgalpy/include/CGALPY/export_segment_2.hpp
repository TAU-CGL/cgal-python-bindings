// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_SEGMENT_2_HPP
#define CGALPY_EXPORT_SEGMENT_2_HPP

#include <boost/python.hpp>

namespace bp = boost::python;

template <typename Segment, typename Point>
void export_segment_2() {
  bp::class_<Segment>("Segment_2")
    .def(bp::init<Point&, Point&>())
    .def("source", &Segment::source, Kernel_return_value_policy())
    .def("target", &Segment::target, Kernel_return_value_policy())
    .def("vertex", &Segment::vertex, Kernel_return_value_policy())
    .def("point", &Segment::point, Kernel_return_value_policy())
    .def("__getitem__", &Segment::operator[], Kernel_return_value_policy())
    .def("min", &Segment::min, Kernel_return_value_policy())
    .def("max", &Segment::max, Kernel_return_value_policy())
    .def("opposite", &Segment::opposite)
    .def("to_vector", &Segment::to_vector)
    .def("supporting_line", &Segment::supporting_line)
    .def("squared_length", &Segment::squared_length)
    .def("direction", &Segment::direction)
    .def("has_on", &Segment::has_on)
    .def("collinear_has_on", &Segment::collinear_has_on)
    .def("is_degenerate", &Segment::is_degenerate)
    .def("is_horizontal", &Segment::is_horizontal)
    .def("is_vertical", &Segment::is_vertical)
    .def("bbox", &Segment::bbox)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    //.setattr("__hash__", &hash<Segment>)
    ;
}

#endif
