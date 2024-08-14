// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_KERNEL_HPP
#define CGALPY_EXPORT_KERNEL_HPP

#include <nanobind/nanobind.h>

// 2D functors
#include "CGALPY/Kernel/export_aff_transformation_2.hpp"
#include "CGALPY/Kernel/export_circle_2.hpp"
#include "CGALPY/Kernel/export_dir_2.hpp"
#include "CGALPY/Kernel/export_iso_rectangle_2.hpp"
#include "CGALPY/Kernel/export_line_2.hpp"
#include "CGALPY/Kernel/export_point_2.hpp"
#include "CGALPY/Kernel/export_ray_2.hpp"
#include "CGALPY/Kernel/export_segment_2.hpp"
#include "CGALPY/Kernel/export_triangle_2.hpp"
#include "CGALPY/Kernel/export_vector_2.hpp"

// 3D functors
#include "CGALPY/Kernel/export_dir_3.hpp"
#include "CGALPY/Kernel/export_iso_cuboid_3.hpp"
#include "CGALPY/Kernel/export_point_3.hpp"
#include "CGALPY/Kernel/export_plane_3.hpp"
#include "CGALPY/Kernel/export_sphere_3.hpp"
#include "CGALPY/Kernel/export_triangle_3.hpp"
#include "CGALPY/Kernel/export_vector_3.hpp"
#include "CGALPY/Kernel/export_weighted_point_3.hpp"

#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

