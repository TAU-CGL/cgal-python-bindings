// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <unordered_set>
#include <any>

#include <boost/python.hpp>
#include <boost/assert.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosLandmarkTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosDirectionalXMonotoneTraits_2.hpp"

#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_landmark_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_directional_x_monotone_traits_classes.hpp"

namespace bp = boost::python;

namespace aos2 {

Segment_2 to_segment(Curve_2& c) { return Segment_2(c); }

}

bp::object export_arr_segment_traits() {
  typedef aos2::Geometry_traits_2       GT;
  auto traits = bp::class_<GT>("Geometry_traits_2");
  bp::scope traits_scope(traits);
  struct Concepts {
    Aos_basic_traits_classes<GT> m_basic_traits_classes;
    Aos_x_monotone_traits_classes<GT> m_x_monotone_traits_classes;
    Aos_traits_classes<GT> m_traits_classes;
    Aos_landmark_traits_classes<GT> m_landmark_traits_classes;
    Aos_directional_x_monotone_traits_classes<GT> m_directional_x_monotone_traits_classes;
  };
  Concepts concepts;
  export_AosTraits_2<GT, Copy_const_reference>(traits, concepts);
  export_AosLandmarkTraits_2<GT, Copy_const_reference>(traits, concepts);
  export_AosDirectionalXMonotoneTraits_2<GT, Copy_const_reference>(traits, concepts);
  traits
    .def("is_in_x_range_2_object", &GT::is_in_x_range_2_object)
    .def("is_in_y_range_2_object", &GT::is_in_y_range_2_object)
    ;

  const bp::type_info info = bp::type_id<aos2::Point_2>();
  const bp::converter::registration* reg = bp::converter::registry::query(info);
  BOOST_ASSERT((reg != nullptr) && ((*reg).m_to_python != nullptr));
  traits_scope.attr("Point_2") = bp::handle<>(reg->get_class_object());

  bp::class_<aos2::Curve_2>("Curve_2")
    .def(bp::init<>())
    .def(bp::init<Segment_2&>())
    .def(bp::init<aos2::Point_2&, aos2::Point_2&>())
    .def(bp::init<Line_2&, aos2::Point_2&, aos2::Point_2&>())
    .def("source", &aos2::Curve_2::source, Copy_const_reference())
    .def("target", &aos2::Curve_2::target, Copy_const_reference())
    .def("line", &aos2::Curve_2::line, Copy_const_reference())
    .def("is_vertical", &aos2::Curve_2::is_vertical)
    .def("flip", &aos2::Curve_2::flip)
    // .def("left", &aos2::Curve_2::left, Copy_const_reference())
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

  bp::class_<GT::Is_in_x_range_2>("Is_in_x_range_2", bp::no_init)
    .def("__call__", &GT::Is_in_x_range_2::operator());
    ;

    bp::class_<GT::Is_in_y_range_2>("Is_in_y_range_2", bp::no_init)
    .def("__call__", &GT::Is_in_y_range_2::operator());
    ;

  return traits;
}
