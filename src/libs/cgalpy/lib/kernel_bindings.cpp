// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>

#include "CGALPY/config.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/export_ft.hpp"

namespace bp = boost::python;

extern void export_gmpz();
extern void export_gmpq();

Kernel::Equal_2 kernel_equal_2(Kernel& k)
{ return (Kernel::Equal_2)(k.equal_2_object()); }

//template<typename T>
//size_t hash(T& immutable)
//{
//  std::ostringstream stream;
//  stream << immutable;
//  std::string s = stream.str();
//  return boost::hash<std::string>()(s); // TODO: two equal objects can have different string representation
//}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void bind_squared_distance_first_type() {
  bp::def<FT(const T1&, const T1&)>("squared_distance", &CGAL::squared_distance);
  bp::def<FT(const T1&, const T2&)>("squared_distance", &CGAL::squared_distance);
  bp::def<FT(const T1&, const T3&)>("squared_distance", &CGAL::squared_distance);
  bp::def<FT(const T1&, const T4&)>("squared_distance", &CGAL::squared_distance);
  bp::def<FT(const T1&, const T5&)>("squared_distance", &CGAL::squared_distance);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void bind_squared_distance_types() {
  bind_squared_distance_first_type< T1, T2, T3, T4, T5 >();
  bind_squared_distance_first_type< T2, T1, T3, T4, T5 >();
  bind_squared_distance_first_type< T3, T2, T1, T4, T5 >();
  bind_squared_distance_first_type< T4, T2, T3, T1, T5 >();
  bind_squared_distance_first_type< T5, T2, T3, T4, T1 >();
}

Point_2 transform_point(Aff_transformation_2& t, Point_2& p)
{ return t.transform(p); }

Vector_2 transform_vector(Aff_transformation_2& t, Vector_2 & v)
{ return t.transform(v); }

Direction_2 transform_direction(Aff_transformation_2& t, Direction_2& d)
{ return t.transform(d); }

Line_2 transform_line(Aff_transformation_2& t, Line_2& l)
{ return t.transform(l); }

// Determine whether the dD kernel is an an EPEC type.
// An EPEC type has a non trivial FT
constexpr bool is_epec_type() {
  return ((CGALPY_KERNEL == CGALPY_KERNEL_EPEC) ||
          (CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT) ||
          (CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ));
}

void export_kernel() {
  const bp::type_info info_gmpz = bp::type_id<CGAL::Gmpz>();
  const auto* reg_gmpz = bp::converter::registry::query(info_gmpz);
  if ((reg_gmpz == nullptr) || ((*reg_gmpz).m_to_python == nullptr))
    export_gmpz();
  else bp::scope().attr("Gmpz") = bp::handle<>(reg_gmpz->m_class_object);

  const bp::type_info info_gmpq = bp::type_id<CGAL::Gmpq>();
  const auto* reg_gmpq = bp::converter::registry::query(info_gmpq);
  if ((reg_gmpq == nullptr) || ((*reg_gmpq).m_to_python == nullptr))
    export_gmpq();
  else bp::scope().attr("Gmpq") = bp::handle<>(reg_gmpq->m_class_object);

#if ((CGALPY_KERNEL == CGALPY_KERNEL_EPEC) ||                           \
     (CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT) ||                 \
     (CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ))
  const bp::type_info info_ft = bp::type_id<FT>();
  const auto* reg_ft = bp::converter::registry::query(info_ft);
  if ((reg_ft == nullptr) || ((*reg_ft).m_to_python == nullptr)) {
    auto ftc = bp::class_<FT>("FT");
    export_ft<FT>(ftc);
  }
  else bp::scope().attr("FT") = bp::handle<>(reg_ft->m_class_object);
