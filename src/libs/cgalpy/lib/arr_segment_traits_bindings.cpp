// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <boost/python.hpp>
#include <boost/assert.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/export_point_2.hpp"

namespace bp = boost::python;

Segment_2 to_segment(Curve_2& c) { return Segment_2(c); }

void export_arr_segment_traits() {
  bp::scope traits_scope = bp::class_<Traits>("Traits")
    .def(bp::init<>())
    .def("is_in_x_range_2_object", &Traits::is_in_x_range_2_object)
    .def("is_in_y_range_2_object", &Traits::is_in_y_range_2_object)
    ;

  const bp::type_info info = bp::type_id<TPoint_2>();
  const bp::converter::registration* reg = bp::converter::registry::query(info);
  BOOST_ASSERT((reg == nullptr) || ((*reg).m_to_python == nullptr));
  traits_scope.attr("Point_2") = bp::handle<>(reg->m_class_object);

  bp::class_<Curve_2>("Curve_2")
    .def(bp::init<>())
    .def(bp::init<Segment_2&>())
    .def(bp::init<TPoint_2&, TPoint_2&>())
    .def(bp::init<Line_2&, TPoint_2&, TPoint_2&>())
    .def("source", &Curve_2::source, bp::return_value_policy<bp::copy_const_reference>())
    .def("target", &Curve_2::target, bp::return_value_policy<bp::copy_const_reference>())
    .def("line", &Curve_2::line, bp::return_value_policy<bp::copy_const_reference>())
    .def("is_vertical", &Curve_2::is_vertical)
    .def("flip", &Curve_2::flip)
    .def("left", &Curve_2::left, bp::return_value_policy<bp::copy_const_reference>())
    .def("right", &Curve_2::right, bp::return_value_policy<bp::copy_const_reference>())
    .def("set_left", &Curve_2::set_left)
    .def("set_right", &Curve_2::set_right)
    .def("is_directed_right", &Curve_2::is_directed_right)

    // Deprecated
    // .def("is_in_x_range", &Curve_2::is_in_x_range)
    // .def("is_in_y_range", &Curve_2::is_in_y_range)

    .def("bbox", &Curve_2::bbox)
    .def("segment", &to_segment)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
  ;

  const bp::type_info cv_info = bp::type_id<X_monotone_curve_2>();
  const bp::converter::registration* cv_reg =
    bp::converter::registry::query(cv_info);
  BOOST_ASSERT((cv_reg != nullptr) || ((*cv_reg).m_to_python != nullptr));
  traits_scope.attr("X_monotone_curve_2") = bp::handle<>(cv_reg->m_class_object);

  bp::class_<Is_in_x_range_2>("Is_in_x_range_2", bp::no_init)
    .def("__call__", &Is_in_x_range_2::operator());
    ;

  bp::class_<Is_in_y_range_2>("Is_in_y_range_2", bp::no_init)
    .def("__call__", &Is_in_y_range_2::operator());
    ;
}
