// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <boost/python.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"

namespace bp = boost::python;

void export_arr_conic_traits() {
  //TODO export RatKernel, AlgKernel

  bp::scope traits_scope = bp::class_<Traits>("Traits")
    .def(bp::init<>())
    ;

  bp::class_<Traits::Integer>("Integer")
    .def(bp::init<>())
    .def(bp::init<const Traits::Integer&>())
    .def(bp::init<int>())
    ;

  bp::class_<Curve_2>("Curve_2")
    .def(bp::init<>())
    .def(bp::init<const Curve_2&>())
    //TODO add constructors
    .def("source", &Curve_2::source, bp::return_value_policy<bp::copy_const_reference>())
    .def("target", &Curve_2::target, bp::return_value_policy<bp::copy_const_reference>())
    .def("orientation", &Curve_2::orientation)
    .def("is_valid", &Curve_2::is_valid)
    .def("is_x_monotone", &Curve_2::is_x_monotone)
    .def("is_y_monotone", &Curve_2::is_y_monotone)
    .def("is_full_conic", &Curve_2::is_full_conic)
    .def("bbox", &Curve_2::bbox)
    .def("set_source", &Curve_2::set_source)
    .def("set_target", &Curve_2::set_target)
    .def("r", &Curve_2::r, bp::return_value_policy<bp::copy_const_reference>())
    .def("s", &Curve_2::s, bp::return_value_policy<bp::copy_const_reference>())
    .def("t", &Curve_2::t, bp::return_value_policy<bp::copy_const_reference>())
    .def("u", &Curve_2::u, bp::return_value_policy<bp::copy_const_reference>())
    .def("v", &Curve_2::v, bp::return_value_policy<bp::copy_const_reference>())
    .def("w", &Curve_2::w, bp::return_value_policy<bp::copy_const_reference>())
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    ;

  bp::class_<X_monotone_curve_2>("X_monotone_curve_2")
    .def(bp::init<const X_monotone_curve_2&>())
    .def("left", &X_monotone_curve_2::left, bp::return_value_policy<bp::copy_const_reference>())
    .def("right", &X_monotone_curve_2::right, bp::return_value_policy<bp::copy_const_reference>())
    ;
}