#endif

  //class_<RT>("RT")
  //  .def(init<RT::Exact_type>())
  //  .def(self_ns::str(self_ns::self))
  //  .def(self == self)
  //  ;

  bp::enum_<CGAL::Sign>("Result")

    //CGAL::Sign
    .value("NEGATIVE", CGAL::NEGATIVE)
    .value("ZERO", CGAL::ZERO)
    .value("POSITIVE", CGAL::POSITIVE)

    //CGAL::Comparison_result
    .value("SMALLER", CGAL::SMALLER)
    .value("EQUAL", CGAL::EQUAL)
    .value("LARGER", CGAL::LARGER)

    //CGAL::Oriented_side
    .value("ON_NEGATIVE_SIDE", CGAL::ON_NEGATIVE_SIDE)
    .value("ON_ORIENTED_BOUNDARY", CGAL::ON_ORIENTED_BOUNDARY)
    .value("ON_POSITIVE_SIDE", CGAL::ON_POSITIVE_SIDE)

    //CGAL::Orientation
    .value("LEFT_TURN", CGAL::LEFT_TURN)
    .value("RIGHT_TURN", CGAL::RIGHT_TURN)
    .value("COLLINEAR", CGAL::COLLINEAR)
    .value("CLOCKWISE", CGAL::CLOCKWISE)
    .value("COUNTERCLOCKWISE", CGAL::COUNTERCLOCKWISE)
    .value("COPLANAR", CGAL::COPLANAR)

    .export_values()
    ;

  bp::enum_<CGAL::Angle>("Angle")
    .value("OBTUSE", CGAL::OBTUSE)
    .value("RIGHT", CGAL::RIGHT)
    .value("ACUTE", CGAL::ACUTE)
    .export_values()
    ;

  bp::class_<Rotation>("Rotation")
    .def(bp::init<>())
    ;

  bp::class_<Scaling>("Scaling")
    .def(bp::init<>())
    ;

  bp::class_<Translation>("Translation")
    .def(bp::init<>())
    ;

  //bp::class_<Kernel>("Kernel")
  //  .def(bp::init<>())
  //  .def("equal_2_object", &kernel_equal_2)
  //  ;

  //bp::class_<Traits>("Traits")
  //  .def(bp::init<>())
  //  .def("equal_2_object", &Traits::equal_2_object)
  //  .def("compare_xy_2_object", &Traits::compare_xy_2_object)
  //  ;

  //bp::class_<Traits::Compare_xy_2>("Traits_compare_xy_2", no_init)
  //  .def<CGAL::Sign(Traits::Compare_xy_2::*)(const Point_2&, const Point_2&) const>("__call__", &Traits::Compare_xy_2::operator())
  //  ;

  //bp::class_<Traits::Equal_2>("Traits_equal_2_object", no_init)
  //  .def<bool (Traits::Equal_2::*)(const Point_2&, const Point_2&) const>("__call__", &Traits::Equal_2::operator())
  //  ;

  //bp::class_<Kernel::Equal_2>("Kernel_equal_2_object", no_init)
  //  //.def<bool (Kernel::Equal_2::*)(const Rational_point&, const Rational_point&) const>("__call__", &Kernel::Equal_2::operator())
  //  ;

  bp::class_<Point_2>("Point_2")
    .def(bp::init<>())
    .def(bp::init<double, double>())
    .def(bp::init<FT&, FT&>())
    .def(bp::init<RT&, RT&>())
    .def(bp::init<Point_2&>())
    .def("x", &Point_2::x, Kernel_return_value_policy())
    .def("y", &Point_2::y, Kernel_return_value_policy())
    .def("hx", &Point_2::hx, Kernel_return_value_policy())
    .def("hy", &Point_2::hy, Kernel_return_value_policy())
    .def("hw", &Point_2::hw, Kernel_return_value_policy())
    .def("bbox", &Point_2::bbox)
    .def("cartesian", &Point_2::cartesian, Kernel_return_value_policy())
    .def("__getitem__", &Point_2::operator[], Kernel_return_value_policy())
#if ((CGALPY_KERNEL == CGALPY_KERNEL_EPIC) ||                           \
     (CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_DOUBLE))
    // TODO: Returning address of local variable or temporary with EPEC kernel
    .def("coordinates", bp::range<>(&Point_2::cartesian_begin, &Point_2::cartesian_end))
