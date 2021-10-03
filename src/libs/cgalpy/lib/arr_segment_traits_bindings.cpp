// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>
#include <boost/assert.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/arrangement_2_concepts/export_ArrangementTraits_2.hpp"
#include "CGALPY/arrangement_2_concepts/export_ArrangementLandmarkTraits_2.hpp"
#include "CGALPY/arrangement_2_concepts/export_ArrangementDirectionalXMonotoneTraits_2.hpp"

namespace bp = boost::python;

namespace aos2 {

Segment_2 to_segment(Curve_2& c) { return Segment_2(c); }

}

void export_arr_segment_traits() {
  auto traits = bp::class_<aos2::Traits>("Traits");
  bp::scope traits_scope = traits;
  export_ArrangementTraits_2<aos2::Traits, Copy_const_reference>(traits);
  export_ArrangementLandmarkTraits_2<aos2::Traits, Copy_const_reference>(traits);
  export_ArrangementDirectionalXMonotoneTraits_2<aos2::Traits, Copy_const_reference>(traits);
  traits
    .def("is_in_x_range_2_object", &aos2::Traits::is_in_x_range_2_object)
    .def("is_in_y_range_2_object", &aos2::Traits::is_in_y_range_2_object)
    ;

  const bp::type_info info = bp::type_id<aos2::TPoint_2>();
  const bp::converter::registration* reg = bp::converter::registry::query(info);
  BOOST_ASSERT((reg != nullptr) && ((*reg).m_to_python != nullptr));
  traits_scope.attr("Point_2") = bp::handle<>(reg->m_class_object);

  bp::class_<aos2::Curve_2>("Curve_2")
    .def(bp::init<>())
    .def(bp::init<Segment_2&>())
    .def(bp::init<aos2::TPoint_2&, aos2::TPoint_2&>())
    .def(bp::init<Line_2&, aos2::TPoint_2&, aos2::TPoint_2&>())
    .def("source", &aos2::Curve_2::source, Copy_const_reference())
    .def("target", &aos2::Curve_2::target, Copy_const_reference())
    .def("line", &aos2::Curve_2::line, Copy_const_reference())
    .def("is_vertical", &aos2::Curve_2::is_vertical)
    .def("flip", &aos2::Curve_2::flip)
    .def("left", &aos2::Curve_2::left, Copy_const_reference())
    .def("right", &aos2::Curve_2::right, Copy_const_reference())
    .def("set_left", &aos2::Curve_2::set_left)
    .def("set_right", &aos2::Curve_2::set_right)
    .def("is_directed_right", &aos2::Curve_2::is_directed_right)

    // Deprecated
    // .def("is_in_x_range", &Curve_2::is_in_x_range)
    // .def("is_in_y_range", &Curve_2::is_in_y_range)

    .def("bbox", &aos2::Curve_2::bbox)
    .def("segment", &aos2::to_segment)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
  ;

  const bp::type_info cv_info = bp::type_id<aos2::X_monotone_curve_2>();
  const bp::converter::registration* cv_reg =
    bp::converter::registry::query(cv_info);
  BOOST_ASSERT((cv_reg != nullptr) && ((*cv_reg).m_to_python != nullptr));
  traits_scope.attr("X_monotone_curve_2") = bp::handle<>(cv_reg->m_class_object);

  bp::class_<aos2::Traits::Is_in_x_range_2>("Is_in_x_range_2", bp::no_init)
    .def("__call__", &aos2::Traits::Is_in_x_range_2::operator());
    ;

    bp::class_<aos2::Traits::Is_in_y_range_2>("Is_in_y_range_2", bp::no_init)
    .def("__call__", &aos2::Traits::Is_in_y_range_2::operator());
    ;
}
