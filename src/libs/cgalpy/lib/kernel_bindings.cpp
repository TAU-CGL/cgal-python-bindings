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
#include <nanobind/stl/string.h>

#include "CGALPY/to_string.hpp"

#include "CGALPY/config.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/Kernel/export_ft.hpp"

// 2D functors
#include "CGALPY/Kernel/export_circle_2.hpp"
#include "CGALPY/Kernel/export_dir_2.hpp"
#include "CGALPY/Kernel/export_line_2.hpp"
#include "CGALPY/Kernel/export_point_2.hpp"
#include "CGALPY/Kernel/export_ray_2.hpp"
#include "CGALPY/Kernel/export_segment_2.hpp"
#include "CGALPY/Kernel/export_vector_2.hpp"

// 3D functors
#include "CGALPY/Kernel/export_point_3.hpp"
#include "CGALPY/Kernel/export_vector_3.hpp"
#include "CGALPY/Kernel/export_plane_3.hpp"

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
  using Sd_fnc1 = FT(*)(const T1&, const T1&);
  using Sd_fnc2 = FT(*)(const T1&, const T2&);
  using Sd_fnc3 = FT(*)(const T1&, const T3&);
  using Sd_fnc4 = FT(*)(const T1&, const T4&);
  using Sd_fnc5 = FT(*)(const T1&, const T5&);
  m.def("squared_distance", static_cast<Sd_fnc1>(&CGAL::squared_distance<Kernel>));
  m.def("squared_distance", static_cast<Sd_fnc2>(&CGAL::squared_distance<Kernel>));
  m.def("squared_distance", static_cast<Sd_fnc3>(&CGAL::squared_distance<Kernel>));
  m.def("squared_distance", static_cast<Sd_fnc4>(&CGAL::squared_distance<Kernel>));
  m.def("squared_distance", static_cast<Sd_fnc5>(&CGAL::squared_distance<Kernel>));
}

//
template <typename T1, typename T2, typename T3, typename T4, typename T5>
void bind_squared_distance_types(py::module_& m) {
  bind_squared_distance_first_type<T1, T2, T3, T4, T5>(m);
  bind_squared_distance_first_type<T2, T1, T3, T4, T5>(m);
  bind_squared_distance_first_type<T3, T2, T1, T4, T5>(m);
  bind_squared_distance_first_type<T4, T2, T3, T1, T5>(m);
  bind_squared_distance_first_type<T5, T2, T3, T4, T1>(m);
}

//
Point_2 transform_point(Aff_transformation_2& t, Point_2& p)
{ return t.transform(p); }