#endif
    .def("dimension", &Point_2::dimension)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self > bp::self)
    .def(bp::self < bp::self)
    .def(bp::self <= bp::self)
    .def(bp::self >= bp::self)
    .def(bp::self - bp::self)
    .def(bp::self += Vector_2())
    .def(bp::self -= Vector_2())
    .def(bp::self + Vector_2())
    .def(bp::self - Vector_2())
    .setattr("__hash__", &hash_rational_point<is_epec_type(), Point_2>)
    .setattr("__doc__", "AAAAAAAAAAAAAAAA")
    ;

  bp::class_<Segment_2>("Segment_2")
    .def(bp::init<Point_2&, Point_2&>())
    .def("source", &Segment_2::source, Kernel_return_value_policy())
    .def("target", &Segment_2::target, Kernel_return_value_policy())
    .def("vertex", &Segment_2::vertex, Kernel_return_value_policy())
    .def("point", &Segment_2::point, Kernel_return_value_policy())
    .def("__getitem__", &Segment_2::operator[], Kernel_return_value_policy())
    .def("min", &Segment_2::min, Kernel_return_value_policy())
    .def("max", &Segment_2::max, Kernel_return_value_policy())
    .def("opposite", &Segment_2::opposite)
    .def("to_vector", &Segment_2::to_vector)
    .def("supporting_line", &Segment_2::supporting_line)
    .def("squared_length", &Segment_2::squared_length)
    .def("direction", &Segment_2::direction)
    .def("has_on", &Segment_2::has_on)
    .def("collinear_has_on", &Segment_2::collinear_has_on)
    .def("is_degenerate", &Segment_2::is_degenerate)
    .def("is_horizontal", &Segment_2::is_horizontal)
    .def("is_vertical", &Segment_2::is_vertical)
    .def("bbox", &Segment_2::bbox)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    // .setattr("__hash__", &hash<Segment_2>)
    ;

  bp::class_<Line_2>("Line_2")
    .def(bp::init<RT&, RT&, RT&>())
    .def(bp::init<Point_2&, Point_2&>())
    .def(bp::init<Point_2&, Direction_2&>())
    .def(bp::init<Point_2&, Vector_2&>())
    .def(bp::init<Segment_2&>())
    .def(bp::init<Ray_2&>())
    .def("a", &Line_2::a)
    .def("b", &Line_2::b)
    .def("c", &Line_2::c)
    .def("is_degenerate", &Line_2::is_degenerate)
    .def("is_horizontal", &Line_2::is_horizontal)
    .def("is_vertical", &Line_2::is_vertical)
    .def("oriented_side", &Line_2::oriented_side)
    .def("has_on", &Line_2::has_on)
    .def("has_on_boundary", &Line_2::has_on_boundary)
    .def("has_on_negative_side", &Line_2::has_on_negative_side)
    .def("has_on_positive_side", &Line_2::has_on_positive_side)
    .def("projection", &Line_2::projection)
    .def("direction", &Line_2::direction)
    .def("to_vector", &Line_2::to_vector)
    .def("opposite", &Line_2::opposite)
    .def("transform", &Line_2::transform)
    .def("perpendicular", &Line_2::perpendicular)
    .def("x_at_y", &Line_2::x_at_y)
    .def("y_at_x", &Line_2::y_at_x)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    //.setattr("__hash__", &hash<Line_2>)
    ;

  bp::class_<Ray_2>("Ray_2")
    .def(bp::init<Point_2&, Point_2&>())
    .def(bp::init<Point_2&, Direction_2&>())
    .def(bp::init<Point_2&, Vector_2&>())
    .def(bp::init<Point_2&, Line_2&>())
    .def("is_degenerate", &Ray_2::is_degenerate)
    .def("is_horizontal", &Ray_2::is_horizontal)
    .def("is_vertical", &Ray_2::is_vertical)
    .def("direction", &Ray_2::direction)
    .def("to_vector", &Ray_2::to_vector)
    .def("has_on", &Ray_2::has_on)
    .def("collinear_has_on", &Ray_2::collinear_has_on)
    .def("point", &Ray_2::point)
    .def("supporting_line", &Ray_2::supporting_line)
    .def("opposite", &Ray_2::opposite)
    .def("transform", &Ray_2::transform)
    .def("source", &Ray_2::source, Kernel_return_value_policy())
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    //.setattr("__hash__", &hash<Ray_2>)
    ;

  bp::class_<Triangle_2>("Triangle_2")
    .def(bp::init < Point_2&, Point_2&, Point_2&>())
    .def("vertex", &Triangle_2::vertex, Kernel_return_value_policy())
    .def("__getitem__", &Triangle_2::operator[], Kernel_return_value_policy())
    .def("is_degenerate", &Triangle_2::is_degenerate)
    .def("orientation", &Triangle_2::orientation)
    .def("oriented_side", &Triangle_2::oriented_side)
    .def("bounded_side", &Triangle_2::bounded_side)
    .def("has_on_positive_side", &Triangle_2::has_on_positive_side)
    .def("has_on_negative_side", &Triangle_2::has_on_negative_side)
    .def("has_on_boundary", &Triangle_2::has_on_boundary)
    .def("has_on_bounded_side", &Triangle_2::has_on_bounded_side)
    .def("has_on_unbounded_side", &Triangle_2::has_on_unbounded_side)
    .def("opposite", &Triangle_2::opposite)
    .def("area", &Triangle_2::area)
    .def("bbox", &Triangle_2::bbox)
    .def("transform", &Triangle_2::transform)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    //.setattr("__hash__", &hash<Triangle_2>)
    ;

  bp::class_<Iso_rectangle_2>("Iso_rectangle_2")
    .def(bp::init<Point_2&, Point_2&>())
    .def(bp::init<Point_2&, Point_2&, int>())
    .def(bp::init<Point_2&, Point_2&, Point_2&, Point_2&>())
    .def(bp::init<RT&, RT&, RT&, RT&, RT&>())
    .def(bp::init<RT, RT, RT, RT>())
    .def(bp::init<Bbox_2&>())
    .def("vertex", &Iso_rectangle_2::vertex)
    .def("__getitem__", &Iso_rectangle_2::operator[])
    .def("xmin", &Iso_rectangle_2::xmin, Kernel_return_value_policy())
    .def("ymin", &Iso_rectangle_2::ymin, Kernel_return_value_policy())
    .def("xmax", &Iso_rectangle_2::xmax, Kernel_return_value_policy())
    .def("ymax", &Iso_rectangle_2::ymax, Kernel_return_value_policy())
    .def("min", &Iso_rectangle_2::min, Kernel_return_value_policy())
    .def("max", &Iso_rectangle_2::max, Kernel_return_value_policy())
    .def("min_coord", &Iso_rectangle_2::min_coord, Kernel_return_value_policy())
    .def("max_coord", &Iso_rectangle_2::max_coord, Kernel_return_value_policy())
    .def("is_degenerate", &Iso_rectangle_2::is_degenerate)
    .def("bounded_side", &Iso_rectangle_2::bounded_side)
    .def("has_on_boundary", &Iso_rectangle_2::has_on_boundary)
    .def("has_on_bounded_side", &Iso_rectangle_2::has_on_bounded_side)
    .def("has_on_unbounded_side", &Iso_rectangle_2::has_on_unbounded_side)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    //.setattr("__hash__", &hash<Iso_rectangle_2>)
    ;

  bp::class_<Circle_2>("Circle_2")
    .def(bp::init<>())
    .def(bp::init<Point_2&, FT&, CGAL::Orientation>())
    .def(bp::init<Point_2&, double, CGAL::Orientation>())
    .def(bp::init<Point_2&, Point_2&, CGAL::Orientation>())
    .def(bp::init<Point_2&, CGAL::Orientation>())
    .def(bp::init<Point_2&, FT&>())
    .def(bp::init<Point_2&, double>())
    .def(bp::init<Point_2&, Point_2&>())
    .def(bp::init<Point_2&>())
    .def(bp::init<Point_2&, Point_2&, Point_2&>())
    .def("center", &Circle_2::center, Kernel_return_value_policy())
    .def("squared_radius", &Circle_2::squared_radius)
    .def("orientation", &Circle_2::orientation)
    .def("is_degenerate", &Circle_2::is_degenerate)
    .def("oriented_side", &Circle_2::oriented_side)
    .def("bounded_side", &Circle_2::bounded_side)
    .def("has_on_positive_side", &Circle_2::has_on_positive_side)
    .def("has_on_negative_side", &Circle_2::has_on_negative_side)
    .def("has_on_boundary", &Circle_2::has_on_boundary)
    .def("has_on_bounded_side", &Circle_2::has_on_bounded_side)
    .def("has_on_unbounded_side", &Circle_2::has_on_unbounded_side)
    .def("orthogonal_transform", &Circle_2::orthogonal_transform)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    //.setattr("__hash__", &hash<Circle_2>)
    ;

  bp::class_<Direction_2>("Direction_2")
    .def(bp::init<Vector_2>())
    .def(bp::init<Line_2>())
    .def(bp::init<Ray_2>())
    .def(bp::init<Segment_2>())
    .def(bp::init<RT&, RT&>())
    .def(bp::init<double, double>())
    .def("dx", &Direction_2::dx, Kernel_return_value_policy())
    .def("dy", &Direction_2::dy, Kernel_return_value_policy())
    .def("vector", &Direction_2::vector)
    .def("transform", &Direction_2::transform)
    .def("counterclockwise_in_between", &Direction_2::counterclockwise_in_between)
    .def("delta", &Direction_2::delta, Kernel_return_value_policy())
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self != bp::self)
    .def(bp::self < bp::self)
    .def(bp::self > bp::self)
    .def(bp::self <= bp::self)
    .def(bp::self >= bp::self)
    .def(-bp::self)
    //.setattr("__hash__", &hash<Direction_2>)
    ;

  bp::class_<Vector_2>("Vector_2")
    .def(bp::init<Point_2&, Point_2&>())
    .def(bp::init<Line_2>())
    .def(bp::init<Ray_2>())
    .def(bp::init<Segment_2>())
    .def(bp::init<FT&, FT&, FT&>())
    .def(bp::init<FT&, FT&>())
    .def(bp::init<double, double>())
    .def("hx", &Vector_2::hx, Kernel_return_value_policy())
    .def("hy", &Vector_2::hy, Kernel_return_value_policy())
    .def("hw", &Vector_2::hw, Kernel_return_value_policy())
    .def("x", &Vector_2::x, Kernel_return_value_policy())
    .def("y", &Vector_2::y, Kernel_return_value_policy())
    .def("squared_length", &Vector_2::squared_length)
    .def("homogeneous", &Vector_2::homogeneous, Kernel_return_value_policy())
    .def("cartesian", &Vector_2::cartesian, Kernel_return_value_policy())
    .def("__getitem__", &Vector_2::operator[], Kernel_return_value_policy())
