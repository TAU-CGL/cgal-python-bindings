// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/to_string.hpp"

#include "CGALPY/config.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/Kernel/export_ft.hpp"
#include "CGALPY/Kernel/export_point_2.hpp"
#include "CGALPY/Kernel/export_point_3.hpp"
#include "CGALPY/Kernel/export_segment_2.hpp"
#include "CGALPY/Kernel/export_vector_2.hpp"
#include "CGALPY/Kernel/export_circle_2.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

extern void export_gmpz(py::module_&);
extern void export_gmpq(py::module_&);

//template<typename T>
//size_t hash(T& immutable) {
//  std::ostringstream stream;
//  stream << immutable;
//  std::string s = stream.str();
//  return boost::hash<std::string>()(s); // TODO: two equal objects can have different string representation
//}

//
template <typename T1, typename T2, typename T3, typename T4, typename T5>
void bind_squared_distance_first_type(py::module_& m) {
  m.def<FT(const T1&, const T1&)>("squared_distance", &CGAL::squared_distance);
  m.def<FT(const T1&, const T2&)>("squared_distance", &CGAL::squared_distance);
  m.def<FT(const T1&, const T3&)>("squared_distance", &CGAL::squared_distance);
  m.def<FT(const T1&, const T4&)>("squared_distance", &CGAL::squared_distance);
  m.def<FT(const T1&, const T5&)>("squared_distance", &CGAL::squared_distance);
}

//
template <typename T1, typename T2, typename T3, typename T4, typename T5>
void bind_squared_distance_types(py::module_& m) {
  bind_squared_distance_first_type< T1, T2, T3, T4, T5 >(m);
  bind_squared_distance_first_type< T2, T1, T3, T4, T5 >(m);
  bind_squared_distance_first_type< T3, T2, T1, T4, T5 >(m);
  bind_squared_distance_first_type< T4, T2, T3, T1, T5 >(m);
  bind_squared_distance_first_type< T5, T2, T3, T4, T1 >(m);
}

//
Point_2 transform_point(Aff_transformation_2& t, Point_2& p)
{ return t.transform(p); }

//
Vector_2 transform_vector(Aff_transformation_2& t, Vector_2 & v)
{ return t.transform(v); }

//
Direction_2 transform_direction(Aff_transformation_2& t, Direction_2& d)
{ return t.transform(d); }

//
Line_2 transform_line(Aff_transformation_2& t, Line_2& l)
{ return t.transform(l); }