//
template <typename Kernel_, typename C_>
void export_kernel(C_& ker_c) {
  using Ker = Kernel_;
  using Ft = typename Ker::FT;

  // Kernel 2D objects
  using Aff_trans_2 = typename Ker::Aff_transformation_2;
  using Circle_2 = typename Ker::Circle_2;
  using Dir_2 = typename Ker::Direction_2;
  using Iso_rect_2 = typename Ker::Iso_rectangle_2;
  using Line_2 = typename Ker::Line_2;
  using Pnt_2 = typename Ker::Point_2;
  using Ray_2 = typename Ker::Ray_2;
  using Seg_2 = typename Ker::Segment_2;
  using Tri_2 = typename Ker::Triangle_2;
  using Vec_2 = typename Ker::Vector_2;

  // Kernel 3D objects
  using Aff_trans_3 = typename Ker::Aff_transformation_3;
  using Dir_3 = typename Ker::Direction_3;
  using Pln_3 = typename Ker::Plane_3;
  using Pnt_3 = typename Ker::Point_3;
  using Sfr_3 = typename Ker::Sphere_3;
  using Tri_3 = typename Ker::Triangle_3;
  using Vec_3 = typename Ker::Vector_3;
  using Wd_pnt_3 = typename Ker::Weighted_point_3;
  using Ic_3 = typename Ker::Iso_cuboid_3;

  // Kernel 2D operators
  using Equal_2 = typename Ker::Equal_2;
  using Ctr_midpnt_2 = typename Ker::Construct_midpoint_2;
  using Ctr_pnt_2 = typename Ker::Construct_point_2;
  using Ctr_seg_2 = typename Ker::Construct_segment_2;
  using Ctr_proj_xy_pnt_2 = typename Ker::Construct_projected_xy_point_2;
  using Cc_in_between_2 = typename Ker::Counterclockwise_in_between_2;

  // Kernel 3D operators
  using Ctr_pln_3 = typename Ker::Construct_plane_3;
  using Ctr_pnt_3 = typename Ker::Construct_point_3;
  using Ctr_tran_pnt_3 = typename Ker::Construct_translated_point_3;
  using Cmp_z_3 = typename Ker::Compare_z_3;

  ker_c.def(py::init<>())
    // 2D operators
    .def("equal_2_object",
         [](const Ker& k)->Equal_2{ return k.equal_2_object(); })
    .def("construct_midpoint_2_object",
         [](const Ker& k)->Ctr_midpnt_2
         { return k.construct_midpoint_2_object(); })
    .def("construct_point_2_object",
         [](const Ker& k)->Ctr_pnt_2
         { return k.construct_point_2_object(); })
    .def("construct_segment_2_object",
         [](const Ker& k)->Ctr_seg_2
         { return k.construct_segment_2_object(); })
    .def("construct_projected_xy_point_2_object",
         [](const Ker& k)->Ctr_proj_xy_pnt_2
         { return k.construct_projected_xy_point_2_object(); })
    .def("counterclockwise_in_between_2_object",
         [](const Ker& k)->Cc_in_between_2
         { return k.counterclockwise_in_between_2_object(); })

    // 3D operators
    .def("construct_plane_3_object",
         [](const Ker& k)->Ctr_pln_3{ return k.construct_plane_3_object(); })
    .def("construct_point_3_object",
         [](const Ker& k)->Ctr_pnt_3{ return k.construct_point_3_object(); })
    .def("construct_translated_point_3_object",
         [](const Ker& k)->Ctr_tran_pnt_3
         { return k.construct_translated_point_3_object(); })
    .def("compare_z_3_object",
         [](const Ker& k)->Cmp_z_3{ return k.compare_z_3_object(); })
    ;

  ////////// 2D

  //////// 2D Objects

  // Circle_2
  if (! add_attr<Circle_2>(ker_c, "Circle_2")) {
    py::class_<Circle_2> circle2_c(ker_c, "Circle_2");
    export_circle_2<Ker>(circle2_c);
  }

  // Direction_2
  if (! add_attr<Dir_2>(ker_c, "Direction_2")) {
    py::class_<Dir_2> dir2_c(ker_c, "Direction_2");
    export_dir_2<Ker>(dir2_c);
  }

  // Line_2
  if (! add_attr<Line_2>(ker_c, "Line_2")) {
    py::class_<Line_2> line2_c(ker_c, "Line_2");
    export_line_2<Ker>(line2_c);
  }

  // Point_2
  if (! add_attr<Pnt_2>(ker_c, "Point_2")) {
    py::class_<Pnt_2> pnt2_c(ker_c, "Point_2");
    export_point_2<Ker>(pnt2_c);
  }

  // Ray_2
  if (! add_attr<Ray_2>(ker_c, "Ray_2")) {
    py::class_<Ray_2> ray2_c(ker_c, "Ray_2");
    export_ray_2<Ker>(ray2_c);
  }

  // Segment_2
  if (! add_attr<Seg_2>(ker_c, "Segment_2")) {
    py::class_<Seg_2> seg2_c(ker_c, "Segment_2");
    export_segment_2<Ker>(seg2_c);
  }

  // Vector_2
  if (! add_attr<Vec_2>(ker_c, "Vector_2")) {
    py::class_<Vec_2> vec2_c(ker_c, "Vector_2");
    export_vector_2<Ker>(vec2_c);
  }

  // Triangle_2
  if (! add_attr<Tri_2>(ker_c, "Triangle_2")) {
    py::class_<Tri_2> tri2_c(ker_c, "Triangle_2");
    export_triangle_2<Ker>(tri2_c);
  }

  // Iso_rectangle_2
  if (! add_attr<Iso_rect_2>(ker_c, "Iso_rectangle_2")) {
    py::class_<Iso_rect_2>iso2_c(ker_c, "Iso_rectangle_2");
    export_iso_rectangle_2<Ker>(iso2_c);
  }

  // Aff_transformation_2
  if (! add_attr<Aff_trans_2>(ker_c, "Aff_transformation_2")) {
    py::class_<Aff_trans_2> aff2_c(ker_c, "Aff_transformation_2");
    export_aff_transformation_2<Ker>(aff2_c);
  }

  //////// 2D Operators

  // Construct_point_2
  py::class_<Ctr_pnt_2>(ker_c, "Construct_point_2")
    .def("__call__", [](Ctr_pnt_2 ctr, const Pnt_2& other)->Pnt_2 const
                     { return ctr(other); })
    .def("__call__", [](Ctr_pnt_2 ctr, CGAL::Origin org)->Pnt_2 const
                     { return ctr(org); })
    .def("__call__", [](Ctr_pnt_2 ctr, const Ft& x, const Ft& y)->Pnt_2 const
                     { return ctr(x, y); })
    ;

  // Construct_midpoint_2
  using Ctr_midpnt_2_op =
    Pnt_2(Ctr_midpnt_2::*)(const Pnt_2&, const Pnt_2&)const;
  py::class_<Ctr_midpnt_2>(ker_c, "Construct_midpoint_2")
    .def("__call__", static_cast<Ctr_midpnt_2_op>(&Ctr_midpnt_2::operator()))
    ;

  // Construct_projected_xy_point_2
  using Ctr_proj_xy_pnt_2_op =
    Pnt_2(Ctr_proj_xy_pnt_2::*)(const Pln_3&, const Pnt_3&)const;
  py::class_<Ctr_proj_xy_pnt_2>(ker_c, "Construct_projected_xy_point_2")
    .def("__call__",
         static_cast<Ctr_proj_xy_pnt_2_op>(&Ctr_proj_xy_pnt_2::operator()))
    ;

  // Construct_segment_2
  using Ctr_seg_2_op = Seg_2(Ctr_seg_2::*)(const Pnt_2&, const Pnt_2&)const;
  py::class_<Ctr_seg_2>(ker_c, "Construct_segment_2")
    .def("__call__", static_cast<Ctr_seg_2_op>(&Ctr_seg_2::operator()))
    ;

  // Counterclockwise_in_between_2
  using Cc_in_between_2_op =
    bool(Cc_in_between_2::*)(const Dir_2&, const Dir_2&, const Dir_2&)const;
  py::class_<Cc_in_between_2>(ker_c, "Counterclockwise_in_between_2")
    .def("__call__", static_cast<Cc_in_between_2_op>(&Cc_in_between_2::operator()))
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

  ////////// 3D

  //////// 3D Objects

  // Aff_transformation_3
  if (! add_attr<Aff_trans_3>(ker_c, "Aff_transformation_3")) {
    py::class_<Aff_trans_3>(ker_c, "Aff_transformation_3")
      .def(py::init<>())
      ;
  }

  // // Direction_3
  if (! add_attr<Dir_3>(ker_c, "Direction_3")) {
    py::class_<Dir_3> dir3_c(ker_c, "Direction_3");
    export_dir_3<Ker>(dir3_c);
  }

  // Plane_3
  if (! add_attr<Pln_3>(ker_c, "Plane_3")) {
    py::class_<Pln_3> pln3_c(ker_c, "Plane_3");
    export_plane_3<Ker>(pln3_c);
  }

  // Point_3
  if (! add_attr<Pnt_3>(ker_c, "Point_3")) {
    py::class_<Pnt_3> pnt3_c(ker_c, "Point_3");
    export_point_3<Ker>(pnt3_c);
  }

  // Sphere_3
  if (! add_attr<Sfr_3>(ker_c, "Sphere_3")) {
    py::class_<Sfr_3> sfr3_c(ker_c, "Sphere_3");
    export_sphere_3<Ker>(sfr3_c);
  }

  // Triangle_3
  if (! add_attr<Tri_3>(ker_c, "Triangle_3")) {
    py::class_<Tri_3> tri3_c(ker_c, "Triangle_3");
    export_triangle_3<Ker>(tri3_c);
  }

  // Vector_3
  if (! add_attr<Vec_3>(ker_c, "Vector_3")) {
    py::class_<Vec_3> vec3_c(ker_c, "Vector_3");
    export_vector_3<Ker>(vec3_c);
  }

  // Weighted_point_3
  if (! add_attr<Wd_pnt_3>(ker_c, "Weighted_point_3")) {
    py::class_<Wd_pnt_3> wd_pnt3_c(ker_c, "Weighted_point_3");
    export_weighted_point_3<Ker>(wd_pnt3_c);
  }

  // Iso_cuboid_3
  if (! add_attr<Ic_3>(ker_c, "Iso_cuboid_3")) {
    py::class_<Ic_3> iso3_c(ker_c, "Iso_cuboid_3");
    export_iso_cuboid_3<Ker>(iso3_c);
  }

  //////// 3D Operators

  // Compare_z_3
  using Cmp_z_3_op =
    CGAL::Comparison_result(Cmp_z_3::*)(const Pnt_3&, const Pnt_3&)const;
  py::class_<Cmp_z_3>(ker_c, "Compare_z_3")
    .def("__call__", static_cast<Cmp_z_3_op>(&Cmp_z_3::operator()))
    ;

  // Construct_plane_3
  using Ctr_pln_3_op = Pln_3(Ctr_pln_3::*)(const Pnt_3&, const Dir_3&)const;
  py::class_<Ctr_pln_3>(ker_c, "Construct_plane_3")
    .def("__call__", static_cast<Ctr_pln_3_op>(&Ctr_pln_3::operator()))
    ;

  // // Construct_point_3
  py::class_<Ctr_pnt_3>(ker_c, "Construct_point_3")
    .def("__call__",
         [](Ctr_pnt_3 ctr, const Pnt_3& other)->Pnt_3 const
         { return ctr(other); })
    .def("__call__",
         [](Ctr_pnt_3 ctr, CGAL::Origin org)->Pnt_3 const { return ctr(org); })
    .def("__call__",
         [](Ctr_pnt_3 ctr, const Ft& x, const Ft& y, const Ft& z)->Pnt_3 const
         { return ctr(x, y, z); })
    ;

  // Construct_translated_point_3
  using Ctr_tran_pnt_3_op =
    Pnt_3(Ctr_tran_pnt_3::*)(const Pnt_3&, const Vec_3&)const;
  py::class_<Ctr_tran_pnt_3>(ker_c, "Construct_translated_point_3")
    .def("__call__", static_cast<Ctr_tran_pnt_3_op>(&Ctr_tran_pnt_3::operator()))
    ;
}

#endif
