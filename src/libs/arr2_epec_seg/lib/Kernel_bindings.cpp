#include <Common.h>


Kernel::Equal_2 kernel_equal_2(Kernel& k)
{
  return (Kernel::Equal_2)(k.equal_2_object());
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


void export_Kernel()
{
  using namespace boost::python;
  class_<CGAL::Gmpq>("Gmpq")
    //.def(init<>())
    .def(init<int, int>())
    .def(init<const std::string&>())
    .def(init<double>())
    .def("to_double", &CGAL::Gmpq::to_double)
    .def(self_ns::str(self_ns::self))
    .def(self == self)
    ;

  class_<FT>("FT")
    .def(init<FT::Exact_type>())
    .def(self_ns::str(self_ns::self))
    .def(self == self)
    ;

  class_<RT>("RT")
    .def(init<RT::Exact_type>())
    .def(self_ns::str(self_ns::self))
    .def(self == self)
    ;

  enum_<CGAL::Sign>("Sign")
    .value("NEGATIVE", CGAL::NEGATIVE)
    .value("ZERO", CGAL::ZERO)
    .value("POSITIVE", CGAL::POSITIVE)
    .export_values()
    ;

  enum_<CGAL::Angle>("Angle")
    .value("OBTUSE", CGAL::OBTUSE)
    .value("RIGHT", CGAL::RIGHT)
    .value("ACUTE", CGAL::ACUTE)
    .export_values()
    ;

  enum_<CGAL::Comparison_result>("Comparison_result")
    .value("SMALLER", CGAL::SMALLER)
    .value("EQUAL", CGAL::EQUAL)
    .value("LARGER", CGAL::LARGER)
    .export_values()
    ;

  enum_<CGAL::Oriented_side>("Oriented_side")
    .value("ON_NEGATIVE_SIDE", CGAL::ON_NEGATIVE_SIDE)
    .value("ON_ORIENTED_BOUNDARY", CGAL::ON_ORIENTED_BOUNDARY)
    .value("ON_POSITIVE_SIDE", CGAL::ON_POSITIVE_SIDE)
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

  class_<Kernel>("Kernel")
    .def(init<>())
    .def("equal_2_object", &kernel_equal_2)
    ;

  class_<Traits>("Traits")
    .def(init<>())
    .def("equal_2_object", &Traits::equal_2_object)
    .def("compare_xy_2_object", &Traits::compare_xy_2_object)
    ;

  class_<Traits::Compare_xy_2>("Traits_compare_xy_2", no_init)
    .def<CGAL::Sign(Traits::Compare_xy_2::*)(const Point_2&, const Point_2&) const>("__call__", &Traits::Compare_xy_2::operator())
    ;

  class_<Traits::Equal_2>("Traits_equal_2_object", no_init)
    .def<bool (Traits::Equal_2::*)(const Point_2&, const Point_2&) const>("__call__", &Traits::Equal_2::operator())
    ;

  class_<Kernel::Equal_2>("Kernel_equal_2_object", no_init)
    //.def<bool (Kernel::Equal_2::*)(const Rational_point&, const Rational_point&) const>("__call__", &Kernel::Equal_2::operator())
    ;

  class_<Point_2>("Point_2")
    .def(init<>())
    .def(init<double, double>())
    .def(init<FT, FT>())
    .def("x", &Point_2::x)
    .def("y", &Point_2::y)
    .def("hx", &Point_2::hx)
    .def("hy", &Point_2::hy)
    .def("hw", &Point_2::hw)
    .def("bbox", &Point_2::bbox)
    .def(self_ns::str(self_ns::self))
    .def(self == self)
    .def(self!=self)
    .def(self += Vector_2())
    .def(self -= Vector_2())
    ;

  class_<Segment_2>("Segment_2")
    .def(init<Point_2, Point_2>())
    .def("source", &Segment_2::source)
    .def("target", &Segment_2::target)
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
    .def(self == self)
    .def(self != self)
    ;

  class_<Curve>("Curve")
    .def("source", &Curve::source, return_value_policy<copy_const_reference>())
    .def("target", &Curve::target, return_value_policy<copy_const_reference>())
    .def("line", &Curve::line, return_value_policy<reference_existing_object>())
    .def("is_vertical", &Curve::is_vertical)
    .def("flip", &Curve::flip)
    .def("left", &Curve::left, return_value_policy<reference_existing_object>())
    .def("right", &Curve::right, return_value_policy<reference_existing_object>())
    .def("is_directed_right", &Curve::is_directed_right)
    .def("is_in_x_range", &Curve::is_in_x_range)
    .def("is_in_y_range", &Curve::is_in_y_range)
    .def("bbox", &Curve::bbox)
    .def(self_ns::str(self_ns::self))
    ;

  class_<Line_2>("Line_2")
    .def("a", &Line_2::a)
    .def("b", &Line_2::b)
    .def("c", &Line_2::c)
    .def("is_degenerate", &Line_2::is_degenerate)
    .def("is_horizontal", &Line_2::is_horizontal)
    .def("is_vertical", &Line_2::is_horizontal)
    .def("has_on", &Line_2::has_on)
    .def("has_on_boundary", &Line_2::has_on_boundary)
    .def("has_on_negative_side", &Line_2::has_on_negative_side)
    .def("has_on_positive_side", &Line_2::has_on_positive_side)
    .def(self_ns::str(self_ns::self))
    .def(self == self)
    .def(self != self)
    ;

  class_<Ray_2>("Ray_2")
    .def("is_degenerate", &Ray_2::is_degenerate)
    .def("is_horizontal", &Ray_2::is_horizontal)
    .def("is_vertical", &Ray_2::is_horizontal)
    .def("has_on", &Ray_2::has_on)
    .def("point", &Ray_2::point)
    .def("supporting_line", &Ray_2::supporting_line)
    .def("opposite", &Ray_2::opposite)
    .def("source", &Ray_2::source)
    .def(self_ns::str(self_ns::self))
    .def(self == self)
    .def(self != self)
    ;

  class_<Direction_2>("Direction_2")
    .def(init<Vector_2>())
    .def(init<Line_2>())
    .def(init<Ray_2>())
    .def(init<Segment_2>())
    .def(init<RT, RT>())
    .def("dx", &Direction_2::dx)
    .def("dy", &Direction_2::dy)
    .def("vector", &Direction_2::vector)
    .def("transform", &Direction_2::transform)
    .def("counterclockwise_in_between", &Direction_2::counterclockwise_in_between)
    .def("delta", &Direction_2::delta)
    .def(self_ns::str(self_ns::self))
    .def(self == self)
    .def(self != self)
    .def(self != self)
    .def(self < self)
    .def(self > self)
    .def(self <= self)
    .def(self >= self)
    .def(-self)
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
    .def(self == self)
    .def(self != self)
    .def(self += self)
    .def(self + self)
    ;

  //TODO: aff_transformation, triangle_2, iso_rectangle(?), weighted_point(?)


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








  //def<FT(const Point_2&, const Point_2&)>("squared_distance", &CGAL::squared_distance);
  //def<FT(const Point_2&, const Segment_2&)>("squared_distance", &CGAL::squared_distance);
  


  def<bool(const Bbox_2&, const Bbox_2&)>("do_overlap", &CGAL::do_overlap);
  

}