// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosDirectionalXMonotoneTraits_2.hpp"
#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_directional_x_monotone_traits_classes.hpp"

namespace bp = boost::python;

bp::class_<aos2::Geometry_traits_2> export_arr_conic_traits() {
  //TODO export RatKernel, AlgKernel
  typedef aos2::Geometry_traits_2       GT;
  typedef aos2::Curve_2                 Curve_2;
  typedef aos2::X_monotone_curve_2      X_monotone_curve_2;
  typedef GT::Rational                  Rational;

  auto traits = bp::class_<GT>("Geometry_traits_2")
    .def(bp::init<>())
    ;

  bp::scope traits_scope(traits);
  struct Concepts {
    Aos_basic_traits_classes<GT> m_basic_traits_classes;
    Aos_x_monotone_traits_classes<GT> m_x_monotone_traits_classes;
    Aos_traits_classes<GT> m_traits_classes;
    Aos_directional_x_monotone_traits_classes<GT>
      m_directional_x_monotone_traits_classes;
  } concepts;
  export_AosTraits_2<GT, Return_by_value>(traits, concepts);
  export_AosDirectionalXMonotoneTraits_2<GT, Return_by_value>(traits, concepts);

  bp::class_<GT::Integer>("Integer")
    .def(bp::init<>())
    .def(bp::init<const GT::Integer&>())
    .def(bp::init<int>())
    ;

  auto& p_co = *(concepts.m_basic_traits_classes.m_point_2);
  p_co
    .def(bp::init<double, double>())
    ;

  auto& xcv_co = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  xcv_co
    .def("left", &X_monotone_curve_2::left, Copy_const_reference())
    .def("right", &X_monotone_curve_2::right, Copy_const_reference())
    ;

  auto& cv_co = *(concepts.m_traits_classes.m_curve_2);
  //TODO add constructors
  cv_co
    .def(bp::init<double, double, double, double, double, double>())
    .def(bp::init<double, double, double, double, double, double,
         CGAL::Orientation, const GT::Point_2&, const GT::Point_2&>())
    .def(bp::init<const Rational&, const Rational&, const Rational&,
         const Rational&, const Rational&, const Rational&>())
    .def(bp::init<const Rational&, const Rational&, const Rational&,
         const Rational&, const Rational&, const Rational&,
         CGAL::Orientation, const GT::Point_2&, const GT::Point_2&>())
    .def("source", &Curve_2::source, Copy_const_reference())
    .def("target", &Curve_2::target, Copy_const_reference())
    .def("orientation", &Curve_2::orientation)
    .def("is_valid", &Curve_2::is_valid)
    .def("is_x_monotone", &Curve_2::is_x_monotone)
    .def("is_y_monotone", &Curve_2::is_y_monotone)
    .def("is_full_conic", &Curve_2::is_full_conic)
    .def("bbox", &Curve_2::bbox)
    .def("set_source", &Curve_2::set_source)
    .def("set_target", &Curve_2::set_target)
    .def("r", &Curve_2::r, Copy_const_reference())
    .def("s", &Curve_2::s, Copy_const_reference())
    .def("t", &Curve_2::t, Copy_const_reference())
    .def("u", &Curve_2::u, Copy_const_reference())
    .def("v", &Curve_2::v, Copy_const_reference())
    .def("w", &Curve_2::w, Copy_const_reference())
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    ;

    return traits;
}
