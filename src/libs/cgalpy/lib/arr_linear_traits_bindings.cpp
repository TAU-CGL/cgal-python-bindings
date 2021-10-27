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
#include "CGALPY/aos_2_concepts/export_AosOpenBoundaryTraits_2.hpp"

#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_landmark_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_open_boundary_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_approximate_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_construct_x_monotone_curve_traits_classes.hpp"

namespace bp = boost::python;

typedef typename aos2::Geometry_traits_2::Curve_2 Curve_2;
using overload = void (Curve_2::*)();

void set_left(Curve_2& c, Point_2& p) { c.set_left(p); }

void set_right(Curve_2& c, Point_2& p) { c.set_right(p); }

bp::object export_arr_linear_traits() {
  typedef aos2::Geometry_traits_2       GT;
  auto traits = bp::class_<GT>("Geometry_traits_2");
  bp::scope traits_scope(traits);
  struct Concepts {
    Aos_basic_traits_classes<GT> m_basic_traits_classes;
    Aos_x_monotone_traits_classes<GT> m_x_monotone_traits_classes;
    Aos_traits_classes<GT> m_traits_classes;
    Aos_landmark_traits_classes<GT> m_landmark_traits_classes;
    Aos_open_boundary_traits_classes<GT> m_open_boundary_traits_classes;
    Aos_approximate_traits_classes<GT> m_approximate_traits_classes;
    Aos_construct_x_monotone_curve_traits_classes<GT>
      m_construct_x_monotone_curve_traits_classes;
  };
  Concepts concepts;
  export_AosTraits_2<GT, Copy_const_reference>(traits, concepts);
  export_AosLandmarkTraits_2<GT, Copy_const_reference>(traits, concepts);
  export_AosOpenBoundaryTraits_2<GT, Copy_const_reference>(traits, concepts);


  auto& cv_co = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  cv_co
    .def(bp::init<Segment_2&>())
    .def(bp::init<Ray_2&>())
    .def(bp::init<Line_2&>())
    .def("source", &Curve_2::source, Copy_const_reference())
    .def("target", &Curve_2::target, Copy_const_reference())
    .def("line", &Curve_2::line)
    .def("is_vertical", &Curve_2::is_vertical)
    .def("is_segment", &Curve_2::is_segment)
    .def("segment", &Curve_2::segment)
    .def("is_ray", &Curve_2::ray)
    .def("is_line", &Curve_2::is_line)
    .def("line", &Curve_2::line)
    .def("supporting_line", &Curve_2::supporting_line, Copy_const_reference())
    .def("left", &Curve_2::left, Copy_const_reference())
    .def("right", &Curve_2::right, Copy_const_reference())
    .def("set_left", static_cast<overload>(&Curve_2::set_left))
    .def("set_left", set_left)
    .def("set_right", static_cast<overload>(&Curve_2::set_right))
    .def("set_right", set_right)
    .def("is_directed_right", &Curve_2::is_directed_right)
    .def("is_in_x_range", &Curve_2::is_in_x_range)
    .def("is_in_y_range", &Curve_2::is_in_y_range)
    .def("is_degenerate", &Curve_2::is_degenerate)
    .def("bbox", &Curve_2::bbox)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    ;

  return traits;
}
