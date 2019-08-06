#include <Common.h>


Kernel::Equal_2 kernel_equal_2(Kernel& k)
{
  return (Kernel::Equal_2)(k.equal_2_object());
}


void export_Kernel()
{
  using namespace boost::python;
  class_<CGAL::Gmpq>("Gmpq")
    //.def(init<>())
    .def(init<int, int>())
    .def(init<const std::string&>())
    .def("to_double", &CGAL::Gmpq::to_double)
    .def(self_ns::str(self_ns::self))
    .def(self == self)
    ;

  class_<FT>("FT")
    .def(init<FT::Exact_type>())
    .def("to_double", &CGAL::Gmpq::to_double)
    .def(self_ns::str(self_ns::self))
    .def(self == self)
    ;

  enum_<CGAL::Sign>("Sign")
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
    .def(self_ns::str(self_ns::self))
    .def(self == self)
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
    .def(self_ns::str(self_ns::self))
    .def(self == self)
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
    .def(self_ns::str(self_ns::self))
    //.def(self == self)
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
    ;

  def<Point_2(const Point_2&, const Point_2&)>("midpoint", &CGAL::midpoint);
  def<FT(const Point_2&, const Point_2&)>("squared_distance", &CGAL::squared_distance);
  def<FT(const Point_2&, const Segment_2&)>("squared_distance", &CGAL::squared_distance);

}