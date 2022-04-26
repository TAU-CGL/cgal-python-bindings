// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS          1
#define BOOST_PYTHON_MAX_ARITY                  30

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
  //TODO export Rat_kernel, Alg_kernel
  typedef aos2::Geometry_traits_2       GT;
  typedef aos2::Curve_2                 Curve_2;
  typedef aos2::X_monotone_curve_2      X_monotone_curve_2;
  typedef GT::Rational                  Rational;
  typedef aos2::Rat_point_2             Rat_point_2;
  typedef aos2::Rat_segment_2           Rat_segment_2;
  typedef aos2::Rat_circle_2            Rat_circle_2;

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

  bp::class_<Rational>("Rational")
    .def(bp::init<>())
    .def(bp::init<const Rational&>())
    .def(bp::init<int, int>())
    ;

  auto& p_co = *(concepts.m_basic_traits_classes.m_point_2);
  p_co
    .def(bp::init<double, double>())
    .def(bp::init<Rational, double>())
    .def(bp::init<double, Rational>())
    .def(bp::init<Rational, Rational>())
    ;

  auto& xcv_co = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  xcv_co
    .def("left", &X_monotone_curve_2::left, Copy_const_reference())
    .def("right", &X_monotone_curve_2::right, Copy_const_reference())
    ;

  /*! \todo Export the rational kernel and the algebraic kernel properly.
   * until the above is done, we export Rat_point_2, Rat_segment_2, and
   * Rat_circle_2 in a minimal way.
   */
  bp::class_<Rat_point_2>("Rat_point_2")
    .def(bp::init<>())
    .def(bp::init<double, double>())
    .def(bp::init<const Rational&, const Rational&>())
    .def(bp::init<double, const Rational&>())
    .def(bp::init<const Rational&, double>())
    // .def(bp::init<RT&, RT&>())
    .def(bp::init<const Rat_point_2&>())
    .def("x", &Rat_point_2::x, Return_by_value())
    .def("y", &Rat_point_2::y, Return_by_value())
    .def("hx", &Rat_point_2::hx, Return_by_value())
    .def("hy", &Rat_point_2::hy, Return_by_value())
    .def("hw", &Rat_point_2::hw, Return_by_value())
    .def("bbox", &Rat_point_2::bbox)
    .def("cartesian", &Rat_point_2::cartesian, Return_by_value())
    .def("__getitem__", &Rat_point_2::operator[], Return_by_value())
    .def("coordinates", bp::range<>(&Rat_point_2::cartesian_begin,
                                    &Rat_point_2::cartesian_end))
    .def("dimension", &Rat_point_2::dimension)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self > bp::self)
    .def(bp::self < bp::self)
    .def(bp::self <= bp::self)
    .def(bp::self >= bp::self)
    .def(bp::self - bp::self)
    // .def(bp::self += Vector_2())
    // .def(bp::self -= Vector_2())
    // .def(bp::self + Vector_2())
    // .def(bp::self - Vector_2())
    // .setattr("__hash__", &hash_rational_point<Rat_point_2>)
    .setattr("__doc__", "Rat_point_2")
    ;

  bp::class_<Rat_segment_2>("Rat_segment_2")
    .def(bp::init<const Rat_point_2&, const Rat_point_2&>())
    .def("source", &Rat_segment_2::source, Return_by_value())
    .def("target", &Rat_segment_2::target, Return_by_value())
    .def("vertex", &Rat_segment_2::vertex, Return_by_value())
    .def("point", &Rat_segment_2::point, Return_by_value())
    .def("__getitem__", &Rat_segment_2::operator[], Return_by_value())
    .def("min", &Rat_segment_2::min, Return_by_value())
    .def("max", &Rat_segment_2::max, Return_by_value())
    .def("opposite", &Rat_segment_2::opposite)
    .def("to_vector", &Rat_segment_2::to_vector)
    .def("supporting_line", &Rat_segment_2::supporting_line)
    .def("squared_length", &Rat_segment_2::squared_length)
    .def("direction", &Rat_segment_2::direction)
    .def("has_on", &Rat_segment_2::has_on)
    .def("collinear_has_on", &Rat_segment_2::collinear_has_on)
    .def("is_degenerate", &Rat_segment_2::is_degenerate)
    .def("is_horizontal", &Rat_segment_2::is_horizontal)
    .def("is_vertical", &Rat_segment_2::is_vertical)
    .def("bbox", &Rat_segment_2::bbox)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    // .setattr("__hash__", &hash<Rat_segment_2>)
    ;

  bp::class_<Rat_circle_2>("Rat_circle_2")
    .def(bp::init<>())
    .def(bp::init<const Rat_point_2&, const Rational&, CGAL::Orientation>())
    .def(bp::init<const Rat_point_2&, double, CGAL::Orientation>())
    .def(bp::init<const Rat_point_2&, const Rat_point_2&, CGAL::Orientation>())
    .def(bp::init<const Rat_point_2&, CGAL::Orientation>())
    .def(bp::init<const Rat_point_2&, const Rational&>())
    .def(bp::init<const Rat_point_2&, double>())
    .def(bp::init<const Rat_point_2&, const Rat_point_2&>())
    .def(bp::init<const Rat_point_2&>())
    .def(bp::init<const Rat_point_2&, const Rat_point_2&, const Rat_point_2&>())
    .def("center", &Rat_circle_2::center, Return_by_value())
    .def("squared_radius", &Rat_circle_2::squared_radius)
    .def("orientation", &Rat_circle_2::orientation)
    .def("is_degenerate", &Rat_circle_2::is_degenerate)
    .def("oriented_side", &Rat_circle_2::oriented_side)
    .def("bounded_side", &Rat_circle_2::bounded_side)
    .def("has_on_positive_side", &Rat_circle_2::has_on_positive_side)
    .def("has_on_negative_side", &Rat_circle_2::has_on_negative_side)
    .def("has_on_boundary", &Rat_circle_2::has_on_boundary)
    .def("has_on_bounded_side", &Rat_circle_2::has_on_bounded_side)
    .def("has_on_unbounded_side", &Rat_circle_2::has_on_unbounded_side)
    .def("orthogonal_transform", &Rat_circle_2::orthogonal_transform)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    //.setattr("__hash__", &hash<Rat_circle_2>)
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
    .def(bp::init<const Rational&, const Rational&, const Rational&,
         const Rational&, const Rational&, const Rational&,
         CGAL::Orientation,
         const GT::Point_2&,
         const Rational&, const Rational&, const Rational&,
         const Rational&, const Rational&, const Rational&,
         const GT::Point_2&,
         const Rational&, const Rational&, const Rational&,
         const Rational&, const Rational&, const Rational&>())
    .def(bp::init<int, int, int, int, int, int, CGAL::Orientation,
         const GT::Point_2&, int, int, int, int, int, int,
         const GT::Point_2&, int, int, int, int, int, int>())
    .def(bp::init<const Rat_point_2&, const Rat_point_2&, const Rat_point_2&>())
    .def(bp::init<const Rat_segment_2&>())
    .def(bp::init<const Rat_circle_2&>())
    .def(bp::init<const Rat_circle_2&, CGAL::Orientation, const GT::Point_2&, const GT::Point_2>())
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
