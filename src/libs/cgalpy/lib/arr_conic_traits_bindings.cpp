// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/config.hpp"
#include "CGALPY/common.hpp"

void export_Arr_conic_traits()
{
  using namespace boost::python;
  class_<Curve_2>("Curve_2")
    .def("source", &Curve_2::source, return_value_policy<copy_const_reference>())
    .def("target", &Curve_2::target, return_value_policy<copy_const_reference>())
    .def("orientation", &Curve_2::orientation)
    .def("is_valid", &Curve_2::is_valid)
    .def("is_x_monotone", &Curve_2::is_x_monotone)
    .def("is_y_monotone", &Curve_2::is_y_monotone)
    .def("is_full_conic", &Curve_2::is_full_conic)
    .def("bbox", &Curve_2::bbox)
    .def("set_source", &Curve_2::set_source)
    .def("set_target", &Curve_2::set_target)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    ;

  class_<X_monotone_curve_2>("Curve_2")
    .def(init<const Curve_2&>())
    .def("left", &X_monotone_curve_2::left, return_value_policy<copy_const_reference>())
    .def("right", &X_monotone_curve_2::right, return_value_policy<copy_const_reference>())
    ;
}
