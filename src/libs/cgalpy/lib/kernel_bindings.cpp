// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/to_string.hpp"

#include "CGALPY/common.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/export_ft.hpp"

namespace py = nanobind;

extern void export_gmpz(py::module_&);
extern void export_gmpq(py::module_&);

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
void bind_squared_distance_first_type(py::module_& m) {
  m.def<FT(const T1&, const T1&)>("squared_distance", &CGAL::squared_distance);
  m.def<FT(const T1&, const T2&)>("squared_distance", &CGAL::squared_distance);
  m.def<FT(const T1&, const T3&)>("squared_distance", &CGAL::squared_distance);
  m.def<FT(const T1&, const T4&)>("squared_distance", &CGAL::squared_distance);
  m.def<FT(const T1&, const T5&)>("squared_distance", &CGAL::squared_distance);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void bind_squared_distance_types(py::module_& m) {
  bind_squared_distance_first_type< T1, T2, T3, T4, T5 >(m);
  bind_squared_distance_first_type< T2, T1, T3, T4, T5 >(m);
  bind_squared_distance_first_type< T3, T2, T1, T4, T5 >(m);
  bind_squared_distance_first_type< T4, T2, T3, T1, T5 >(m);
  bind_squared_distance_first_type< T5, T2, T3, T4, T1 >(m);
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

void export_kernel(py::module_& m) {
  const py::type_info info_gmpz = py::type_id<CGAL::Gmpz>();
  const auto* reg_gmpz = py::converter::registry::query(info_gmpz);
  if ((reg_gmpz == nullptr) || ((*reg_gmpz).m_to_python == nullptr))
    export_gmpz(m);
  else py::scope().attr("Gmpz") = py::handle<>(reg_gmpz->m_class_object);

  const py::type_info info_gmpq = py::type_id<CGAL::Gmpq>();
  const auto* reg_gmpq = py::converter::registry::query(info_gmpq);
  if ((reg_gmpq == nullptr) || ((*reg_gmpq).m_to_python == nullptr))
    export_gmpq(m);
  else py::scope().attr("Gmpq") = py::handle<>(reg_gmpq->m_class_object);

#if ((CGALPY_KERNEL == CGALPY_KERNEL_EPEC) ||                           \
     (CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT) ||                 \
     (CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ))
  const py::type_info info_ft = py::type_id<FT>();
  const auto* reg_ft = py::converter::registry::query(info_ft);
  if ((reg_ft == nullptr) || ((*reg_ft).m_to_python == nullptr)) {
    auto ftc = py::class_<FT>(m, "FT");
    export_ft<FT>(ftc);
  }
  else py::scope().attr("FT") = py::handle<>(reg_ft->m_class_object);
#endif

  //class_<RT>(m, "RT")
  //  .def(init<RT::Exact_type>())
  //  .def(self_ns::str(self_ns::self))
  //  .def(self == self)
  //  ;

  py::enum_<CGAL::Sign>(m, "Result")

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

  py::enum_<CGAL::Angle>(m, "Angle")
    .value("OBTUSE", CGAL::OBTUSE)
    .value("RIGHT", CGAL::RIGHT)
    .value("ACUTE", CGAL::ACUTE)
    .export_values()
    ;

  py::class_<Rotation>(m, "Rotation")
    .def(py::init<>())
    ;

  py::class_<Scaling>(m, "Scaling")
    .def(py::init<>())
    ;

  py::class_<Translation>(m, "Translation")
    .def(py::init<>())
    ;

  //py::class_<Kernel>("Kernel")
  //  .def(py::init<>())
  //  .def("equal_2_object", &kernel_equal_2)
  //  ;

  //py::class_<Traits>("Traits")
  //  .def(py::init<>())
  //  .def("equal_2_object", &Traits::equal_2_object)
  //  .def("compare_xy_2_object", &Traits::compare_xy_2_object)
  //  ;

  //py::class_<Traits::Compare_xy_2>("Traits_compare_xy_2", no_init)
  //  .def<CGAL::Sign(Traits::Compare_xy_2::*)(const Point_2&, const Point_2&) const>("__call__", &Traits::Compare_xy_2::operator())
  //  ;

  //py::class_<Traits::Equal_2>("Traits_equal_2_object", no_init)
  //  .def<bool (Traits::Equal_2::*)(const Point_2&, const Point_2&) const>("__call__", &Traits::Equal_2::operator())
  //  ;

  //py::class_<Kernel::Equal_2>("Kernel_equal_2_object", no_init)
  //  //.def<bool (Kernel::Equal_2::*)(const Rational_point&, const Rational_point&) const>("__call__", &Kernel::Equal_2::operator())
  //  ;

  py::class_<Point_2>(m, "Point_2")
    .def(py::init<>())
    .def(py::init<double, double>())
    .def(py::init<FT&, FT&>())
    .def(py::init<RT&, RT&>())
    .def(py::init<Point_2&>())
    .def("x", &Point_2::x)
    .def("y", &Point_2::y)
    .def("hx", &Point_2::hx)
    .def("hy", &Point_2::hy)
    .def("hw", &Point_2::hw)
    .def("bbox", &Point_2::bbox)
    .def("cartesian", &Point_2::cartesian)
    .def("__getitem__", &Point_2::operator[])
#if ((CGALPY_KERNEL == CGALPY_KERNEL_EPIC) ||                           \
     (CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_DOUBLE))
    // TODO: Returning address of local variable or temporary with EPEC kernel
    .def("coordinates", py::range<>(&Point_2::cartesian_begin, &Point_2::cartesian_end))
#endif
    .def("dimension", &Point_2::dimension)
    .def("__str__", to_string<Point_2>)
    .def("__repr__", to_string<Point_2>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self > py::self)
    .def(py::self < py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def(py::self - py::self)
    .def(py::self += Vector_2())
    .def(py::self -= Vector_2())
    .def(py::self + Vector_2())
    .def(py::self - Vector_2())
    .setattr("__hash__", &hash_rational_point<is_epec_type(), Point_2>)
    .setattr("__doc__", "AAAAAAAAAAAAAAAA")
    ;

  py::class_<Segment_2>(m, "Segment_2")
    .def(py::init<Point_2&, Point_2&>())
    .def("source", &Segment_2::source)
    .def("target", &Segment_2::target)
    .def("vertex", &Segment_2::vertex)
    .def("point", &Segment_2::point)
    .def("__getitem__", &Segment_2::operator[])
    .def("min", &Segment_2::min)
    .def("max", &Segment_2::max)
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
    .def("__str__", to_string<Segment_2>)
    .def("__repr__", to_string<Segment_2>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    // .setattr("__hash__", &hash<Segment_2>)
    ;

  py::class_<Line_2>(m, "Line_2")
    .def(py::init<RT&, RT&, RT&>())
    .def(py::init<Point_2&, Point_2&>())
    .def(py::init<Point_2&, Direction_2&>())
    .def(py::init<Point_2&, Vector_2&>())
    .def(py::init<Segment_2&>())
    .def(py::init<Ray_2&>())
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
    .def("__str__", to_string<Line_2>)
    .def("__repr__", to_string<Line_2>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    //.setattr("__hash__", &hash<Line_2>)
    ;

  py::class_<Ray_2>(m, "Ray_2")
    .def(py::init<Point_2&, Point_2&>())
    .def(py::init<Point_2&, Direction_2&>())
    .def(py::init<Point_2&, Vector_2&>())
    .def(py::init<Point_2&, Line_2&>())
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
    .def("source", &Ray_2::source)
    .def("__str__", to_string<Ray_2>)
    .def("__repr__", to_string<Ray_2>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    //.setattr("__hash__", &hash<Ray_2>)
    ;

  py::class_<Triangle_2>(m, "Triangle_2")
    .def(py::init < Point_2&, Point_2&, Point_2&>())
    .def("vertex", &Triangle_2::vertex)
    .def("__getitem__", &Triangle_2::operator[])
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
    .def("__str__", to_string<Triangle_2>)
    .def("__repr__", to_string<Triangle_2>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    //.setattr("__hash__", &hash<Triangle_2>)
    ;

  py::class_<Iso_rectangle_2>(m, "Iso_rectangle_2")
    .def(py::init<Point_2&, Point_2&>())
    .def(py::init<Point_2&, Point_2&, int>())
    .def(py::init<Point_2&, Point_2&, Point_2&, Point_2&>())
    .def(py::init<RT&, RT&, RT&, RT&, RT&>())
    .def(py::init<RT, RT, RT, RT>())
    .def(py::init<Bbox_2&>())
    .def("vertex", &Iso_rectangle_2::vertex)
    .def("__getitem__", &Iso_rectangle_2::operator[])
    .def("xmin", &Iso_rectangle_2::xmin)
    .def("ymin", &Iso_rectangle_2::ymin)
    .def("xmax", &Iso_rectangle_2::xmax)
    .def("ymax", &Iso_rectangle_2::ymax)
    .def("min", &Iso_rectangle_2::min)
    .def("max", &Iso_rectangle_2::max)
    .def("min_coord", &Iso_rectangle_2::min_coord)
    .def("max_coord", &Iso_rectangle_2::max_coord)
    .def("is_degenerate", &Iso_rectangle_2::is_degenerate)
    .def("bounded_side", &Iso_rectangle_2::bounded_side)
    .def("has_on_boundary", &Iso_rectangle_2::has_on_boundary)
    .def("has_on_bounded_side", &Iso_rectangle_2::has_on_bounded_side)
    .def("has_on_unbounded_side", &Iso_rectangle_2::has_on_unbounded_side)
    .def("__str__", to_string<Iso_rectangle_2>)
    .def("__repr__", to_string<Iso_rectangle_2>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    //.setattr("__hash__", &hash<Iso_rectangle_2>)
    ;

  py::class_<Circle_2>(m, "Circle_2")
    .def(py::init<>())
    .def(py::init<Point_2&, FT&, CGAL::Orientation>())
    .def(py::init<Point_2&, double, CGAL::Orientation>())
    .def(py::init<Point_2&, Point_2&, CGAL::Orientation>())
    .def(py::init<Point_2&, CGAL::Orientation>())
    .def(py::init<Point_2&, FT&>())
    .def(py::init<Point_2&, double>())
    .def(py::init<Point_2&, Point_2&>())
    .def(py::init<Point_2&>())
    .def(py::init<Point_2&, Point_2&, Point_2&>())
    .def("center", &Circle_2::center)
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
    .def("__str__", to_string<Circle_2>)
    .def("__repr__", to_string<Circle_2>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    //.setattr("__hash__", &hash<Circle_2>)
    ;

  py::class_<Direction_2>(m, "Direction_2")
    .def(py::init<Vector_2>())
    .def(py::init<Line_2>())
    .def(py::init<Ray_2>())
    .def(py::init<Segment_2>())
    .def(py::init<RT&, RT&>())
    .def(py::init<double, double>())
    .def("dx", &Direction_2::dx)
    .def("dy", &Direction_2::dy)
    .def("vector", &Direction_2::vector)
    .def("transform", &Direction_2::transform)
    .def("counterclockwise_in_between", &Direction_2::counterclockwise_in_between)
    .def("delta", &Direction_2::delta)
    .def("__str__", to_string<Direction_2>)
    .def("__repr__", to_string<Direction_2>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self != py::self)
    .def(py::self < py::self)
    .def(py::self > py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def(-py::self)
    //.setattr("__hash__", &hash<Direction_2>)
    ;

  py::class_<Vector_2>(m, "Vector_2")
    .def(py::init<Point_2&, Point_2&>())
    .def(py::init<Line_2>())
    .def(py::init<Ray_2>())
    .def(py::init<Segment_2>())
    .def(py::init<FT&, FT&, FT&>())
    .def(py::init<FT&, FT&>())
    .def(py::init<double, double>())
    .def("hx", &Vector_2::hx)
    .def("hy", &Vector_2::hy)
    .def("hw", &Vector_2::hw)
    .def("x", &Vector_2::x)
    .def("y", &Vector_2::y)
    .def("squared_length", &Vector_2::squared_length)
    .def("homogeneous", &Vector_2::homogeneous)
    .def("cartesian", &Vector_2::cartesian)
    .def("__getitem__", &Vector_2::operator[])
#if CGALPY_KERNEL == CGALPY_KERNEL_EPIC
    // TODO: Returning address of local variable or temporary with EPEC kernel
    .def("cartesian_coordinates", py::range<>(&Vector_2::cartesian_begin, & Vector_2::cartesian_end))
#endif
    .def("dimension", &Vector_2::dimension)
    .def("direction", &Vector_2::direction)
    .def("transform", &Vector_2::transform)
    .def("perpendicular", &Vector_2::perpendicular)
    .def("__str__", to_string<Vector_2>)
    .def("__repr__", to_string<Vector_2>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self != py::self)
    .def(py::self + py::self)
    .def(py::self += py::self)
    .def(py::self - py::self)
    .def(py::self -= py::self)
    .def(-py::self)
    .def(py::self * py::self)
    //.def(self*RT())
    .def(py::self * FT())
    //.def(RT() * py::self)
    .def(FT()*py::self)
    //.def(py::self *= RT())
    .def(py::self*=FT())
    //.def(py::self / RT())
    .def(py::self / FT())
    //.def(py::self /= RT())
    .def(py::self /= FT())
    //.setattr("__hash__", &hash<Vector_2>)
    ;

  py::class_<Bbox_2>(m, "Bbox_2")
    .def(py::init<>())
    .def(py::init<double, double, double, double>())
    .def("dimension", &Bbox_2::dimension)
    .def("dilate", &Bbox_2::dilate)
    .def("xmin", &Bbox_2::xmin)
    .def("ymin", &Bbox_2::ymin)
    .def("xmax", &Bbox_2::xmax)
    .def("ymax", &Bbox_2::ymax)
    .def("min", &Bbox_2::min)
    .def("max", &Bbox_2::max)
    .def("__str__", to_string<Bbox_2>)
    .def("__repr__", to_string<Bbox_2>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self += py::self)
    .def(py::self + py::self)
    ;

  py::class_<Point_3>(m, "Point_3")
    .def(py::init<>())
    .def(py::init<double, double, double>())
    .def(py::init<FT&, FT&, FT&>())
    .def(py::init<RT&, RT&, RT&>())
    .def("x", &Point_3::x)
    .def("y", &Point_3::y)
    .def("z", &Point_3::z)
    .def("hx", &Point_3::hx)
    .def("hy", &Point_3::hy)
    .def("hz", &Point_3::hz)
    .def("hw", &Point_3::hw)
    .def("dimension", &Point_2::dimension)
    .def("__str__", to_string<Point_3>)
    .def("__repr__", to_string<Point_3>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self > py::self)
    .def(py::self < py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def(py::self - py::self)
    //.setattr("__hash__", &hash<Point_3>)
    ;

  py::class_<Weighted_point_3>(m, "Weighted_point_3")
    .def(py::init<>())
    .def(py::init<const CGAL::Origin&>())
    .def(py::init<const Point_3&>())
    .def(py::init<const Point_3&, const FT&>())
    .def(py::init<const FT&, const FT&, const FT&>())
    // Accessors
    .def("point", &Weighted_point_3::point)
    .def("weight", &Weighted_point_3::weight)
    .def("x", &Weighted_point_3::x)
    .def("y", &Weighted_point_3::y)
    .def("z", &Weighted_point_3::z)
    .def("hx", &Weighted_point_3::hx)
    .def("hy", &Weighted_point_3::hy)
    .def("hz", &Weighted_point_3::hz)
    .def("hw", &Weighted_point_3::hw)
    // Operations
    .def("__str__", to_string<Weighted_point_3>)
    .def("__repr__", to_string<Weighted_point_3>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    // Convenient operations
    .def("homogeneous", &Weighted_point_3::homogeneous)
    .def("cartesian", &Weighted_point_3::cartesian)
    // Kernel::FT 	operator[] (int i) const
    // Cartesian_const_iterator 	cartesian_begin () const
    // Cartesian_const_iterator 	cartesian_end () const
    .def("dimension", &Weighted_point_3::dimension)
    .def("bbox", &Weighted_point_3::bbox)
    // .def("transform", &Weighted_point_3::transform)
    //.setattr("__hash__", &hash<Point_3>)
    ;

  py::class_<Aff_transformation_2>(m, "Aff_transformation_2")
    .def(py::init<>())
    .def(py::init<RT&, RT&, RT&, RT&, RT&>())
    .def(py::init<RT, RT, RT, RT>())
    .def(py::init<RT&, RT&, RT&, RT&, RT&, RT&, RT&>())
    .def(py::init<RT, RT, RT, RT, RT, RT, RT>())
    .def(py::init<const Translation, const Vector_2&>())
    .def(py::init<const Rotation, const Direction_2&, const RT&, const RT&>())
    .def(py::init<const Rotation, const Direction_2&, const RT, const RT>())
    .def(py::init<const Rotation, const RT&, const RT&, const RT&>())
    .def(py::init<const Rotation, const RT, const RT, const RT>())
    .def(py::init<Scaling, const RT&, const RT&>())
    .def(py::init<Scaling, const RT, const RT>())
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
    .def("__str__", to_string<Aff_transformation_2>)
    .def("__repr__", to_string<Aff_transformation_2>)
    .def(py::self * py::self)
    ;

  py::class_<Aff_transformation_3>(m, "Aff_transformation_3")
    .def(py::init<>())
    ;

  //Global kernel functions
  m.def<CGAL::Angle(const Vector_2&, const Vector_2&)>("angle", &CGAL::angle);
  m.def<CGAL::Angle(const Point_2&, const Point_2&, const Point_2&)>("angle", &CGAL::angle);
  m.def<CGAL::Angle(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("angle", &CGAL::angle);

  m.def<FT (const Point_2&, const Point_2&, const Point_2&)>("area", &CGAL::area);

  m.def<bool(const Point_2&, const Point_2&, const Point_2&)>("are_ordered_along_line", &CGAL::are_ordered_along_line);

  m.def<bool(const Point_2&, const Point_2&, const Point_2&)>("are_strictly_ordered_along_line", &CGAL::are_strictly_ordered_along_line);

  m.def<Point_2(const Point_2&, const FT&, const Point_2&, const FT&)>("barycenter", &CGAL::barycenter);
  m.def<Point_2(const Point_2&, const FT&, const Point_2&, const FT&, const Point_2&, const FT&)>("barycenter", &CGAL::barycenter);
  m.def<Point_2(const Point_2&, const FT&, const Point_2&, const FT&, const Point_2&, const FT&, const Point_2&, const FT&)>("barycenter", &CGAL::barycenter);

  m.def<Line_2(const Point_2&, const Point_2&)>("bisector", &CGAL::bisector);

  // Requires sqrt operation
  //def<Line_2(const Line_2&, const Line_2&)>("bisector", &CGAL::bisector);

  m.def<Point_2(const Point_2&, const Point_2&, const Point_2&)>("centroid", &CGAL::centroid);
  m.def<Point_2(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("centroid", &CGAL::centroid);
  m.def<Point_2(const Triangle_2&)>("centroid", &CGAL::centroid);

  m.def<Point_2(const Point_2&, const Point_2&)>("circumcenter", &CGAL::circumcenter);
  m.def<Point_2(const Point_2&, const Point_2&, const Point_2&)>("circumcenter", &CGAL::circumcenter);
  m.def<Point_2(const Triangle_2&)>("circumcenter", &CGAL::circumcenter);

  m.def<bool(const Point_2&, const Point_2&, const Point_2&)>("collinear_are_ordered_along_line", &CGAL::collinear_are_ordered_along_line);

  m.def<bool(const Point_2&, const Point_2&, const Point_2&)>("collinear_are_strictly_ordered_along_line", &CGAL::collinear_are_strictly_ordered_along_line);

  m.def<bool(const Point_2&, const Point_2&, const Point_2&)>("collinear", &CGAL::collinear);

  m.def<CGAL::Comparison_result(const Point_2&, const Point_2&, const Point_2&)>("compare_distance_to_point", &CGAL::compare_distance_to_point);

  m.def <CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_lexicographically", &CGAL::compare_lexicographically);

  m.def<CGAL::Comparison_result(const Line_2&, const Point_2&, const Point_2&)>("compare_signed_distance_to_line", &CGAL::compare_signed_distance_to_line);

  m.def<CGAL::Comparison_result(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("compare_signed_distance_to_line", &CGAL::compare_signed_distance_to_line);
  m.def<CGAL::Comparison_result(const Line_2&, const Point_2&, const Point_2&)>("compare_signed_distance_to_line", &CGAL::compare_signed_distance_to_line);

  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&)>("compare_slope", &CGAL::compare_slope);
  m.def<CGAL::Comparison_result(const Segment_2&, const Segment_2&)>("compare_slope", &CGAL::compare_slope);

  m.def<CGAL::Comparison_result(const Point_2&, const Point_2&, const FT&)>("compare_squared_distance", &CGAL::compare_squared_distance);

  m.def<CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_x", &CGAL::compare_x);
  m.def<CGAL::Comparison_result(const Point_2&, const Line_2&, const Line_2&)>("compare_x", &CGAL::compare_x);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_x", &CGAL::compare_x);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_x", &CGAL::compare_x);

  m.def<CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_xy", &CGAL::compare_xy);

  m.def<CGAL::Comparison_result(const Point_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y);
  m.def<CGAL::Comparison_result(const Point_2&, const Line_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y);

  m.def<CGAL::Comparison_result(const Point_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  m.def<CGAL::Comparison_result(const Point_2&, const Line_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  m.def<CGAL::Comparison_result(const Point_2&, const Segment_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  m.def<CGAL::Comparison_result(const Point_2&, const Segment_2&, const Segment_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);

  m.def<CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_y", &CGAL::compare_y);
  m.def<CGAL::Comparison_result(const Point_2&, const Line_2&, const Line_2&)>("compare_y", &CGAL::compare_y);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_y", &CGAL::compare_y);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_y", &CGAL::compare_y);

  m.def<CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_yx", &CGAL::compare_yx);
  m.def<FT(const Vector_2&, const Vector_2&)>("determinant", &CGAL::determinant);

  m.def<bool (const Point_2&, const Point_2&, const Point_2&)>("has_larger_distace_to_point", &CGAL::has_larger_distance_to_point);

  m.def<bool(const Line_2&, const Point_2&, const Point_2&)>("has_larger_signed_distance_to_line", &CGAL::has_larger_signed_distance_to_line);
  m.def<bool(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("has_larger_signed_distance_to_line", &CGAL::has_larger_signed_distance_to_line);

  m.def<bool(const Point_2&, const Point_2&, const Point_2&)>("has_smaller_distace_to_point", &CGAL::has_smaller_distance_to_point);

  m.def<bool(const Line_2&, const Point_2&, const Point_2&)>("has_smaller_signed_distance_to_line", &CGAL::has_smaller_signed_distance_to_line);
  m.def<bool(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("has_smaller_signed_distance_to_line", &CGAL::has_smaller_signed_distance_to_line);

  m.def<FT(const Point_2&, const Point_2&)>("l_infinity_distance", &CGAL::l_infinity_distance);

  m.def<bool(const Point_2&, const Point_2&, const Point_2&)>("left_turn", &CGAL::left_turn);

  m.def<bool(const Point_2&, const Point_2&)>("lexicographically_xy_larger", &CGAL::lexicographically_xy_larger);

  m.def<bool(const Point_2&, const Point_2&)>("lexicographically_xy_larger_or_equal", &CGAL::lexicographically_xy_larger_or_equal);

  m.def<bool(const Point_2&, const Point_2&)>("lexicographically_xy_smaller", &CGAL::lexicographically_xy_smaller);

  m.def<bool(const Point_2&, const Point_2&)>("lexicographically_xy_smaller_or_equal", &CGAL::lexicographically_xy_smaller_or_equal);

  m.def<Point_2(const Iso_rectangle_2&)>("max_vertex", &CGAL::max_vertex);

  m.def<Point_2(const Point_2&, const Point_2&)>("midpoint", &CGAL::midpoint);

  m.def<Point_2(const Iso_rectangle_2&)>("min_vertex", &CGAL::min_vertex);

  m.def<CGAL::Orientation(const Point_2&, const Point_2&, const Point_2&)>("orientation", CGAL::orientation);
  m.def<CGAL::Orientation(const Vector_2&, const Vector_2&)>("orientation", CGAL::orientation);

  m.def<bool(const Line_2&, const Line_2&)>("parallel", CGAL::parallel);
  m.def<bool(const Ray_2&, const Ray_2&)>("parallel", CGAL::parallel);
  m.def<bool(const Segment_2&, const Segment_2&)>("parallel", CGAL::parallel);

  m.def<Line_2 (const Circle_2&, const Circle_2&)>("radical_line", &CGAL::radical_line);

  m.def<void(const RT&, const RT&, RT&, RT&, RT&, const RT&, const RT&)>("rational_rotation_approximation", &CGAL::rational_rotation_approximation);

  m.def<bool(const Point_2&, const Point_2&, const Point_2&)>("right_turn", &CGAL::right_turn);

  m.def<FT(const Vector_2&, const Vector_2&)>("scalar_product", &CGAL::scalar_product);

  m.def<CGAL::Bounded_side(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("side_of_bounded_circle", &CGAL::side_of_bounded_circle);
  m.def<CGAL::Bounded_side(const Point_2&, const Point_2&, const Point_2&)>("side_of_bounded_circle", &CGAL::side_of_bounded_circle);

  m.def<CGAL::Oriented_side(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("side_of_oriented_circle", &CGAL::side_of_oriented_circle);

  bind_squared_distance_types<Point_2, Line_2, Ray_2, Segment_2, Triangle_2>(m);

  m.def<FT(const Point_2&, const Point_2&, const Point_2&)>("squared_radius", &CGAL::squared_radius);
  m.def<FT(const Point_2&, const Point_2&)>("squared_radius", &CGAL::squared_radius);
  m.def<FT(const Point_2&)>("squared_radius", &CGAL::squared_radius);

  m.def<bool(const Point_2&, const Point_2&)>("x_equal", &CGAL::x_equal);

  m.def<bool(const Point_2&, const Point_2&)>("y_equal", &CGAL::y_equal);

  m.def<bool(const Bbox_2&, const Bbox_2&)>("do_overlap", &CGAL::do_overlap);
}
