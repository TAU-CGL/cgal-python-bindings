// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/common.hpp"

Kernel::Equal_2 kernel_equal_2(Kernel& k)
{
  return (Kernel::Equal_2)(k.equal_2_object());
}

template<typename T>
size_t hash(T& immutable)
{
  std::ostringstream stream;
  stream << immutable;
  std::string s = stream.str();
  return boost::hash<std::string>()(s);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void bind_squared_distance_first_type()
{
  using namespace boost::python;
  def<FT(const T1&, const T1&)>("squared_distance", &CGAL::squared_distance);
  def<FT(const T1&, const T2&)>("squared_distance", &CGAL::squared_distance);
  def<FT(const T1&, const T3&)>("squared_distance", &CGAL::squared_distance);
  def<FT(const T1&, const T4&)>("squared_distance", &CGAL::squared_distance);
  def<FT(const T1&, const T5&)>("squared_distance", &CGAL::squared_distance);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void bind_squared_distance_types()
{
  bind_squared_distance_first_type< T1, T2, T3, T4, T5 >();
  bind_squared_distance_first_type< T2, T1, T3, T4, T5 >();
  bind_squared_distance_first_type< T3, T2, T1, T4, T5 >();
  bind_squared_distance_first_type< T4, T2, T3, T1, T5 >();
  bind_squared_distance_first_type< T5, T2, T3, T4, T1 >();
}


#if CGALPY_KERNEL == CGALPY_EPEC_KERNEL
typename FT::Exact_type& FT_exact(FT& ft)
{
  return ft.exact();
}

typename FT::Approximate_type& FT_approx(FT& ft)
{
  return ft.approx();
}
#endif

double FT_to_double(FT& ft)
{
  return CGAL::to_double(ft);
}

Point_2 transform_point(Aff_transformation_2& t, Point_2& p) { return t.transform(p); }
Vector_2 transform_vector(Aff_transformation_2& t, Vector_2 & v) { return t.transform(v); }
Direction_2 transform_direction(Aff_transformation_2& t, Direction_2& d) { return t.transform(d); }
Line_2 transform_line(Aff_transformation_2& t, Line_2& l) { return t.transform(l); }

void export_kernel()
{
  using namespace boost::python;

  class_<Gmpz>("Gmpz")
    //.def(init<>())
    .def(init<int>())
    //.def(init<Gmpz&>())
    .def("to_double", &Gmpz::to_double)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .def(self < self)
    .def(self > self)
    .def(self <= self)
    .def(self >= self)
    .def(self + self)
    .def(self += self)
    .def(self - self)
    .def(self -= self)
    .def(self * self)
    .def(self *= self)
    .def(self / self)
    .def(self /= self)
    ;

  class_<Gmpq>("Gmpq")
    //.def(init<>())
    .def(init<Gmpz, Gmpz>())
    .def(init<unsigned long, unsigned long>())
    .def(init<const std::string&>())
    //.def(init<Gmpq&>())
    .def(init<double>())
    .def("to_double", &Gmpq::to_double)
    .def("numerator", &Gmpq::numerator)
    .def("denominator", &Gmpq::denominator)
    .def("size", &Gmpq::size)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .def(self < self)
    .def(self > self)
    .def(self <= self)
    .def(self >= self)
    .def(self + self)
    .def(self += self)
    .def(self - self)
    .def(self -= self)
    .def(self * self)
    .def(self *= self)
    .def(self / self)
    .def(self /= self)
    .def(-self)
    ;
#if CGALPY_KERNEL == CGALPY_EPEC_KERNEL
  class_<FT>("FT")
    .def(init<double>())
    .def(init<FT::Exact_type>())
    .def(init<FT>())
    .def("exact", &FT_exact, return_internal_reference<>())
    //.def("approx", &FT_approx, return_internal_reference<>())
    .def("to_double", &FT_to_double)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .def(self < self)
    .def(self > self)
    .def(self <= self)
    .def(self >= self)
    .def(self + self)
    .def(self += self)
    .def(self - self)
    .def(self -= self)
    .def(self * self)
    .def(self *= self)
    .def(self / self)
    .def(self /= self)
    .def(-self)
    ;
#endif

  //class_<RT>("RT")
  //  .def(init<RT::Exact_type>())
  //  .def(self_ns::str(self_ns::self))
  //  .def(self == self)
  //  ;

  enum_<CGAL::Sign>("Result")

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

  enum_<CGAL::Angle>("Angle")
    .value("OBTUSE", CGAL::OBTUSE)
    .value("RIGHT", CGAL::RIGHT)
    .value("ACUTE", CGAL::ACUTE)
    .export_values()
    ;

  enum_<CGAL::Arr_halfedge_direction>("Arr_halfedge_direction")
    .value("ARR_RIGHT_TO_LEFT", CGAL::Arr_halfedge_direction::ARR_RIGHT_TO_LEFT)
    .value("ARR_LEFT_TO_RIGHT", CGAL::Arr_halfedge_direction::ARR_LEFT_TO_RIGHT)
    .export_values()
    ;
  enum_<CGAL::Arr_curve_end>("Arr_curve_end")
    .value("ARR_MIN_END", CGAL::Arr_curve_end::ARR_MIN_END)
    .value("ARR_MAX_END", CGAL::Arr_curve_end::ARR_MAX_END)
    .export_values()
    ;

  class_<Rotation>("Rotation")
    .def(init<>())
    ;

  class_<Scaling>("Scaling")
    .def(init<>())
    ;

  class_<Translation>("Translation")
    .def(init<>())
    ;

  //class_<Kernel>("Kernel")
  //  .def(init<>())
  //  .def("equal_2_object", &kernel_equal_2)
  //  ;

  //class_<Traits>("Traits")
  //  .def(init<>())
  //  .def("equal_2_object", &Traits::equal_2_object)
  //  .def("compare_xy_2_object", &Traits::compare_xy_2_object)
  //  ;

  //class_<Traits::Compare_xy_2>("Traits_compare_xy_2", no_init)
  //  .def<CGAL::Sign(Traits::Compare_xy_2::*)(const Point_2&, const Point_2&) const>("__call__", &Traits::Compare_xy_2::operator())
  //  ;

  //class_<Traits::Equal_2>("Traits_equal_2_object", no_init)
  //  .def<bool (Traits::Equal_2::*)(const Point_2&, const Point_2&) const>("__call__", &Traits::Equal_2::operator())
  //  ;

  //class_<Kernel::Equal_2>("Kernel_equal_2_object", no_init)
  //  //.def<bool (Kernel::Equal_2::*)(const Rational_point&, const Rational_point&) const>("__call__", &Kernel::Equal_2::operator())
  //  ;

  class_<Point_2>("Point_2")
    .def(init<>())
    .def(init<double, double>())
    .def(init<FT&, FT&>())
    .def(init<RT&, RT&>())
    .def(init<Point_2&>())
    .def("x", &Point_2::x, Kernel_return_value_policy())
    .def("y", &Point_2::y, Kernel_return_value_policy())
    .def("hx", &Point_2::hx, Kernel_return_value_policy())
    .def("hy", &Point_2::hy, Kernel_return_value_policy())
    .def("hw", &Point_2::hw, Kernel_return_value_policy())
    .def("bbox", &Point_2::bbox)
    .def("dimension", &Point_2::dimension)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self!=self)
    .def(self > self)
    .def(self < self)
    .def(self <= self)
    .def(self >= self)
    .def(self - self)
    .def(self += Vector_2())
    .def(self -= Vector_2())
    .def(self + Vector_2())
    .def(self - Vector_2())
    .setattr("__hash__", &hash<Point_2>)
    ;

  class_<Segment_2>("Segment_2")
    .def(init<Point_2&, Point_2&>())
    .def("source", &Segment_2::source, Kernel_return_value_policy())
    .def("target", &Segment_2::target, Kernel_return_value_policy())
    .def("supporting_line", &Segment_2::supporting_line)
    .def("squared_length", &Segment_2::squared_length)
    .def("direction", &Segment_2::direction)
    .def("has_on", &Segment_2::has_on)
    .def("collinear_has_on", &Segment_2::collinear_has_on)
    .def("is_degenerate", &Segment_2::is_degenerate)
    .def("is_horizontal", &Segment_2::is_horizontal)
    .def("is_vertical", &Segment_2::is_vertical)
    .def("bbox", &Segment_2::bbox)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .setattr("__hash__", &hash<Segment_2>)
    ;

  class_<Line_2>("Line_2")
    .def(init<RT&, RT&, RT&>())
    .def(init<Point_2&, Point_2&>())
    .def(init<Point_2&, Direction_2&>())
    .def(init<Point_2&, Vector_2&>())
    .def(init<Segment_2&>())
    .def(init<Ray_2&>())
    .def("a", &Line_2::a)
    .def("b", &Line_2::b)
    .def("c", &Line_2::c)
    .def("is_degenerate", &Line_2::is_degenerate)
    .def("is_horizontal", &Line_2::is_horizontal)
    .def("is_vertical", &Line_2::is_vertical)
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
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .setattr("__hash__", &hash<Line_2>)
    ;

  class_<Ray_2>("Ray_2")
    .def(init<Point_2&, Point_2&>())
    .def(init<Point_2&, Direction_2&>())
    .def(init<Point_2&, Vector_2&>())
    .def(init<Point_2&, Line_2&>())
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
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .setattr("__hash__", &hash<Ray_2>)
    ;

  class_<Triangle_2>("Triangle_2")
    .def(init < Point_2&, Point_2&, Point_2&>())
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
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .setattr("__hash__", &hash<Triangle_2>)
    ;

  class_<Iso_rectangle_2>("Iso_rectangle_2")
    .def(init<Point_2&, Point_2&>())
    .def(init<Point_2&, Point_2&, int>())
    .def(init<Point_2&, Point_2&, Point_2&, Point_2&>())
    .def(init<RT&, RT&, RT&, RT&, RT&>())
    .def(init<RT, RT, RT, RT>())
    .def(init<Bbox_2&>())
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
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .setattr("__hash__", &hash<Iso_rectangle_2>)
    ;

  class_<Circle_2>("Circle_2")
    .def(init<>())
    .def(init<Point_2&, FT&, CGAL::Orientation>())
    .def(init<Point_2&, FT, CGAL::Orientation>())
    .def(init<Point_2&, Point_2&, Point_2&>())
    .def(init<Point_2&, Point_2&, CGAL::Orientation>())
    .def(init<Point_2&, CGAL::Orientation>())
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
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .setattr("__hash__", &hash<Circle_2>)
    ;

  class_<Direction_2>("Direction_2")
    .def(init<Vector_2>())
    .def(init<Line_2>())
    .def(init<Ray_2>())
    .def(init<Segment_2>())
    .def(init<RT&, RT&>())
    .def(init<double, double>())
    .def("dx", &Direction_2::dx, Kernel_return_value_policy())
    .def("dy", &Direction_2::dy, Kernel_return_value_policy())
    .def("vector", &Direction_2::vector)
    .def("transform", &Direction_2::transform)
    .def("counterclockwise_in_between", &Direction_2::counterclockwise_in_between)
    .def("delta", &Direction_2::delta, Kernel_return_value_policy())
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .def(self != self)
    .def(self < self)
    .def(self > self)
    .def(self <= self)
    .def(self >= self)
    .def(-self)
    .setattr("__hash__", &hash<Direction_2>)
    ;

  class_<Vector_2>("Vector_2")
    .def(init<Point_2&, Point_2&>())
    .def(init<Line_2>())
    .def(init<Ray_2>())
    .def(init<Segment_2>())
    .def(init<FT&, FT&, FT&>())
    .def(init<FT&, FT&>())
    .def(init<double, double>())
    .def("hx", &Vector_2::hx, Kernel_return_value_policy())
    .def("hy", &Vector_2::hy, Kernel_return_value_policy())
    .def("hw", &Vector_2::hw, Kernel_return_value_policy())
    .def("x", &Vector_2::x, Kernel_return_value_policy())
    .def("y", &Vector_2::y, Kernel_return_value_policy())
    .def("squared_length", &Vector_2::squared_length)
    .def("homogeneous", &Vector_2::homogeneous, Kernel_return_value_policy())
    .def("cartesian", &Vector_2::cartesian, Kernel_return_value_policy())
    .def("__getitem__", &Vector_2::operator[], Kernel_return_value_policy())
    //.def("cartesian_coordinates", range(&Vector_2::cartesian_begin, &Vector_2::cartesian_end))
    .def("dimension", &Vector_2::dimension)
    .def("direction", &Vector_2::direction)
    .def("transform", &Vector_2::transform)
    .def("perpendicular", &Vector_2::perpendicular)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .def(self != self)
    .def(self + self)
    .def(self += self)
    .def(self - self)
    .def(self -= self)
    .def(-self)
    .def(self * self)
    //.def(self*RT())
    .def(self*FT())
    //.def(RT()*self)
    .def(FT()*self)
    //.def(self*=RT())
    .def(self*=FT())
    //.def(self/RT())
    .def(self/FT())
    //.def(self/=RT())
    .def(self /= FT())
    .setattr("__hash__", &hash<Vector_2>)
    ;

  class_<Bbox_2>("Bbox_2")
    .def(init<>())
    .def(init<double, double, double, double>())
    .def("dimension", &Bbox_2::dimension)
    .def("dilate", &Bbox_2::dilate)
    .def("xmin", &Bbox_2::xmin)
    .def("ymin", &Bbox_2::ymin)
    .def("xmax", &Bbox_2::xmax)
    .def("ymax", &Bbox_2::ymax)
    .def("min", &Bbox_2::min)
    .def("max", &Bbox_2::max)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .def(self += self)
    .def(self + self)
    ;

  class_<Point_3>("Point_3")
    .def(init<>())
    .def(init<double, double, double>())
    .def(init<FT&, FT&, FT&>())
    .def(init<RT&, RT&, RT&>())
    .def("x", &Point_3::x, Kernel_return_value_policy())
    .def("y", &Point_3::y, Kernel_return_value_policy())
    .def("z", &Point_3::z, Kernel_return_value_policy())
    .def("hx", &Point_3::hx, Kernel_return_value_policy())
    .def("hy", &Point_3::hy, Kernel_return_value_policy())
    .def("hz", &Point_3::hz, Kernel_return_value_policy())
    .def("hw", &Point_3::hw, Kernel_return_value_policy())
    .def("dimension", &Point_2::dimension)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .def(self > self)
    .def(self < self)
    .def(self <= self)
    .def(self >= self)
    .def(self - self)
    .setattr("__hash__", &hash<Point_3>)
    ;

  class_<Weighted_point_3>("Weighted_point_3")
    .def(init<>())
    .def(init<const CGAL::Origin&>())
    .def(init<const Point_3&>())
    .def(init<const Point_3&, const FT&>())
    .def(init<const FT&, const FT&, const FT&>())
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
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    // Convenient operations
    .def("homogeneous", &Weighted_point_3::homogeneous)
    .def("cartesian", &Weighted_point_3::cartesian, Kernel_return_value_policy())
    // Kernel::FT 	operator[] (int i) const
    // Cartesian_const_iterator 	cartesian_begin () const
    // Cartesian_const_iterator 	cartesian_end () const
    .def("dimension", &Weighted_point_3::dimension)
    .def("bbox", &Weighted_point_3::bbox)
    // .def("transform", &Weighted_point_3::transform)
    .setattr("__hash__", &hash<Point_3>)
    ;

  class_<Aff_transformation_2>("Aff_transformation_2")
    .def(init<>())
    .def(init<RT&, RT&, RT&, RT&, RT&>())
    .def(init<RT, RT, RT, RT>())
    .def(init<RT&, RT&, RT&, RT&, RT&, RT&, RT&>())
    .def(init<RT, RT, RT, RT, RT, RT, RT>())
    .def(init<const Translation, const Vector_2&>())
    .def(init<const Rotation, const Direction_2&, const RT&, const RT&>())
    .def(init<const Rotation, const Direction_2&, const RT, const RT>())
    .def(init<const Rotation, const RT&, const RT&, const RT&>())
    .def(init<const Rotation, const RT, const RT, const RT>())
    .def(init<Scaling, const RT&, const RT&>())
    .def(init<Scaling, const RT, const RT>())
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
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    //.def(self == self)
    .def(self * self)
    ;

  class_<Aff_transformation_3>("Aff_transformation_3")
    .def(init<>())
    ;

  //Global kernel functions
  def<CGAL::Angle(const Vector_2&, const Vector_2&)>("angle", &CGAL::angle);
  def<CGAL::Angle(const Point_2&, const Point_2&, const Point_2&)>("angle", &CGAL::angle);
  def<CGAL::Angle(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("angle", &CGAL::angle);

  def<FT (const Point_2&, const Point_2&, const Point_2&)>("area", &CGAL::area);

  def<bool(const Point_2&, const Point_2&, const Point_2&)>("are_ordered_along_line", &CGAL::are_ordered_along_line);

  def<bool(const Point_2&, const Point_2&, const Point_2&)>("are_strictly_ordered_along_line", &CGAL::are_strictly_ordered_along_line);

  def<Point_2(const Point_2&, const FT&, const Point_2&, const FT&)>("barycenter", &CGAL::barycenter);
  def<Point_2(const Point_2&, const FT&, const Point_2&, const FT&, const Point_2&, const FT&)>("barycenter", &CGAL::barycenter);
  def<Point_2(const Point_2&, const FT&, const Point_2&, const FT&, const Point_2&, const FT&, const Point_2&, const FT&)>("barycenter", &CGAL::barycenter);

  def<Line_2(const Point_2&, const Point_2&)>("bisector", &CGAL::bisector);
  //def<Line_2(const Line_2&, const Line_2&)>("bisector", &CGAL::bisector);

  def<Point_2(const Point_2&, const Point_2&, const Point_2&)>("centroid", &CGAL::centroid);
  def<Point_2(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("centroid", &CGAL::centroid);
  def<Point_2(const Triangle_2&)>("centroid", &CGAL::centroid);

  def<Point_2(const Point_2&, const Point_2&)>("circumcenter", &CGAL::circumcenter);
  def<Point_2(const Point_2&, const Point_2&, const Point_2&)>("circumcenter", &CGAL::circumcenter);
  def<Point_2(const Triangle_2&)>("circumcenter", &CGAL::circumcenter);

  def<bool(const Point_2&, const Point_2&, const Point_2&)>("collinear_are_ordered_along_line", &CGAL::collinear_are_ordered_along_line);

  def<bool(const Point_2&, const Point_2&, const Point_2&)>("collinear_are_strictly_ordered_along_line", &CGAL::collinear_are_strictly_ordered_along_line);

  def<bool(const Point_2&, const Point_2&, const Point_2&)>("collinear", &CGAL::collinear);

  def<CGAL::Comparison_result(const Point_2&, const Point_2&, const Point_2&)>("compare_distance_to_point", &CGAL::compare_distance_to_point);

  def <CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_lexicographically", &CGAL::compare_lexicographically);

  def<CGAL::Comparison_result(const Line_2&, const Point_2&, const Point_2&)>("compare_signed_distance_to_line", &CGAL::compare_signed_distance_to_line);

  def<CGAL::Comparison_result(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("compare_signed_distance_to_line", &CGAL::compare_signed_distance_to_line);
  def<CGAL::Comparison_result(const Line_2&, const Point_2&, const Point_2&)>("compare_signed_distance_to_line", &CGAL::compare_signed_distance_to_line);

  def<CGAL::Comparison_result(const Line_2&, const Line_2&)>("compare_slope", &CGAL::compare_slope);
  def<CGAL::Comparison_result(const Segment_2&, const Segment_2&)>("compare_slope", &CGAL::compare_slope);

  def<CGAL::Comparison_result(const Point_2&, const Point_2&, const FT&)>("compare_squared_distance", &CGAL::compare_squared_distance);

  def<CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_x", &CGAL::compare_x);
  def<CGAL::Comparison_result(const Point_2&, const Line_2&, const Line_2&)>("compare_x", &CGAL::compare_x);
  def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_x", &CGAL::compare_x);
  def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_x", &CGAL::compare_x);

  def<CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_xy", &CGAL::compare_xy);

  def<CGAL::Comparison_result(const Point_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y);
  def<CGAL::Comparison_result(const Point_2&, const Line_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y);
  def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y);
  def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y);

  def<CGAL::Comparison_result(const Point_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  def<CGAL::Comparison_result(const Point_2&, const Line_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  def<CGAL::Comparison_result(const Point_2&, const Segment_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);
  def<CGAL::Comparison_result(const Point_2&, const Segment_2&, const Segment_2&)>("compare_y_at_x", &CGAL::compare_y_at_x);

  def<CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_y", &CGAL::compare_y);
  def<CGAL::Comparison_result(const Point_2&, const Line_2&, const Line_2&)>("compare_y", &CGAL::compare_y);
  def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_y", &CGAL::compare_y);
  def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_y", &CGAL::compare_y);

  def<CGAL::Comparison_result(const Point_2&, const Point_2&)>("compare_yx", &CGAL::compare_yx);
  def<FT(const Vector_2&, const Vector_2&)>("determinant", &CGAL::determinant);

  def<bool (const Point_2&, const Point_2&, const Point_2&)>("has_larger_distace_to_point", &CGAL::has_larger_distance_to_point);

  def<bool(const Line_2&, const Point_2&, const Point_2&)>("has_larger_signed_distance_to_line", &CGAL::has_larger_signed_distance_to_line);
  def<bool(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("has_larger_signed_distance_to_line", &CGAL::has_larger_signed_distance_to_line);

  def<bool(const Point_2&, const Point_2&, const Point_2&)>("has_smaller_distace_to_point", &CGAL::has_smaller_distance_to_point);

  def<bool(const Line_2&, const Point_2&, const Point_2&)>("has_smaller_signed_distance_to_line", &CGAL::has_smaller_signed_distance_to_line);
  def<bool(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("has_smaller_signed_distance_to_line", &CGAL::has_smaller_signed_distance_to_line);

  //l_infinity_distance() ?

  def<bool(const Point_2&, const Point_2&, const Point_2&)>("left_turn", &CGAL::left_turn);

  def<bool(const Point_2&, const Point_2&)>("lexicographically_xy_larger", &CGAL::lexicographically_xy_larger);

  def<bool(const Point_2&, const Point_2&)>("lexicographically_xy_larger_or_equal", &CGAL::lexicographically_xy_larger_or_equal);

  def<bool(const Point_2&, const Point_2&)>("lexicographically_xy_smaller", &CGAL::lexicographically_xy_smaller);

  def<bool(const Point_2&, const Point_2&)>("lexicographically_xy_smaller_or_equal", &CGAL::lexicographically_xy_smaller_or_equal);

  def<Point_2(const Iso_rectangle_2&)>("max_vertex", &CGAL::max_vertex);

  def<Point_2(const Point_2&, const Point_2&)>("midpoint", &CGAL::midpoint);

  def<Point_2(const Iso_rectangle_2&)>("min_vertex", &CGAL::min_vertex);

  def<CGAL::Orientation(const Point_2&, const Point_2&, const Point_2&)>("orientation", CGAL::orientation);
  def<CGAL::Orientation(const Vector_2&, const Vector_2&)>("orientation", CGAL::orientation);

  def<bool(const Line_2&, const Line_2&)>("parallel", CGAL::parallel);
  def<bool(const Ray_2&, const Ray_2&)>("parallel", CGAL::parallel);
  def<bool(const Segment_2&, const Segment_2&)>("parallel", CGAL::parallel);

  def<Line_2 (const Circle_2&, const Circle_2&)>("radical_line", &CGAL::radical_line);

  //rational_rotation_approximation() (?)

  def<bool(const Point_2&, const Point_2&, const Point_2&)>("right_turn", &CGAL::right_turn);


  def<FT(const Vector_2&, const Vector_2&)>("scalar_product", &CGAL::scalar_product);

  def<CGAL::Bounded_side(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("side_of_bounded_circle", &CGAL::side_of_bounded_circle);
  def<CGAL::Bounded_side(const Point_2&, const Point_2&, const Point_2&)>("side_of_bounded_circle", &CGAL::side_of_bounded_circle);

  def<CGAL::Oriented_side(const Point_2&, const Point_2&, const Point_2&, const Point_2&)>("side_of_oriented_circle", &CGAL::side_of_oriented_circle);

  bind_squared_distance_types<Point_2, Line_2, Ray_2, Segment_2, Triangle_2>();

  def<FT(const Point_2&, const Point_2&, const Point_2&)>("squared_radius", &CGAL::squared_radius);
  def<FT(const Point_2&, const Point_2&)>("squared_radius", &CGAL::squared_radius);
  def<FT(const Point_2&)>("squared_radius", &CGAL::squared_radius);

  def<bool(const Point_2&, const Point_2&)>("x_equal", &CGAL::x_equal);

  def<bool(const Point_2&, const Point_2&)>("y_equal", &CGAL::y_equal);

  def<bool(const Bbox_2&, const Bbox_2&)>("do_overlap", &CGAL::do_overlap);


}