//
Vector_2 transform_vector(Aff_transformation_2& t, Vector_2& v)
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
      .def("__init__", [](FT* self, const std::string& str)
                       { new (self) FT(Fte(str)); })
      .def("__init__", [](FT* self, int nom, int den)
                       { new (self) FT(Fte(nom, den)); })
      .def("to_double", [](const FT& ft)->double { return CGAL::to_double(ft); })
      .def("exact", [](const FT& ft)->const Fte& { return ft.exact();} )
      .def("approx", [](const FT& ft)->const Fta& { return ft.approx();} )
      ;
  }
  #endif

  //class_<RT>(m, "RT")
  //  .def(init<RT::Exact_type>())
  //  .def(self_ns::str(self_ns::self))
  //  .def(self == self)
  //  ;

  if (! add_attr<CGAL::Sign>(m, "Result")) {
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
  }

  if (! add_attr<CGAL::Origin>(m, "Origin")) {
    py::class_<CGAL::Origin>(m, "Origin")
      .def(py::init<>())
      ;
  }

  if (! add_attr<CGAL::Angle>(m, "Angle")) {
    py::enum_<CGAL::Angle>(m, "Angle")
      .value("OBTUSE", CGAL::OBTUSE)
      .value("RIGHT", CGAL::RIGHT)
      .value("ACUTE", CGAL::ACUTE)
      .export_values()
      ;
  }

  if (! add_attr<Rotation>(m, "Rotation")) {
    py::class_<Rotation>(m, "Rotation")
      .def(py::init<>())
      ;
  }

  if (! add_attr<Scaling>(m, "Scaling")) {
    py::class_<Scaling>(m, "Scaling")
      .def(py::init<>())
      ;
  }

  if (! add_attr<Translation>(m, "Translation")) {
    py::class_<Translation>(m, "Translation")
      .def(py::init<>())
      ;
  }

  // Kernel objects
  using Circle_2 = Kernel::Circle_2;
  using Dir_2 = Kernel::Direction_2;
  using Line_2 = Kernel::Line_2;
  using Pnt_2 = Kernel::Point_2;
  using Ray_2 = Kernel::Ray_2;
  using Seg_2 = Kernel::Segment_2;
  using Vec_2 = Kernel::Vector_2;

  using Pnt_3 = Kernel::Point_3;
  using Vec_3 = Kernel::Vector_3;
  using Pln_3 = Kernel::Plane_3;

  // Circle_2
  if (! add_attr<Circle_2>(m, "Circle_2")) {
    py::class_<Circle_2> circle2_c(m, "Circle_2");
    export_circle_2<Kernel>(circle2_c);
  }

  // Direction_2
  if (! add_attr<Dir_2>(m, "Direction_2")) {
    py::class_<Dir_2> dir2_c(m, "Direction_2");
    export_dir_2<Kernel>(dir2_c);
  }

  // Line_2
  if (! add_attr<Line_2>(m, "Line_2")) {
    py::class_<Line_2> line2_c(m, "Line_2");
    export_line_2<Kernel>(line2_c);
  }

  // Point_2
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

  // Ray_2
  if (! add_attr<Ray_2>(m, "Ray_2")) {
    py::class_<Ray_2> ray2_c(m, "Ray_2");
    export_ray_2<Kernel>(ray2_c);
  }

  // Segment_2
  if (! add_attr<Seg_2>(m, "Segment_2")) {
    py::class_<Seg_2> seg2_c(m, "Segment_2");
    export_segment_2<Kernel>(seg2_c);
  }

  // Vector_2
  if (! add_attr<Vec_2>(m, "Vector_2")) {
    py::class_<Vec_2> vec2_c(m, "Vector_2");
    export_vector_2<Kernel>(vec2_c);

    if (! is_exact_ft()) {
      using Cci = Kernel::Cartesian_const_iterator_2;
      add_iterator<Cci, Cci>("Cartesian_iterator", vec2_c);
      vec2_c.def("cartesians",
                 [] (const Vec_2& v)
                 { return make_iterator(v.cartesian_begin(), v.cartesian_end()); },
                 py::keep_alive<0, 1>());
    }
  }

  // Triangle_2
  if (! add_attr<Triangle_2>(m, "Triangle_2")) {
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
  }

  // Iso_rectangle_2
  if (! add_attr<Iso_rectangle_2>(m, "Iso_rectangle_2")) {
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
  }

  // Bbox_2
  if (! add_attr<Bbox_2>(m, "Bbox_2")) {
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
  }

  // Aff_transformation_2
  if (! add_attr<Aff_transformation_2>(m, "Aff_transformation_2")) {
    py::class_<Aff_transformation_2>(m, "Aff_transformation_2")
      .def(py::init<>())
      .def(py::init<RT&, RT&, RT&, RT&, RT&>())
      .def(py::init<RT, RT, RT, RT>())
      .def(py::init<RT&, RT&, RT&, RT&, RT&, RT&, RT&>())
      .def(py::init<RT, RT, RT, RT, RT, RT, RT>())
      .def(py::init<const Translation, const Vec_2&>())
      .def(py::init<const Rotation, const Dir_2&, const RT&, const RT&>())
      .def(py::init<const Rotation, const Dir_2&, const RT, const RT>())
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
  }

  // Aff_transformation_3
  if (! add_attr<Aff_transformation_3>(m, "Aff_transformation_3")) {
    py::class_<Aff_transformation_3>(m, "Aff_transformation_3")
      .def(py::init<>())
      ;
  }

  // 3D Objects

  // Point_3
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

  // Weighted_point_3
  if (! add_attr<Weighted_point_3>(m, "Weighted_point_3")) {
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
  }

  // Vector_3
  if (! add_attr<Vec_3>(m, "Vector_3")) {
    py::class_<Vec_3> vec3_c(m, "Vector_3");
    export_vector_3<Kernel>(vec3_c);
  }

  // Plane_3
  if (! add_attr<Pln_3>(m, "Plane_3")) {
    py::class_<Pln_3> pln3_c(m, "Plane_3");
    export_plane_3<Kernel>(pln3_c);
  }

  /// \name Kernel operations
  /// @{
  using Equal_2 = Kernel::Equal_2;
  using Ctr_seg_2 = Kernel::Construct_segment_2;
  using Ctr_midpnt_2 = Kernel::Construct_midpoint_2;
  using Cc_in_between_2 = Kernel::Counterclockwise_in_between_2;

  // Kernel
  if (! add_attr<Kernel>(m, "Kernel")) {
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
      .def("counterclockwise_in_between_2_object",
           [](const Kernel& k)->Cc_in_between_2
           { return k.counterclockwise_in_between_2_object(); })
      ;

    // Equal_2
    using Equal_2_circle = bool(Equal_2::*)(const Circle_2&, const Circle_2&)const;
    using Equal_2_dir = bool(Equal_2::*)(const Dir_2&, const Dir_2&)const;
    using Equal_2_line = bool(Equal_2::*)(const Line_2&, const Line_2&)const;
    using Equal_2_pnt = bool(Equal_2::*)(const Pnt_2&, const Pnt_2&)const;
    using Equal_2_seg = bool(Equal_2::*)(const Seg_2&, const Seg_2&)const;
    using Equal_2_ray = bool(Equal_2::*)(const Ray_2&, const Ray_2&)const;
    using Equal_2_vec = bool(Equal_2::*)(const Vec_2&, const Vec_2&)const;
    py::class_<Equal_2>(ker_c, "Equal_2")
      .def("__call__", static_cast<Equal_2_circle>(&Equal_2::operator()))
      .def("__call__", static_cast<Equal_2_dir>(&Equal_2::operator()))
      .def("__call__", static_cast<Equal_2_line>(&Equal_2::operator()))
      .def("__call__", static_cast<Equal_2_pnt>(&Equal_2::operator()))
      .def("__call__", static_cast<Equal_2_ray>(&Equal_2::operator()))
      .def("__call__", static_cast<Equal_2_seg>(&Equal_2::operator()))
      .def("__call__", static_cast<Equal_2_vec>(&Equal_2::operator()))
      ;

    // Construct_segment_2
    using Ctr_seg_2_op = Seg_2(Ctr_seg_2::*)(const Pnt_2&, const Pnt_2&)const;
    py::class_<Ctr_seg_2>(ker_c, "Construct_segment_2")
      .def("__call__", static_cast<Ctr_seg_2_op>(&Ctr_seg_2::operator()))
      ;

    // Construct_midpoint_2
    using Ctr_midpnt_2_op =
      Pnt_2(Ctr_midpnt_2::*)(const Pnt_2&, const Pnt_2&)const;
    py::class_<Ctr_midpnt_2>(ker_c, "Construct_midpoint_2")
      .def("__call__", static_cast<Ctr_midpnt_2_op>(&Ctr_midpnt_2::operator()))
      ;

    // Counterclockwise_in_between_2
    using Cc_in_between_2_op =
      bool(Cc_in_between_2::*)(const Dir_2&, const Dir_2&, const Dir_2&)const;
    py::class_<Cc_in_between_2>(ker_c, "Counterclockwise_in_between_2")
      .def("__call__", static_cast<Cc_in_between_2_op>(&Cc_in_between_2::operator()))
      ;
  }

  /// @}

  /// \name Global kernel functions
  /// @{
  using Angle_fnc1 = CGAL::Angle(*)(const Vec_2&, const Vec_2&);
  using Angle_fnc2 = CGAL::Angle(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Angle_fnc3 = CGAL::Angle(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("angle", static_cast<Angle_fnc1>(&CGAL::angle<Kernel>));
  m.def("angle", static_cast<Angle_fnc2>(&CGAL::angle<Kernel>));
  m.def("angle", static_cast<Angle_fnc3>(&CGAL::angle<Kernel>));

  using Area_fnc = FT(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("area", static_cast<Area_fnc>(&CGAL::area<Kernel>));

  using Aoal_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("are_ordered_along_line",
        static_cast<Aoal_fnc>(&CGAL::are_ordered_along_line<Kernel>));

  using Asoal_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("are_strictly_ordered_along_line",
        static_cast<Asoal_fnc>(&CGAL::are_strictly_ordered_along_line<Kernel>));

  using Bc_fnc1 = Pnt_2(*)(const Pnt_2&, const FT&, const Pnt_2&, const FT&);
  using Bc_fnc2 = Pnt_2(*)(const Pnt_2&, const FT&, const Pnt_2&, const FT&, const Pnt_2&, const FT&);
  using Bc_fnc3 = Pnt_2(*)(const Pnt_2&, const FT&, const Pnt_2&, const FT&, const Pnt_2&, const FT&, const Pnt_2&, const FT&);
  m.def("barycenter", static_cast<Bc_fnc1>(&CGAL::barycenter<Kernel>));
  m.def("barycenter", static_cast<Bc_fnc2>(&CGAL::barycenter<Kernel>));
  m.def("barycenter", static_cast<Bc_fnc3>(&CGAL::barycenter<Kernel>));

  using Bisector_fnc = Line_2(*)(const Pnt_2&, const Pnt_2&);
  m.def("bisector", static_cast<Bisector_fnc>(&CGAL::bisector<Kernel>));

  // Requires sqrt operation
  //def<Line_2(const Line_2&, const Line_2&)>("bisector", &CGAL::bisector);

  using Cr_fnc1 = Pnt_2(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Cr_fnc2 = Pnt_2(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Cr_fnc3 = Pnt_2(*)(const Triangle_2&);
  m.def("centroid", static_cast<Cr_fnc1>(&CGAL::centroid<Kernel>));
  m.def("centroid", static_cast<Cr_fnc2>(&CGAL::centroid<Kernel>));
  m.def("centroid", static_cast<Cr_fnc3>(&CGAL::centroid<Kernel>));

  using Cc_fnc1 = Pnt_2(*)(const Pnt_2&, const Pnt_2&);
  using Cc_fnc2 = Pnt_2(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Cc_fnc3 = Pnt_2(*)(const Triangle_2&);
  m.def("circumcenter", static_cast<Cc_fnc1>(&CGAL::circumcenter<Kernel>));
  m.def("circumcenter", static_cast<Cc_fnc2>(&CGAL::circumcenter<Kernel>));
  m.def("circumcenter", static_cast<Cc_fnc3>(&CGAL::circumcenter<Kernel>));

  using Caoral_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("collinear_are_ordered_along_line", static_cast<Caoral_fnc>(&CGAL::collinear_are_ordered_along_line<Kernel>));

  using Casoral_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("collinear_are_strictly_ordered_along_line", static_cast<Casoral_fnc>(&CGAL::collinear_are_strictly_ordered_along_line<Kernel>));

  using Clr_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("collinear", static_cast<Clr_fnc>(&CGAL::collinear<Kernel>));

  using Cdtp_fnc = CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("compare_distance_to_point", static_cast<Cdtp_fnc>(&CGAL::compare_distance_to_point<Kernel>));

  using Cl_fnc = CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&);
  m.def("compare_lexicographically", static_cast<Cl_fnc>(&CGAL::compare_lexicographically<Kernel>));

#if !defined(_MSC_VER)
  // MSVC does handle the following for some reason....
  using Csdtl_fnc1 = CGAL::Comparison_result(*)(const Line_2&, const Pnt_2&, const Pnt_2&);
  m.def("compare_signed_distance_to_line", static_cast<Csdtl_fnc1>(&CGAL::compare_signed_distance_to_line<Kernel>));
#endif
  using Csdtl_fnc2 = CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("compare_signed_distance_to_line", static_cast<Csdtl_fnc2>(&CGAL::compare_signed_distance_to_line<Kernel>));

  using Cs_fnc1 = CGAL::Comparison_result(*)(const Line_2&, const Line_2&);
  using Cs_fnc2 = CGAL::Comparison_result(*)(const Segment_2&, const Segment_2&);
  m.def("compare_slope", static_cast<Cs_fnc1>(&CGAL::compare_slope<Kernel>));
  m.def("compare_slope", static_cast<Cs_fnc2>(&CGAL::compare_slope<Kernel>));

  using Csd_fnc = CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&, const FT&);
  m.def("compare_squared_distance", static_cast<Csd_fnc>(&CGAL::compare_squared_distance<Kernel>));

  using Cx_fnc1 = CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&);
  using Cx_fnc2 = CGAL::Comparison_result(*)(const Pnt_2&, const Line_2&, const Line_2&);
  using Cx_fnc3 = CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&);
  using Cx_fnc4 = CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&, const Line_2&);
  m.def("compare_x", static_cast<Cx_fnc1>(&CGAL::compare_x<Kernel>));
  m.def("compare_x", static_cast<Cx_fnc2>(&CGAL::compare_x<Kernel>));
  m.def("compare_x", static_cast<Cx_fnc3>(&CGAL::compare_x<Kernel>));
  m.def("compare_x", static_cast<Cx_fnc4>(&CGAL::compare_x<Kernel>));

  using Cxy_fnc = CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&);
  m.def("compare_xy", static_cast<Cxy_fnc>(&CGAL::compare_xy<Kernel>));

  using Cxaty_fnc1 = CGAL::Comparison_result(*)(const Pnt_2&, const Line_2&);
  using Cxaty_fnc2 = CGAL::Comparison_result(*)(const Pnt_2&, const Line_2&, const Line_2&);
  using Cxaty_fnc3 = CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&);
  using Cxaty_fnc4 = CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&, const Line_2&);
  m.def("compare_x_at_y", static_cast<Cxaty_fnc1>(&CGAL::compare_x_at_y<Kernel>));
  m.def("compare_x_at_y", static_cast<Cxaty_fnc2>(&CGAL::compare_x_at_y<Kernel>));
  m.def("compare_x_at_y", static_cast<Cxaty_fnc3>(&CGAL::compare_x_at_y<Kernel>));
  m.def("compare_x_at_y", static_cast<Cxaty_fnc4>(&CGAL::compare_x_at_y<Kernel>));

  using Cyatx_fnc1 = CGAL::Comparison_result(*)(const Pnt_2&, const Line_2&);
  using Cyatx_fnc2 = CGAL::Comparison_result(*)(const Pnt_2&, const Line_2&, const Line_2&);
  using Cyatx_fnc3 = CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&);
  using Cyatx_fnc4 = CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&, const Line_2&);
  using Cyatx_fnc5 = CGAL::Comparison_result(*)(const Pnt_2&, const Segment_2&);
  using Cyatx_fnc6 = CGAL::Comparison_result(*)(const Pnt_2&, const Segment_2&, const Segment_2&);
  m.def("compare_y_at_x", static_cast<Cyatx_fnc1>(&CGAL::compare_y_at_x<Kernel>));
  m.def("compare_y_at_x", static_cast<Cyatx_fnc2>(&CGAL::compare_y_at_x<Kernel>));
  m.def("compare_y_at_x", static_cast<Cyatx_fnc3>(&CGAL::compare_y_at_x<Kernel>));
  m.def("compare_y_at_x", static_cast<Cyatx_fnc4>(&CGAL::compare_y_at_x<Kernel>));
  m.def("compare_y_at_x", static_cast<Cyatx_fnc5>(&CGAL::compare_y_at_x<Kernel>));
  m.def("compare_y_at_x", static_cast<Cyatx_fnc6>(&CGAL::compare_y_at_x<Kernel>));

  using Cy_fnc1 = CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&);
  using Cy_fnc2 = CGAL::Comparison_result(*)(const Pnt_2&, const Line_2&, const Line_2&);
  using Cy_fnc3 = CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&);
  using Cy_fnc4 = CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&, const Line_2&);
  m.def("compare_y", static_cast<Cy_fnc1>(&CGAL::compare_y<Kernel>));
  m.def("compare_y", static_cast<Cy_fnc2>(&CGAL::compare_y<Kernel>));
  m.def("compare_y", static_cast<Cy_fnc3>(&CGAL::compare_y<Kernel>));
  m.def("compare_y", static_cast<Cy_fnc4>(&CGAL::compare_y<Kernel>));

  using Cyx_fnc = CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&);
  m.def("compare_yx", static_cast<Cyx_fnc>(&CGAL::compare_yx<Kernel>));

  using Dt_fnc = FT(*)(const Vec_2&, const Vec_2&);
  m.def("determinant", static_cast<Dt_fnc>(&CGAL::determinant<Kernel>));

  using Hldtp_fnc = bool (*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("has_larger_distace_to_point", static_cast<Hldtp_fnc>(&CGAL::has_larger_distance_to_point<Kernel>));

#if !defined(_MSC_VER)
  // MSVC does handle the following for some reason....
  using Hldtl_fnc1 = bool(*)(const Line_2&, const Pnt_2&, const Pnt_2&);
  m.def("has_larger_signed_distance_to_line", static_cast<Hldtl_fnc1>(&CGAL::has_larger_signed_distance_to_line<Kernel>));
#endif
  using Hldtl_fnc2 = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("has_larger_signed_distance_to_line", static_cast<Hldtl_fnc2>(&CGAL::has_larger_signed_distance_to_line<Kernel>));

  using Hsdtp_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("has_smaller_distace_to_point", static_cast<Hsdtp_fnc>(&CGAL::has_smaller_distance_to_point<Kernel>));

#if !defined(_MSC_VER)
  // MSVC does handle the following for some reason....
  using Hssdtl_fnc1 = bool(*)(const Line_2&, const Pnt_2&, const Pnt_2&);
  m.def("has_smaller_signed_distance_to_line", static_cast<Hssdtl_fnc1>(&CGAL::has_smaller_signed_distance_to_line<Kernel>));
#endif

  using Hssdtl_fnc2 = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("has_smaller_signed_distance_to_line", static_cast<Hssdtl_fnc2>(&CGAL::has_smaller_signed_distance_to_line<Kernel>));

  using Lid_fnc = FT(*)(const Pnt_2&, const Pnt_2&);
  m.def("l_infinity_distance", static_cast<Lid_fnc>(&CGAL::l_infinity_distance<Kernel>));

  using Lt_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("left_turn", static_cast<Lt_fnc>(&CGAL::left_turn<Kernel>));

  using Lxyl_fnc = bool(*)(const Pnt_2&, const Pnt_2&);
  m.def("lexicographically_xy_larger", static_cast<Lxyl_fnc>(&CGAL::lexicographically_xy_larger<Kernel>));

  using Lxyloe_fnc = bool(*)(const Pnt_2&, const Pnt_2&);
  m.def("lexicographically_xy_larger_or_equal", static_cast<Lxyloe_fnc>(&CGAL::lexicographically_xy_larger_or_equal<Kernel>));

  using Lxys_fnc = bool(*)(const Pnt_2&, const Pnt_2&);
  m.def("lexicographically_xy_smaller", static_cast<Lxys_fnc>(&CGAL::lexicographically_xy_smaller<Kernel>));

  using Lxysoe_fnc = bool(*)(const Pnt_2&, const Pnt_2&);
  m.def("lexicographically_xy_smaller_or_equal", static_cast<Lxysoe_fnc>(&CGAL::lexicographically_xy_smaller_or_equal<Kernel>));

  using Maxv_fnc = Pnt_2(*)(const Iso_rectangle_2&);
  m.def("max_vertex", static_cast<Maxv_fnc>(&CGAL::max_vertex<Kernel>));

  using Mp_fnc = Pnt_2(*)(const Pnt_2&, const Pnt_2&);
  m.def("midpoint", static_cast<Mp_fnc>(&CGAL::midpoint<Kernel>));

  using Minv_fnc = Pnt_2(*)(const Iso_rectangle_2&);
  m.def("min_vertex", static_cast<Minv_fnc>(&CGAL::min_vertex<Kernel>));

  using Or_fnc1 = CGAL::Orientation(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Or_fnc2 = CGAL::Orientation(*)(const Vec_2&, const Vec_2&);
  m.def("orientation", static_cast<Or_fnc1>(&CGAL::orientation));
  m.def("orientation", static_cast<Or_fnc2>(&CGAL::orientation));

  using Pl_fnc1 = bool(*)(const Line_2&, const Line_2&);
  using Pl_fnc2 = bool(*)(const Ray_2&, const Ray_2&);
  using Pl_fnc3 = bool(*)(const Segment_2&, const Segment_2&);
  m.def("parallel", static_cast<Pl_fnc1>(&CGAL::parallel));
  m.def("parallel", static_cast<Pl_fnc2>(&CGAL::parallel));
  m.def("parallel", static_cast<Pl_fnc3>(&CGAL::parallel));

  using Rl_fnc = Line_2 (*)(const Circle_2&, const Circle_2&);
  m.def("radical_line", static_cast<Rl_fnc>(&CGAL::radical_line<Kernel>));

  // Parameterized by FT (not Kernel)
  using Rra_fnc = void(*)(const RT&, const RT&, RT&, RT&, RT&, const RT&, const RT&);
  m.def("rational_rotation_approximation", static_cast<Rra_fnc>(&CGAL::rational_rotation_approximation<FT>));

  using Rt_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("right_turn", static_cast<Rt_fnc>(&CGAL::right_turn<Kernel>));

  using Sp_fnc2 = FT(*)(const Vec_2&, const Vec_2&);
  using Sp_fnc3 = FT(*)(const Vec_3&, const Vec_3&);
  m.def("scalar_product", static_cast<Sp_fnc2>(&CGAL::scalar_product<Kernel>));
  m.def("scalar_product", static_cast<Sp_fnc3>(&CGAL::scalar_product<Kernel>));

  using Cp_fnc3 = Vec_3(*)(const Vec_3&, const Vec_3&);
  m.def("cross_product", static_cast<Cp_fnc3>(&CGAL::cross_product<Kernel>));

  using Sobc_fnc1 = CGAL::Bounded_side(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Sobc_fnc2 = CGAL::Bounded_side(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("side_of_bounded_circle", static_cast<Sobc_fnc1>(&CGAL::side_of_bounded_circle<Kernel>));
  m.def("side_of_bounded_circle", static_cast<Sobc_fnc2>(&CGAL::side_of_bounded_circle<Kernel>));

  using Sooc_fnc = CGAL::Oriented_side(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("side_of_oriented_circle", static_cast<Sooc_fnc>(&CGAL::side_of_oriented_circle<Kernel>));

  bind_squared_distance_types<Pnt_2, Line_2, Ray_2, Segment_2, Triangle_2>(m);

  using Sd_fnc1 = FT(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Sd_fnc2 = FT(*)(const Pnt_2&, const Pnt_2&);
  using Sd_fnc3 = FT(*)(const Pnt_2&);
  m.def("squared_radius", static_cast<Sd_fnc1>(&CGAL::squared_radius<Kernel>));
  m.def("squared_radius", static_cast<Sd_fnc2>(&CGAL::squared_radius<Kernel>));
  m.def("squared_radius", static_cast<Sd_fnc3>(&CGAL::squared_radius<Kernel>));

  using Xe_fnc = bool(*)(const Pnt_2&, const Pnt_2&);
  m.def("x_equal", static_cast<Xe_fnc>(&CGAL::x_equal<Kernel>));

  using Ye_fnc =bool(*)(const Pnt_2&, const Pnt_2&) ;
  m.def("y_equal", static_cast<Ye_fnc>(&CGAL::y_equal<Kernel>));

  using Do_fnc = bool(*)(const Bbox_2&, const Bbox_2&);
  m.def("do_overlap", static_cast<Do_fnc>(&CGAL::do_overlap));

  using Cmp3_fnc = CGAL::Comparison_result(*)(const Pnt_3&, const Pnt_3&);
  m.def("compare_z", static_cast<Cmp3_fnc>(&CGAL::compare_z<Kernel>))
    ;

  /// @}
}