//
void export_kernel(py::module_& m) {
  if (! add_attr<CGAL::Gmpz>(m, "Gmpz")) export_gmpz(m);
  if (! add_attr<CGAL::Gmpq>(m, "Gmpq")) export_gmpq(m);

#if ((CGALPY_KERNEL == CGALPY_KERNEL_EPEC) ||                              \
     (CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT) ||                    \
     (CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ))

  if (! add_attr<FT>(m, "FT")) {
    using Fte = FT::Exact_type;
    using Fta = FT::Approximate_type;

    py::class_<FT> ft_c(m, "FT");
    export_ft(ft_c);
    ft_c.def(py::init<Fte>())
      .def("to_double", [](const FT& ft)->double { return CGAL::to_double(ft); })
      .def("exact", [](const FT& ft)->const Fte& { return ft.exact();} )
      .def("approx", [](const FT& ft)->const Fta& { return ft.approx();} )
      .def("__init__", [](FT& self, int nom, int den) {
                         new (&self) FT(Fte(nom, den));
                       })
      ;
  }
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

  // Kernel objects
  using Pnt_2 = Kernel::Point_2;
  using Seg_2 = Kernel::Segment_2;
  using Line_2 = Kernel::Line_2;
  using Vector_2 = Kernel::Vector_2;
  using Circle_2 = Kernel::Circle_2;

  using Pnt_3 = Kernel::Point_3;

  if (! add_attr<Pnt_2>(m, "Point_2")) {
    py::class_<Pnt_2> pnt2_c(m, "Point_2");
    export_point_2<Kernel>(pnt2_c);

    if (! is_exact_ft()) {
      using Cci = Kernel::Cartesian_const_iterator_2;
      add_iterator<Cci, Cci>("Cartesian_iterator", pnt2_c);
      pnt2_c.def("cartesians",
                [] (const Pnt_2& p)
                { return make_iterator(p.cartesian_begin(), p.cartesian_end()); },
                py::keep_alive<0, 1>());
    }
  }

  if (! add_attr<Vector_2>(m, "Vecotr_2")) {
    py::class_<Vector_2> vec_c(m, "Vecotr_2");
    export_vector_2<Kernel>(vec_c);

    if (! is_exact_ft()) {
      using Cci = Kernel::Cartesian_const_iterator_2;
      add_iterator<Cci, Cci>("Cartesian_iterator", vec_c);
      vec_c.def("cartesians",
                [] (const Vector_2& v)
                { return make_iterator(v.cartesian_begin(), v.cartesian_end()); },
                py::keep_alive<0, 1>());
    }
  }

  if (! add_attr<Seg_2>(m, "Segment_2")) {
    py::class_<Seg_2> seg_c(m, "Segment_2");
    export_segment_2<Kernel>(seg_c);
  }

  if (! add_attr<Circle_2>(m, "Circle_2")) {
    py::class_<Circle_2> circle_c(m, "Circle_2");
    export_circle_2<Kernel>(circle_c);
  }

  py::class_<Line_2>(m, "Line_2")
    .def(py::init<RT&, RT&, RT&>())
    .def(py::init<Pnt_2&, Pnt_2&>())
    .def(py::init<Pnt_2&, Direction_2&>())
    .def(py::init<Pnt_2&, Vector_2&>())
    .def(py::init<Seg_2&>())
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
    .def(py::init<Pnt_2&, Pnt_2&>())
    .def(py::init<Pnt_2&, Direction_2&>())
    .def(py::init<Pnt_2&, Vector_2&>())
    .def(py::init<Pnt_2&, Line_2&>())
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
    .def(py::init < Pnt_2&, Pnt_2&, Pnt_2&>())
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
    .def(py::init<Pnt_2&, Pnt_2&>())
    .def(py::init<Pnt_2&, Pnt_2&, int>())
    .def(py::init<Pnt_2&, Pnt_2&, Pnt_2&, Pnt_2&>())
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

  py::class_<Direction_2>(m, "Direction_2")
    .def(py::init<Vector_2>())
    .def(py::init<Line_2>())
    .def(py::init<Ray_2>())
    .def(py::init<Seg_2>())
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

  if (! add_attr<Pnt_3>(m, "Point_3")) {
    py::class_<Pnt_3> pnt3_c(m, "Point_3");
    export_point_3<Kernel>(pnt3_c);

    if (! is_exact_ft()) {
      using Cci = Kernel::Cartesian_const_iterator_3;
      add_iterator<Cci, Cci>("Cartesian_iterator", pnt3_c);
      pnt3_c.def("cartesians",
                 [] (const Pnt_3& p)
                 { return make_iterator(p.cartesian_begin(), p.cartesian_end()); },
                 py::keep_alive<0, 1>());
    }
  }

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

  /// \name Kernel operations
  /// @{
  using Equal_2 = Kernel::Equal_2;
  using Ctr_seg_2 = Kernel::Construct_segment_2;
  using Ctr_midpnt_2 = Kernel::Construct_midpoint_2;

  py::class_<Kernel> ker_c(m, "Kernel");
  ker_c.def(py::init<>())
    .def("equal_2_object",
         [](const Kernel& k)->Equal_2
         { return k.equal_2_object(); })
    .def("construct_midpoint_2_object",
         [](const Kernel& k)->Ctr_midpnt_2
         { return k.construct_midpoint_2_object(); })
    .def("construct_segment_2_object",
         [](const Kernel& k)->Ctr_seg_2
         { return k.construct_segment_2_object(); })
    ;

  // Equal_2
  using Equal_2_pnt = bool(Equal_2::*)(const Pnt_2&, const Pnt_2&)const;
  using Equal_2_seg = bool(Equal_2::*)(const Pnt_2&, const Pnt_2&)const;
  using Equal_2_line = bool(Equal_2::*)(const Pnt_2&, const Pnt_2&)const;
  using Equal_2_ray = bool(Equal_2::*)(const Pnt_2&, const Pnt_2&)const;
  using Equal_2_circle = bool(Equal_2::*)(const Pnt_2&, const Pnt_2&)const;
  py::class_<Equal_2>(ker_c, "Equal_2")
    .def("__call__", static_cast<Equal_2_pnt>(&Equal_2::operator()))
    .def("__call__", static_cast<Equal_2_seg>(&Equal_2::operator()))
    .def("__call__", static_cast<Equal_2_line>(&Equal_2::operator()))
    .def("__call__", static_cast<Equal_2_ray>(&Equal_2::operator()))
    .def("__call__", static_cast<Equal_2_circle>(&Equal_2::operator()))
    ;

  // Construct_segment_2
  using Ctr_seg_2_op =
    Seg_2(Ctr_seg_2::*)(const Pnt_2&, const Pnt_2&)const;
  py::class_<Ctr_seg_2>(ker_c, "Construct_segment_2")
    .def("__call__", static_cast<Ctr_seg_2_op>(&Ctr_seg_2::operator()))
    ;

  // Construct_midpoint_2
  using Ctr_midpnt_2_op =
    Pnt_2(Ctr_midpnt_2::*)(const Pnt_2&, const Pnt_2&)const;
  py::class_<Ctr_midpnt_2>(ker_c, "Construct_midpoint_2")
    .def("__call__", static_cast<Ctr_midpnt_2_op>(&Ctr_midpnt_2::operator()))
    ;

  /// @}

  /// \name Global kernel functions
  /// @{
  using Angle_fnc1 = CGAL::Angle(*)(const Vector_2&, const Vector_2&);
  using Angle_fnc2 = CGAL::Angle(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Angle_fnc3 = CGAL::Angle(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("angle", static_cast<Angle_fnc1>(&CGAL::angle<Kernel>));
  m.def("angle", static_cast<Angle_fnc2>(&CGAL::angle<Kernel>));
  m.def("angle", static_cast<Angle_fnc3>(&CGAL::angle<Kernel>));

  using Area_fnc = FT(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("area", static_cast<Area_fnc>(&CGAL::area<Kernel>));

  using Aoal_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("are_ordered_along_line", static_cast<Aoal_fnc>(&CGAL::are_ordered_along_line<Kernel>));

  using Asoal_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("are_strictly_ordered_along_line", static_cast<Asoal_fnc>(&CGAL::are_strictly_ordered_along_line<Kernel>));

  using barycenter_fnc1 = Pnt_2(*)(const Pnt_2&, const FT&, const Pnt_2&, const FT&);
  using barycenter_fnc2 = Pnt_2(*)(const Pnt_2&, const FT&, const Pnt_2&, const FT&, const Pnt_2&, const FT&);
  using barycenter_fnc3 = Pnt_2(*)(const Pnt_2&, const FT&, const Pnt_2&, const FT&, const Pnt_2&, const FT&, const Pnt_2&, const FT&);
  m.def("barycenter", static_cast<barycenter_fnc1>(&CGAL::barycenter<Kernel>));
  m.def("barycenter", static_cast<barycenter_fnc2>(&CGAL::barycenter<Kernel>));
  m.def("barycenter", static_cast<barycenter_fnc3>(&CGAL::barycenter<Kernel>));

  using Bisector_fnc = Line_2(*)(const Pnt_2&, const Pnt_2&);
  m.def("bisector", static_cast<Bisector_fnc>(&CGAL::bisector<Kernel>));

  // Requires sqrt operation
  //def<Line_2(const Line_2&, const Line_2&)>("bisector", &CGAL::bisector);

  using Centroid_fnc1 = Pnt_2(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Centroid_fnc2 = Pnt_2(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Centroid_fnc3 = Pnt_2(*)(const Triangle_2&);
  m.def("centroid", static_cast<Centroid_fnc1>(&CGAL::centroid<Kernel>));
  m.def("centroid", static_cast<Centroid_fnc2>(&CGAL::centroid<Kernel>));
  m.def("centroid", static_cast<Centroid_fnc3>(&CGAL::centroid<Kernel>));

  using Circumcenter_fnc1 = Pnt_2(*)(const Pnt_2&, const Pnt_2&);
  using Circumcenter_fnc2 = Pnt_2(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Circumcenter_fnc3 = Pnt_2(*)(const Triangle_2&);
  m.def("circumcenter", static_cast<Circumcenter_fnc1>(&CGAL::circumcenter<Kernel>));
  m.def("circumcenter", static_cast<Circumcenter_fnc2>(&CGAL::circumcenter<Kernel>));
  m.def("circumcenter", static_cast<Circumcenter_fnc3>(&CGAL::circumcenter<Kernel>));

  m.def<bool(const Pnt_2&, const Pnt_2&, const Pnt_2&)>("collinear_are_ordered_along_line", &CGAL::collinear_are_ordered_along_line<Kernel>);

  m.def<bool(const Pnt_2&, const Pnt_2&, const Pnt_2&)>("collinear_are_strictly_ordered_along_line", &CGAL::collinear_are_strictly_ordered_along_line<Kernel>);

  m.def<bool(const Pnt_2&, const Pnt_2&, const Pnt_2&)>("collinear", &CGAL::collinear<Kernel>);

  m.def<CGAL::Comparison_result(const Pnt_2&, const Pnt_2&, const Pnt_2&)>("compare_distance_to_point", &CGAL::compare_distance_to_point<Kernel>);

  m.def<CGAL::Comparison_result(const Pnt_2&, const Pnt_2&)>("compare_lexicographically", &CGAL::compare_lexicographically<Kernel>);

  m.def<CGAL::Comparison_result(const Line_2&, const Pnt_2&, const Pnt_2&)>("compare_signed_distance_to_line", &CGAL::compare_signed_distance_to_line<Kernel>);

  m.def<CGAL::Comparison_result(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&)>("compare_signed_distance_to_line", &CGAL::compare_signed_distance_to_line<Kernel>);
  m.def<CGAL::Comparison_result(const Line_2&, const Pnt_2&, const Pnt_2&)>("compare_signed_distance_to_line", &CGAL::compare_signed_distance_to_line<Kernel>);

  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&)>("compare_slope", &CGAL::compare_slope<Kernel>);
  m.def<CGAL::Comparison_result(const Segment_2&, const Segment_2&)>("compare_slope", &CGAL::compare_slope<Kernel>);

  m.def<CGAL::Comparison_result(const Pnt_2&, const Pnt_2&, const FT&)>("compare_squared_distance", &CGAL::compare_squared_distance<Kernel>);

  m.def<CGAL::Comparison_result(const Pnt_2&, const Pnt_2&)>("compare_x", &CGAL::compare_x<Kernel>);
  m.def<CGAL::Comparison_result(const Pnt_2&, const Line_2&, const Line_2&)>("compare_x", &CGAL::compare_x<Kernel>);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_x", &CGAL::compare_x<Kernel>);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_x", &CGAL::compare_x<Kernel>);

  m.def<CGAL::Comparison_result(const Pnt_2&, const Pnt_2&)>("compare_xy", &CGAL::compare_xy<Kernel>);

  m.def<CGAL::Comparison_result(const Pnt_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y<Kernel>);
  m.def<CGAL::Comparison_result(const Pnt_2&, const Line_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y<Kernel>);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y<Kernel>);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_x_at_y", &CGAL::compare_x_at_y<Kernel>);

  m.def<CGAL::Comparison_result(const Pnt_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x<Kernel>);
  m.def<CGAL::Comparison_result(const Pnt_2&, const Line_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x<Kernel>);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x<Kernel>);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_y_at_x", &CGAL::compare_y_at_x<Kernel>);
  m.def<CGAL::Comparison_result(const Pnt_2&, const Segment_2&)>("compare_y_at_x", &CGAL::compare_y_at_x<Kernel>);
  m.def<CGAL::Comparison_result(const Pnt_2&, const Segment_2&, const Segment_2&)>("compare_y_at_x", &CGAL::compare_y_at_x<Kernel>);

  m.def<CGAL::Comparison_result(const Pnt_2&, const Pnt_2&)>("compare_y", &CGAL::compare_y<Kernel>);
  m.def<CGAL::Comparison_result(const Pnt_2&, const Line_2&, const Line_2&)>("compare_y", &CGAL::compare_y<Kernel>);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&)>("compare_y", &CGAL::compare_y<Kernel>);
  m.def<CGAL::Comparison_result(const Line_2&, const Line_2&, const Line_2&, const Line_2&)>("compare_y", &CGAL::compare_y<Kernel>);

  m.def<CGAL::Comparison_result(const Pnt_2&, const Pnt_2&)>("compare_yx", &CGAL::compare_yx<Kernel>);
  m.def<FT(const Vector_2&, const Vector_2&)>("determinant", &CGAL::determinant<Kernel>);

  m.def<bool (const Pnt_2&, const Pnt_2&, const Pnt_2&)>("has_larger_distace_to_point", &CGAL::has_larger_distance_to_point<Kernel>);

  m.def<bool(const Line_2&, const Pnt_2&, const Pnt_2&)>("has_larger_signed_distance_to_line", &CGAL::has_larger_signed_distance_to_line<Kernel>);
  m.def<bool(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&)>("has_larger_signed_distance_to_line", &CGAL::has_larger_signed_distance_to_line<Kernel>);

  m.def<bool(const Pnt_2&, const Pnt_2&, const Pnt_2&)>("has_smaller_distace_to_point", &CGAL::has_smaller_distance_to_point<Kernel>);

  m.def<bool(const Line_2&, const Pnt_2&, const Pnt_2&)>("has_smaller_signed_distance_to_line", &CGAL::has_smaller_signed_distance_to_line<Kernel>);
  m.def<bool(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&)>("has_smaller_signed_distance_to_line", &CGAL::has_smaller_signed_distance_to_line<Kernel>);

  m.def<FT(const Pnt_2&, const Pnt_2&)>("l_infinity_distance", &CGAL::l_infinity_distance<Kernel>);

  m.def<bool(const Pnt_2&, const Pnt_2&, const Pnt_2&)>("left_turn", &CGAL::left_turn<Kernel>);

  m.def<bool(const Pnt_2&, const Pnt_2&)>("lexicographically_xy_larger", &CGAL::lexicographically_xy_larger<Kernel>);

  m.def<bool(const Pnt_2&, const Pnt_2&)>("lexicographically_xy_larger_or_equal", &CGAL::lexicographically_xy_larger_or_equal<Kernel>);

  m.def<bool(const Pnt_2&, const Pnt_2&)>("lexicographically_xy_smaller", &CGAL::lexicographically_xy_smaller<Kernel>);

  m.def<bool(const Pnt_2&, const Pnt_2&)>("lexicographically_xy_smaller_or_equal", &CGAL::lexicographically_xy_smaller_or_equal<Kernel>);

  m.def<Pnt_2(const Iso_rectangle_2&)>("max_vertex", &CGAL::max_vertex<Kernel>);

  m.def<Pnt_2(const Pnt_2&, const Pnt_2&)>("midpoint", &CGAL::midpoint<Kernel>);

  m.def<Pnt_2(const Iso_rectangle_2&)>("min_vertex", &CGAL::min_vertex<Kernel>);

  m.def<CGAL::Orientation(const Pnt_2&, const Pnt_2&, const Pnt_2&)>("orientation", CGAL::orientation);
  m.def<CGAL::Orientation(const Vector_2&, const Vector_2&)>("orientation", CGAL::orientation);

  m.def<bool(const Line_2&, const Line_2&)>("parallel", CGAL::parallel);
  m.def<bool(const Ray_2&, const Ray_2&)>("parallel", CGAL::parallel);
  m.def<bool(const Segment_2&, const Segment_2&)>("parallel", CGAL::parallel);

  m.def<Line_2 (const Circle_2&, const Circle_2&)>("radical_line", &CGAL::radical_line<Kernel>);

  // Parameterized by FT (not Kernel)
  m.def<void(const RT&, const RT&, RT&, RT&, RT&, const RT&, const RT&)>("rational_rotation_approximation", &CGAL::rational_rotation_approximation<FT>);

  m.def<bool(const Pnt_2&, const Pnt_2&, const Pnt_2&)>("right_turn", &CGAL::right_turn<Kernel>);

  m.def<FT(const Vector_2&, const Vector_2&)>("scalar_product", &CGAL::scalar_product<Kernel>);

  m.def<CGAL::Bounded_side(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&)>("side_of_bounded_circle", &CGAL::side_of_bounded_circle<Kernel>);
  m.def<CGAL::Bounded_side(const Pnt_2&, const Pnt_2&, const Pnt_2&)>("side_of_bounded_circle", &CGAL::side_of_bounded_circle<Kernel>);

  m.def<CGAL::Oriented_side(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&)>("side_of_oriented_circle", &CGAL::side_of_oriented_circle<Kernel>);

  bind_squared_distance_types<Pnt_2, Line_2, Ray_2, Segment_2, Triangle_2>(m);

  m.def<FT(const Pnt_2&, const Pnt_2&, const Pnt_2&)>("squared_radius", &CGAL::squared_radius<Kernel>);
  m.def<FT(const Pnt_2&, const Pnt_2&)>("squared_radius", &CGAL::squared_radius<Kernel>);
  m.def<FT(const Pnt_2&)>("squared_radius", &CGAL::squared_radius<Kernel>);

  m.def<bool(const Pnt_2&, const Pnt_2&)>("x_equal", &CGAL::x_equal<Kernel>);

  m.def<bool(const Pnt_2&, const Pnt_2&)>("y_equal", &CGAL::y_equal<Kernel>);

  m.def<bool(const Bbox_2&, const Bbox_2&)>("do_overlap", &CGAL::do_overlap);

  /// @}
}