#if CGALPY_KERNEL == CGALPY_KERNEL_EPIC
    // TODO: Returning address of local variable or temporary with EPEC kernel
    .def("cartesian_coordinates", bp::range<>(&Vector_2::cartesian_begin, & Vector_2::cartesian_end))
#endif
    .def("dimension", &Vector_2::dimension)
    .def("direction", &Vector_2::direction)
    .def("transform", &Vector_2::transform)
    .def("perpendicular", &Vector_2::perpendicular)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self != bp::self)
    .def(bp::self + bp::self)
    .def(bp::self += bp::self)
    .def(bp::self - bp::self)
    .def(bp::self -= bp::self)
    .def(-bp::self)
    .def(bp::self * bp::self)
    //.def(self*RT())
    .def(bp::self * FT())
    //.def(RT() * bp::self)
    .def(FT()*bp::self)
    //.def(bp::self *= RT())
    .def(bp::self*=FT())
    //.def(bp::self / RT())
    .def(bp::self / FT())
    //.def(bp::self /= RT())
    .def(bp::self /= FT())
    //.setattr("__hash__", &hash<Vector_2>)
    ;

  bp::class_<Bbox_2>("Bbox_2")
    .def(bp::init<>())
    .def(bp::init<double, double, double, double>())
    .def("dimension", &Bbox_2::dimension)
    .def("dilate", &Bbox_2::dilate)
    .def("xmin", &Bbox_2::xmin)
    .def("ymin", &Bbox_2::ymin)
    .def("xmax", &Bbox_2::xmax)
    .def("ymax", &Bbox_2::ymax)
    .def("min", &Bbox_2::min)
    .def("max", &Bbox_2::max)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self += bp::self)
    .def(bp::self + bp::self)
    ;

  bp::class_<Point_3>("Point_3")
    .def(bp::init<>())
    .def(bp::init<double, double, double>())
    .def(bp::init<FT&, FT&, FT&>())
    .def(bp::init<RT&, RT&, RT&>())
    .def("x", &Point_3::x, Kernel_return_value_policy())
    .def("y", &Point_3::y, Kernel_return_value_policy())
    .def("z", &Point_3::z, Kernel_return_value_policy())
    .def("hx", &Point_3::hx, Kernel_return_value_policy())
    .def("hy", &Point_3::hy, Kernel_return_value_policy())
    .def("hz", &Point_3::hz, Kernel_return_value_policy())
    .def("hw", &Point_3::hw, Kernel_return_value_policy())
    .def("dimension", &Point_2::dimension)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self > bp::self)
    .def(bp::self < bp::self)
    .def(bp::self <= bp::self)
    .def(bp::self >= bp::self)
    .def(bp::self - bp::self)
    //.setattr("__hash__", &hash<Point_3>)
    ;

  bp::class_<Weighted_point_3>("Weighted_point_3")
    .def(bp::init<>())
    .def(bp::init<const CGAL::Origin&>())
    .def(bp::init<const Point_3&>())
    .def(bp::init<const Point_3&, const FT&>())
    .def(bp::init<const FT&, const FT&, const FT&>())
    // Accessors
    .def("point", &Weighted_point_3::point, Kernel_return_value_policy())
    .def("weight", &Weighted_point_3::weight, Kernel_return_value_policy())
    .def("x", &Weighted_point_3::x, Kernel_return_value_policy())
    .def("y", &Weighted_point_3::y, Kernel_return_value_policy())
    .def("z", &Weighted_point_3::z, Kernel_return_value_policy())
    .def("hx", &Weighted_point_3::hx, Kernel_return_value_policy())
    .def("hy", &Weighted_point_3::hy, Kernel_return_value_policy())
    .def("hz", &Weighted_point_3::hz, Kernel_return_value_policy())
    .def("hw", &Weighted_point_3::hw, Kernel_return_value_policy())
    // Operations
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    // Convenient operations
    .def("homogeneous", &Weighted_point_3::homogeneous)
    .def("cartesian", &Weighted_point_3::cartesian, Kernel_return_value_policy())
    // Kernel::FT 	operator[] (int i) const
    // Cartesian_const_iterator 	cartesian_begin () const
    // Cartesian_const_iterator 	cartesian_end () const
    .def("dimension", &Weighted_point_3::dimension)
    .def("bbox", &Weighted_point_3::bbox)
    // .def("transform", &Weighted_point_3::transform)
    //.setattr("__hash__", &hash<Point_3>)
    ;

  bp::class_<Aff_transformation_2>("Aff_transformation_2")
    .def(bp::init<>())
    .def(bp::init<RT&, RT&, RT&, RT&, RT&>())
    .def(bp::init<RT, RT, RT, RT>())
    .def(bp::init<RT&, RT&, RT&, RT&, RT&, RT&, RT&>())
    .def(bp::init<RT, RT, RT, RT, RT, RT, RT>())
    .def(bp::init<const Translation, const Vector_2&>())
    .def(bp::init<const Rotation, const Direction_2&, const RT&, const RT&>())
    .def(bp::init<const Rotation, const Direction_2&, const RT, const RT>())
    .def(bp::init<const Rotation, const RT&, const RT&, const RT&>())
    .def(bp::init<const Rotation, const RT, const RT, const RT>())
    .def(bp::init<Scaling, const RT&, const RT&>())
    .def(bp::init<Scaling, const RT, const RT>())
    .def("transform", transform_point)
    .def("transform", transform_vector)
    .def("transform", transform_direction)
    .def("transform", transform_line)
    .def("inverse", &Aff_transformation_2::inverse)
    .def("is_even", &Aff_transformation_2::is_even)
    .def("is_odd", &Aff_transformation_2::is_odd)
    .def("cartesian", &Aff_transformation_2::cartesian)
    .def("m", &Aff_transformation_2::m)
    .def("homogeneous", &Aff_transformation_2::homogeneous)
    .def("hm", &Aff_transformation_2::hm)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    //.def(self == bp::self)
    .def(bp::self * bp::self)
    ;

  bp::class_<Aff_transformation_3>("Aff_transformation_3")
    .def(bp::init<>())
    ;

  //Global kernel functions
  bp::def<CGAL::Angle(const Vector_2&, const Vector_2&)>("angle", &CGAL::angle);
  bp::def<CGAL::Angle(const Point_2&, const Point_2&, const Point_2&)>("angle", &CGAL::angle);
  bp::def<CGAL::Angle(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("angle", &CGAL::angle);

  bp::def<FT (const Point_2&, const Point_2&, const Point_2&)>("area", &CGAL::area);

  bp::def<bool(const Point_2&, const Point_2&, const Point_2&)>("are_ordered_along_line", &CGAL::are_ordered_along_line);

  bp::def<bool(const Point_2&, const Point_2&, const Point_2&)>("are_strictly_ordered_along_line", &CGAL::are_strictly_ordered_along_line);

  bp::def<Point_2(const Point_2&, const FT&, const Point_2&, const FT&)>("barycenter", &CGAL::barycenter);
  bp::def<Point_2(const Point_2&, const FT&, const Point_2&, const FT&, const Point_2&, const FT&)>("barycenter", &CGAL::barycenter);
  bp::def<Point_2(const Point_2&, const FT&, const Point_2&, const FT&, const Point_2&, const FT&, const Point_2&, const FT&)>("barycenter", &CGAL::barycenter);

  bp::def<Line_2(const Point_2&, const Point_2&)>("bisector", &CGAL::bisector);

  // Requires sqrt operation
  //def<Line_2(const Line_2&, const Line_2&)>("bisector", &CGAL::bisector);

  bp::def<Point_2(const Point_2&, const Point_2&, const Point_2&)>("centroid", &CGAL::centroid);
  bp::def<Point_2(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("centroid", &CGAL::centroid);
  bp::def<Point_2(const Triangle_2&)>("centroid", &CGAL::centroid);

  bp::def<Point_2(const Point_2&, const Point_2&)>("circumcenter", &CGAL::circumcenter);
  bp::def<Point_2(const Point_2&, const Point_2&, const Point_2&)>("circumcenter", &CGAL::circumcenter);
  bp::def<Point_2(const Triangle_2&)>("circumcenter", &CGAL::circumcenter);

  bp::def<bool(const Point_2&, const Point_2&, const Point_2&)>("collinear_are_ordered_along_line", &CGAL::collinear_are_ordered_along_line);

  bp::def<bool(const Point_2&, const Point_2&, const Point_2&)>("collinear_are_strictly_ordered_along_line", &CGAL::collinear_are_strictly_ordered_along_line);

  bp::def<bool(const Point_2&, const Point_2&, const Point_2&)>("collinear", &CGAL::collinear);

  bp::def<CGAL::Comparison_result(const Point_2&, const Point_2&, const Point_2&)>("compare_distance_to_point", &CGAL::compare_distance_to_point);

  bp::def <CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_lexicographically", &CGAL::compare_lexicographically);

  bp::def<CGAL::Comparison_result(const Line_2&, const Point_2&, const Point_2&)>("compare_signed_distance_to_line", &CGAL::compare_signed_distance_to_line);

  bp::def<CGAL::Comparison_result(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("compare_signed_distance_to_line", &CGAL::compare_signed_distance_to_line);
  bp::def<CGAL::Comparison_result(const Line_2&, const Point_2&, const Point_2&)>("compare_signed_distance_to_line", &CGAL::compare_signed_distance_to_line);

  bp::def<CGAL::Comparison_result(const Line_2&, const Line_2&)>("compare_slope", &CGAL::compare_slope);
  bp::def<CGAL::Comparison_result(const Segment_2&, const Segment_2&)>("compare_slope", &CGAL::compare_slope);

  bp::def<CGAL::Comparison_result(const Point_2&, const Point_2&, const FT&)>("compare_squared_distance", &CGAL::compare_squared_distance);

  bp::def<CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_x", &CGAL::compare_x);
  bp::def<CGAL::Comparison_result(const Point_2&, const Line_2&, const Line_2&)>("compare_x", &CGAL::compare_x);
  bp::def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_x", &CGAL::compare_x);
  bp::def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_x", &CGAL::compare_x);

  bp::def<CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_xy", &CGAL::compare_xy);

  bp::def<CGAL::Comparison_result(const Point_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y);
  bp::def<CGAL::Comparison_result(const Point_2&, const Line_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y);
  bp::def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y);
  bp::def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y);

  bp::def<CGAL::Comparison_result(const Point_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  bp::def<CGAL::Comparison_result(const Point_2&, const Line_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  bp::def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  bp::def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  bp::def<CGAL::Comparison_result(const Point_2&, const Segment_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  bp::def<CGAL::Comparison_result(const Point_2&, const Segment_2&, const Segment_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);

  bp::def<CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_y", &CGAL::compare_y);
  bp::def<CGAL::Comparison_result(const Point_2&, const Line_2&, const Line_2&)>("compare_y", &CGAL::compare_y);
  bp::def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_y", &CGAL::compare_y);
  bp::def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_y", &CGAL::compare_y);

  bp::def<CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_yx", &CGAL::compare_yx);
  bp::def<FT(const Vector_2&, const Vector_2&)>("determinant", &CGAL::determinant);

  bp::def<bool (const Point_2&, const Point_2&, const Point_2&)>("has_larger_distace_to_point", &CGAL::has_larger_distance_to_point);

  bp::def<bool(const Line_2&, const Point_2&, const Point_2&)>("has_larger_signed_distance_to_line", &CGAL::has_larger_signed_distance_to_line);
  bp::def<bool(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("has_larger_signed_distance_to_line", &CGAL::has_larger_signed_distance_to_line);

  bp::def<bool(const Point_2&, const Point_2&, const Point_2&)>("has_smaller_distace_to_point", &CGAL::has_smaller_distance_to_point);

  bp::def<bool(const Line_2&, const Point_2&, const Point_2&)>("has_smaller_signed_distance_to_line", &CGAL::has_smaller_signed_distance_to_line);
  bp::def<bool(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("has_smaller_signed_distance_to_line", &CGAL::has_smaller_signed_distance_to_line);

  bp::def<FT(const Point_2&, const Point_2&)>("l_infinity_distance", &CGAL::l_infinity_distance);

  bp::def<bool(const Point_2&, const Point_2&, const Point_2&)>("left_turn", &CGAL::left_turn);

  bp::def<bool(const Point_2&, const Point_2&)>("lexicographically_xy_larger", &CGAL::lexicographically_xy_larger);

  bp::def<bool(const Point_2&, const Point_2&)>("lexicographically_xy_larger_or_equal", &CGAL::lexicographically_xy_larger_or_equal);

  bp::def<bool(const Point_2&, const Point_2&)>("lexicographically_xy_smaller", &CGAL::lexicographically_xy_smaller);

  bp::def<bool(const Point_2&, const Point_2&)>("lexicographically_xy_smaller_or_equal", &CGAL::lexicographically_xy_smaller_or_equal);

  bp::def<Point_2(const Iso_rectangle_2&)>("max_vertex", &CGAL::max_vertex);

  bp::def<Point_2(const Point_2&, const Point_2&)>("midpoint", &CGAL::midpoint);

  bp::def<Point_2(const Iso_rectangle_2&)>("min_vertex", &CGAL::min_vertex);

  bp::def<CGAL::Orientation(const Point_2&, const Point_2&, const Point_2&)>("orientation", CGAL::orientation);
  bp::def<CGAL::Orientation(const Vector_2&, const Vector_2&)>("orientation", CGAL::orientation);

  bp::def<bool(const Line_2&, const Line_2&)>("parallel", CGAL::parallel);
  bp::def<bool(const Ray_2&, const Ray_2&)>("parallel", CGAL::parallel);
  bp::def<bool(const Segment_2&, const Segment_2&)>("parallel", CGAL::parallel);

  bp::def<Line_2 (const Circle_2&, const Circle_2&)>("radical_line", &CGAL::radical_line);

  bp::def<void(const RT&, const RT&, RT&, RT&, RT&, const RT&, const RT&)>("rational_rotation_approximation", &CGAL::rational_rotation_approximation);

  bp::def<bool(const Point_2&, const Point_2&, const Point_2&)>("right_turn", &CGAL::right_turn);

  bp::def<FT(const Vector_2&, const Vector_2&)>("scalar_product", &CGAL::scalar_product);

  bp::def<CGAL::Bounded_side(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("side_of_bounded_circle", &CGAL::side_of_bounded_circle);
  bp::def<CGAL::Bounded_side(const Point_2&, const Point_2&, const Point_2&)>("side_of_bounded_circle", &CGAL::side_of_bounded_circle);

  bp::def<CGAL::Oriented_side(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("side_of_oriented_circle", &CGAL::side_of_oriented_circle);

  bind_squared_distance_types<Point_2, Line_2, Ray_2, Segment_2, Triangle_2>();

  bp::def<FT(const Point_2&, const Point_2&, const Point_2&)>("squared_radius", &CGAL::squared_radius);
  bp::def<FT(const Point_2&, const Point_2&)>("squared_radius", &CGAL::squared_radius);
  bp::def<FT(const Point_2&)>("squared_radius", &CGAL::squared_radius);

  bp::def<bool(const Point_2&, const Point_2&)>("x_equal", &CGAL::x_equal);

  bp::def<bool(const Point_2&, const Point_2&)>("y_equal", &CGAL::y_equal);

  bp::def<bool(const Bbox_2&, const Bbox_2&)>("do_overlap", &CGAL::do_overlap);
}
