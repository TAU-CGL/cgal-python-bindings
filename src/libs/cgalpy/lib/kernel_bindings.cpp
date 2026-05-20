// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <type_traits>

#include <boost/multiprecision/cpp_int.hpp>
#if CGAL_USE_GMP
#include <boost/multiprecision/gmp.hpp>
#endif

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <CGAL/IO/polygon_soup_io.h>
#include <CGAL/basic.h>
#include <CGAL/Mesh_constant_domain_field_3.h>
#if CGAL_USE_GMP
#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>
#include <CGAL/GMP/Gmpz_type.h>
#include <CGAL/GMP/Gmpq_type.h>
#endif

#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/cartesian_product.hpp"
#include "CGALPY/config.hpp"
#include "CGALPY/export_boost_multiprecision.hpp"
#include "CGALPY/kernel_type.hpp"
#include "CGALPY/Kernel/export_ft.hpp"
#include "CGALPY/Kernel/export_rt.hpp"
#include <cgalpy/Kernel_23_docstrings.hpp>
namespace doc = cgalpy::docstrings::Kernel_23;
#include "CGALPY/Kernel/export_kernel.hpp"
#include "CGALPY/parse_named_parameters.hpp"
#include "CGALPY/stl_forward_iterator.hpp"
#include "CGALPY/to_string.hpp"

// 2D functors
#include "CGALPY/Kernel/export_circle_2.hpp"
#include "CGALPY/Kernel/export_dir_2.hpp"
#include "CGALPY/Kernel/export_line_2.hpp"
#include "CGALPY/Kernel/export_point_2.hpp"
#include "CGALPY/Kernel/export_ray_2.hpp"
#include "CGALPY/Kernel/export_segment_2.hpp"
#include "CGALPY/Kernel/export_triangle_2.hpp"
#include "CGALPY/Kernel/export_vector_2.hpp"

// 3D functors
#include "CGALPY/Kernel/export_aff_transformation_3.hpp"
#include "CGALPY/Kernel/export_circle_3.hpp"
#include "CGALPY/Kernel/export_dir_3.hpp"
#include "CGALPY/Kernel/export_point_3.hpp"
#include "CGALPY/Kernel/export_plane_3.hpp"
#include "CGALPY/Kernel/export_line_3.hpp"
#include "CGALPY/Kernel/export_ray_3.hpp"
#include "CGALPY/Kernel/export_segment_3.hpp"
#include "CGALPY/Kernel/export_tetrahedron_3.hpp"
#include "CGALPY/Kernel/export_sphere_3.hpp"
#include "CGALPY/Kernel/export_triangle_3.hpp"
#include "CGALPY/Kernel/export_vector_3.hpp"
#include "CGALPY/Kernel/export_weighted_point_3.hpp"

#include "CGALPY/Kernel/export_mesh_constant_domain_field_3.hpp"

namespace py = nanobind;

extern void export_bbox_2(py::class_<CGAL::Bbox_2>&);
extern void export_bbox_3(py::class_<CGAL::Bbox_3>&);

#if CGAL_USE_GMP
extern void export_gmpz(py::module_&);
extern void export_gmpq(py::module_&);
#endif

#if CGAL_USE_GMPXX
extern void export_mpq_class(py::module_&);
extern void export_mpz_class(py::module_&);
#endif

extern void export_mpz_int(py::module_&);
extern void export_mpq_rational(py::module_&);

namespace ker {

CGAL::Bbox_2 bbox_2(const py::list& points) {
  auto begin = stl_forward_iterator<Point_2>(points, true);
  auto end = stl_forward_iterator<Point_2>(points, false);
  return CGAL::bbox_2(begin, end);
}

}

//template<typename T>
//size_t hash(T& immutable) {
//  std::ostringstream stream;
//  stream << immutable;
//  std::string s = stream.str();
//  return boost::hash<std::string>()(s); // TODO: two equal objects can have different string representation
//}

// The supported overloaded functions CGAL::intersection(T1& t1, T2& t2) have
// a complicated return value; it's a polymorphic object. It can be nothing or
// one of several types  that depends on the type of the input arguments.
// Therefore, the selection is implemented in a different way as follows. We
// still use the type of the return value (of the particular
// CGAL::intersection(T1& t1, T2& t2) function, but we do not try to match
// this type to the type of an argument. Instead, we use this type as the
// default value of an unnamed template parameter.
template<typename, typename> void bind_squared_distance(py::module_&, ...) {}

//! Implementation
template <typename T1, typename T2,
          typename = decltype(CGAL::squared_distance<Kernel>(T1(), T2()))>
void bind_squared_distance(py::module_& m, bool) {
  using Sd_fnc = FT(*)(const T1&, const T2&);
  m.def("squared_distance",
        static_cast<Sd_fnc>(&CGAL::squared_distance<Kernel>));
}

//! Squared distance wrapper
template <typename Arg, typename ... Types> struct Sd_wrapper {
  void operator()(Arg& arg) { bind_squared_distance<Types...>(arg, true); }
};

//!
void export_kernel_module(py::module_& m) {
  constexpr auto ri(py::rv_policy::reference_internal);

#if ((CGALPY_KERNEL == CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL) || \
     (CGALPY_KERNEL == CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  if (! add_attr<FT>(m, "FT")) {
    py::class_<FT> ft_c(m, "FT");
    export_ft(ft_c);
  }
#else

#if CGAL_USE_GMP
  if (! add_attr<CGAL::Gmpz>(m, "Gmpz")) export_gmpz(m);
  if (! add_attr<CGAL::Gmpq>(m, "Gmpq")) export_gmpq(m);
#endif

#if CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT
  py::class_<FT> ft_c(m, "FT");
  export_ft(ft_c);
#endif

#if ((CGALPY_KERNEL == CGALPY_KERNEL_EPEC) ||                              \
     (CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ))
    // (CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT) ||

  using Fte = FT::Exact_type;
  using Fta = FT::Approximate_type;
  if (! add_attr<FT>(m, "FT")) {
    py::class_<FT> ft_c(m, "FT");
    export_ft(ft_c);

    ft_c.def(py::init<Fte>())
      .def("__init__", [](FT* self, const std::string& str) { new (self) FT(Fte(str)); })
      .def("__init__", [](FT* self, int nom, int den) { new (self) FT(Fte(nom, den)); })
      .def("to_double", [](const FT& ft)->double { return CGAL::to_double(ft); })
      .def("exact", [](const FT& ft)->const Fte& { return ft.exact();}, ri)
      .def("approx", [](const FT& ft)->const Fta& { return ft.approx();} )
      ;
  }

  // Export GMP boost multi-precision
#if CGAL_USE_GMP
  using mpz_int = boost::multiprecision::mpz_int;
  if (! add_attr<mpz_int>(m, "mpz_int")) {
    py::class_<mpz_int> mpz_int_c(m, "mpz_int");
    export_boost_multiprecision_int(mpz_int_c);
  }

  using mpq_rat = boost::multiprecision::mpq_rational;
  if (! add_attr<mpq_rat>(m, "mpq_rational")) {
    py::class_<mpq_rat> mpq_rat_c(m, "mpq_rational");
    export_boost_multiprecision_rational(mpq_rat_c);
  }
#endif

  // Export CPP boost multi-precision
  using cpp_int = boost::multiprecision::cpp_int;
  if (! add_attr<cpp_int>(m, "cpp_int")) {
    py::class_<cpp_int> cpp_int_c(m, "cpp_int");
    export_boost_multiprecision_int(cpp_int_c);
  }

  using cpp_rat = boost::multiprecision::cpp_rational;
  if (! add_attr<cpp_rat>(m, "cpp_rational")) {
    py::class_<cpp_rat> cpp_rat_c(m, "cpp_rational");
    export_boost_multiprecision_rational(cpp_rat_c);
  }

#if CGAL_USE_GMPXX
  // If gmp is supported, the exact number type is mpq_class defined in the global namespace in <gmpxx.h>
  export_mpz_class(m);
  export_mpq_class(m);
#endif
  // Fall back
  if (! add_attr<Fte>(m, "Exact")) {
    py::class_<Fte> fte_c(m, "Exact");
    fte_c.def(py::init<const Fte&>())
      ;

    add_insertion(fte_c, "__str__");
    add_insertion(fte_c, "__repr__");
    add_extraction(fte_c);
  }

  if (! add_attr<RT>(m, "RT")) {
    py::class_<RT> rt_c(m, "RT");
    export_rt(rt_c);
  }

#endif
#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))

  struct dummy {};
  if (! add_attr<dummy>(m, "FT")) {
    py::class_<dummy> ft_c(m, "FT", py::sig("class FT(float)"));
  }

#endif
#endif

  //class_<RT>(m, "RT")
  //  .def(init<RT::Exact_type>())
  //  .def(self_ns::str(self_ns::self))
  //  .def(self == self)
  //  ;

  // Kernel
  if (! add_attr<Kernel>(m, "Kernel")) {
    py::class_<Kernel> ker_c(m, "Kernel");
    export_kernel<Kernel>(ker_c);
  }

  // Bbox_2
  if (! add_attr<Bbox_2>(m, "Bbox_2")) {
    py::class_<Bbox_2> bbox_c(m, "Bbox_2", doc::Bbox_2__class__);
    export_bbox_2(bbox_c);
  }

  // Bbox_3
  if (! add_attr<Bbox_3>(m, "Bbox_3")) {
    py::class_<Bbox_3> bbox_c(m, "Bbox_3", doc::Bbox_3__class__);
    export_bbox_3(bbox_c);
  }

  using Orientation = CGAL::Orientation;

  // Kernel objects
  using Circle_2 = Kernel::Circle_2;
  using Dir_2 = Kernel::Direction_2;
  using Line_2 = Kernel::Line_2;
  using Pnt_2 = Kernel::Point_2;
  using Ray_2 = Kernel::Ray_2;
  using Seg_2 = Kernel::Segment_2;
  using Tri_2 = Kernel::Triangle_2;
  using Vec_2 = Kernel::Vector_2;
  using Wd_pnt_2 = Kernel::Weighted_point_2;

  using Circle_3 = Kernel::Circle_3;
  using Dir_3 = Kernel::Direction_3;
  using Pln_3 = Kernel::Plane_3;
  using Pnt_3 = Kernel::Point_3;
  using Seg_3 = Kernel::Segment_3;
  using Ray_3 = Kernel::Ray_3;
  using Sfr_3 = Kernel::Sphere_3;
  using Tri_3 = Kernel::Triangle_3;
  using Vec_3 = Kernel::Vector_3;
  using Wd_pnt_3 = Kernel::Weighted_point_3;

  using Mesh_df_int = CGAL::Mesh_constant_domain_field_3<Kernel, int>;

  // Circle_2
  if (! add_attr<Circle_2>(m, "Circle_2")) {
    py::class_<Circle_2> circle2_c(m, "Circle_2", doc::Circle_2__class__);
    export_circle_2<Kernel>(circle2_c);
  }

  // Direction_2
  if (! add_attr<Dir_2>(m, "Direction_2")) {
    py::class_<Dir_2> dir2_c(m, "Direction_2", doc::Direction_2__class__);
    export_dir_2<Kernel>(dir2_c);
  }

  // Line_2
  if (! add_attr<Line_2>(m, "Line_2")) {
    py::class_<Line_2> line2_c(m, "Line_2", doc::Line_2__class__);
    export_line_2<Kernel>(line2_c);
  }

  // Point_2
  if (! add_attr<Pnt_2>(m, "Point_2")) {
    py::class_<Pnt_2> pnt2_c(m, "Point_2", doc::Point_2__class__);
    export_point_2<Kernel>(pnt2_c);
  }

  // Ray_2
  if (! add_attr<Ray_2>(m, "Ray_2")) {
    py::class_<Ray_2> ray2_c(m, "Ray_2", doc::Ray_2__class__);
    export_ray_2<Kernel>(ray2_c);
  }

  // Segment_2
  if (! add_attr<Seg_2>(m, "Segment_2")) {
    py::class_<Seg_2> seg2_c(m, "Segment_2", doc::Segment_2__class__);
    export_segment_2<Kernel>(seg2_c);
  }

  // Vector_2
  if (! add_attr<Vec_2>(m, "Vector_2")) {
    py::class_<Vec_2> vec2_c(m, "Vector_2", doc::Vector_2__class__);
    export_vector_2<Kernel>(vec2_c);
  }

  // Triangle_2
  if (! add_attr<Tri_2>(m, "Triangle_2")) {
    py::class_<Tri_2> tri2_c(m, "Triangle_2", doc::Triangle_2__class__);
    export_triangle_2<Kernel>(tri2_c);
  }

  // Iso_rectangle_2
  if (! add_attr<Iso_rectangle_2>(m, "Iso_rectangle_2")) {
    py::class_<Iso_rectangle_2>iso2_c(m, "Iso_rectangle_2", doc::Iso_rectangle_2__class__);
    export_iso_rectangle_2<Kernel>(iso2_c);
  }

  // Aff_transformation_2
  if (! add_attr<Aff_transformation_2>(m, "Aff_transformation_2")) {
    py::class_<Aff_transformation_2> aff2_c(m, "Aff_transformation_2", doc::Aff_transformation_2__class__);
    export_aff_transformation_2<Kernel>(aff2_c);
  }

  // Aff_transformation_3
  if (! add_attr<Aff_transformation_3>(m, "Aff_transformation_3")) {
    py::class_<Aff_transformation_3> aff3_c(m, "Aff_transformation_3", doc::Aff_transformation_3__class__);
    export_aff_transformation_3<Kernel>(aff3_c);
  }

  // 3D Objects

  // Circle_3
  if (! add_attr<Circle_3>(m, "Circle_3")) {
    py::class_<Circle_3> circle3_c(m, "Circle_3", doc::Circle_3__class__);
    export_circle_3<Kernel>(circle3_c);
  }

  // Direction_3
  if (! add_attr<Dir_3>(m, "Direction_3")) {
    py::class_<Dir_3> dir3_c(m, "Direction_3", doc::Direction_3__class__);
    export_dir_3<Kernel>(dir3_c);
  }

  // Point_3
  if (! add_attr<Pnt_3>(m, "Point_3")) {
    py::class_<Pnt_3> pnt3_c(m, "Point_3", doc::Point_3__class__);
    export_point_3<Kernel>(pnt3_c);
  }

  // Line_3
  if (! add_attr<Line_3>(m, "Line_3")) {
    py::class_<Line_3> line3_c(m, "Line_3", doc::Line_3__class__);
    export_line_3<Kernel>(line3_c);
  }

  // Ray_3
  if (! add_attr<Ray_3>(m, "Ray_3")) {
    py::class_<Ray_3> ray3_c(m, "Ray_3", doc::Ray_3__class__);
    export_ray_3<Kernel>(ray3_c);
  }

  // Segment_3
  if (! add_attr<Seg_3>(m, "Segment_3")) {
    py::class_<Seg_3> seg3_c(m, "Segment_3", doc::Segment_3__class__);
    export_segment_3<Kernel>(seg3_c);
  }

  // Tetrahedron_3
  if (! add_attr<Tetrahedron_3>(m, "Tetrahedron_3")) {
    py::class_<Tetrahedron_3> tet3_c(m, "Tetrahedron_3", doc::Tetrahedron_3__class__);
    export_tetrahedron_3<Kernel>(tet3_c);
  }

  // Triangle_2
  if (! add_attr<Tri_3>(m, "Triangle_3")) {
    py::class_<Tri_3> tri3_c(m, "Triangle_3", doc::Triangle_3__class__);
    export_triangle_3<Kernel>(tri3_c);
  }

  // Weighted_point_3
  if (! add_attr<Wd_pnt_3>(m, "Weighted_point_3")) {
    py::class_<Wd_pnt_3> wd_pnt3_c(m, "Weighted_point_3", doc::Weighted_point_3__class__);
    export_weighted_point_3<Kernel>(wd_pnt3_c);
  }

  // Vector_3
  if (! add_attr<Vec_3>(m, "Vector_3")) {
    py::class_<Vec_3> vec3_c(m, "Vector_3", doc::Vector_3__class__);
    export_vector_3<Kernel>(vec3_c);
  }

  // Plane_3
  if (! add_attr<Pln_3>(m, "Plane_3")) {
    py::class_<Pln_3> pln3_c(m, "Plane_3", doc::Plane_3__class__);
    export_plane_3<Kernel>(pln3_c);
  }

  // Sphere_3
  if (! add_attr<Sfr_3>(m, "Sphere_3")) {
    py::class_<Sfr_3> sfr3_c(m, "Sphere_3", doc::Sphere_3__class__);
    export_sphere_3<Kernel>(sfr3_c);
  }

  if (! add_attr<Mesh_df_int>(m, "Mesh_constant_domain_field_3_int")) {
    py::class_<Mesh_df_int> mesh_df_int_c(m, "Mesh_constant_domain_field_3_int");
    export_mesh_constant_domain_field_3<Kernel, Mesh_df_int>(mesh_df_int_c);
  }

  // Kernel Function Objects
  using Construct_vector_2 = typename Kernel::Construct_vector_2;
  using Construct_vector_3 = typename Kernel::Construct_vector_3;
  using Construct_sphere_3 = typename Kernel::Construct_sphere_3;
  using Construct_line_3 = typename Kernel::Construct_line_3;
  using Construct_circle_2 = typename Kernel::Construct_circle_2;
  using Construct_point_on_3 = typename Kernel::Construct_point_on_3;
  using Compute_x_2 = typename Kernel::Compute_x_2;
  using Compute_y_2 = typename Kernel::Compute_y_2;
  using Compute_x_3 = typename Kernel::Compute_x_3;
  using Compute_y_3 = typename Kernel::Compute_y_3;
  using Compute_z_3 = typename Kernel::Compute_z_3;
  using Compute_squared_length_2 = typename Kernel::Compute_squared_length_2;
  using Compute_squared_length_3 = typename Kernel::Compute_squared_length_3;
  using Construct_scaled_vector_2 = typename Kernel::Construct_scaled_vector_2;
  using Construct_scaled_vector_3 = typename Kernel::Construct_scaled_vector_3;
  using Construct_sum_of_vectors_2 = typename Kernel::Construct_sum_of_vectors_2;
  using Construct_sum_of_vectors_3 = typename Kernel::Construct_sum_of_vectors_3;
  using Compute_scalar_product_2 = typename Kernel::Compute_scalar_product_2;
  using Compute_scalar_product_3 = typename Kernel::Compute_scalar_product_3;
  using Construct_cross_product_vector_3 =
    typename Kernel::Construct_cross_product_vector_3;
  using Construct_center_2 = typename Kernel::Construct_center_2;
  using Construct_center_3 = typename Kernel::Construct_center_3;
  using Compute_squared_radius_2 = typename Kernel::Compute_squared_radius_2;
  using Compute_squared_radius_3 = typename Kernel::Compute_squared_radius_3;
  using Collinear_2 = typename Kernel::Collinear_2;
  using Collinear_3 = typename Kernel::Collinear_3;

  // Kernel Function Objects
  if (! add_attr<Construct_vector_2>(m, "Construct_vector_2"))
    py::class_<Construct_vector_2>(m, "Construct_vector_2")
      .def(py::init<>())
      .def("__call__",
           [](Construct_vector_2 ctr, const Pnt_2& a, const Pnt_2& b)
           { return ctr(a, b); },
           py::arg("a"), py::arg("b"),
           "Introduces the vector b-a.")
      .def("__call__",
           [](Construct_vector_2 ctr,
              const CGAL::Origin& o, const Pnt_2& b)
           { return ctr(o, b); }, py::arg("o"), py::arg("b"),
           "Introduces the vector b.")
      .def("__call__",
           [](Construct_vector_2 ctr, const Pnt_2& a,
              const CGAL::Origin& o)
           { return ctr(a, o); }, py::arg("a"), py::arg("o"),
           "Introduces the vector -a.")
      .def("__call__",
           [](Construct_vector_2 ctr, const Seg_2& s)
           { return ctr(s); }, py::arg("s"),
           "Introduces the vector s.target()-s.source().")
      .def("__call__",
           [](Construct_vector_2 ctr, const Ray_2& r)
           { return ctr(r); }, py::arg("r"),
           "Introduces a vector having the same direction as r.")
      .def("__call__",
           [](Construct_vector_2 ctr, const Line_2& l)
           { return ctr(l); }, py::arg("l"),
           "Introduces a vector having the same direction as l.")
      .def("__call__",
           [](Construct_vector_2 ctr,
              const CGAL::Null_vector& NULL_VECTOR)
           { return ctr(NULL_VECTOR); }, py::arg("NULL_VECTOR"),
           "Introduces the null vector.")
      ;

  if (! add_attr<Construct_vector_3>(m, "Construct_vector_3"))
    py::class_<Construct_vector_3>(m, "Construct_vector_3")
      .def(py::init<>())
      .def("__call__",
           [](Construct_vector_3 ctr, const Pnt_3& a, const Pnt_3& b)
           { return ctr(a, b); },
           py::arg("a"), py::arg("b"),
           "Introduces the vector b-a.")
      .def("__call__",
           [](Construct_vector_3 ctr, const CGAL::Origin& o, const Pnt_3& b)
           { return ctr(o, b); },
           py::arg("o"), py::arg("b"),
           "Introduces the vector b.")
      .def("__call__",
           [](Construct_vector_3 ctr, const Pnt_3& a, const CGAL::Origin& o)
           { return ctr(a, o); },
           py::arg("a"), py::arg("o"),
           "Introduces the vector -a.")
      .def("__call__",
           [](Construct_vector_3 ctr, const Seg_3& s) { return ctr(s); },
           py::arg("s"),
           "Introduces the vector s.target()-s.source().")
      .def("__call__",
           [](Construct_vector_3 ctr, const Ray_3& r) { return ctr(r); },
           py::arg("r"),
           "Introduces a vector having the same direction as r.")
      .def("__call__",
           [](Construct_vector_3 ctr, const Line_3& l) { return ctr(l); },
           py::arg("l"),
           "Introduces a vector having the same direction as l.")
      .def("__call__",
           [](Construct_vector_3 ctr, const CGAL::Null_vector& NULL_VECTOR)
           { return ctr(NULL_VECTOR); },
           py::arg("NULL_VECTOR"),
           "Introduces the null vector.")
      ;

  if (! add_attr<Construct_sphere_3>(m, "Construct_sphere_3"))
    py::class_<Construct_sphere_3>(m, "Construct_sphere_3")
      .def(py::init<>())
      .def("__call__",
           [](Construct_sphere_3 ctr, const Pnt_3& center,
              const FT& squared_radius, const Orientation& orientation)
           { return ctr(center, squared_radius, orientation); },
           py::arg("center"), py::arg("squared_radius"),
           py::arg("orientation") = CGAL::COUNTERCLOCKWISE,
           "Introduces a sphere initialized to the sphere with center center, squared radius squared_radius and orientation orientation.\n\n"
           "Precondition\n"
           "• orientation != CGAL::COPLANAR and squared_radius >= 0. \n\n")
      .def("__call__",
           [](Construct_sphere_3 ctr, const Pnt_3& center,
              const Orientation& orientation)
           { return ctr(center, orientation); },
           py::arg("center"), py::arg("orientation") = CGAL::COUNTERCLOCKWISE,
           "Introduces a sphere s initialized to the sphere with center center, squared radius zero and orientation orientation.\n\n"
           "Precondition\n"
           "• orientation != CGAL::COPLANAR. \n\n"
           "Postcondition\n"
           "• s.is_degenerate() = true. \n\n")
      .def("__call__",
           [](Construct_sphere_3 ctr, const Pnt_3& p, const Pnt_3& q,
              const Pnt_3& r, const Pnt_3& s) { return ctr(p, q, r, s); },
           py::arg("p"), py::arg("q"), py::arg("r"), py::arg("s"),
           "Introduces a sphere initialized to the unique sphere which passes through the points p, q, r and s.\n\n"
           "The orientation of the sphere is the orientation of the point quadruple p, q, r, s.\n\n"
           "Precondition\n"
           "• p, q, r, and s are not coplanar. \n\n")
      .def("__call__",
           [](Construct_sphere_3 ctr, const Pnt_3& p, const Pnt_3& q,
              const Pnt_3& r, const Orientation& o) { return ctr(p, q, r, o); },
           py::arg("p"), py::arg("q"), py::arg("r"),
           py::arg("o") = CGAL::COUNTERCLOCKWISE,
           "Introduces a sphere initialized to the smallest sphere which passes through the points p, q, and r.\n\n"
           "The orientation of the sphere is o.\n\n"
           "Precondition\n"
           "• o != CGAL::COPLANAR. \n\n")
      .def("__call__",
           [](Construct_sphere_3 ctr, const Pnt_3& p, const Pnt_3& q,
              const Orientation& o)
           { return ctr(p, q, o); },
           py::arg("p"), py::arg("q"), py::arg("o") = CGAL::COUNTERCLOCKWISE,
           "Introduces a sphere initialized to the smallest sphere which passes through the points p and q.\n\n"
           "The orientation of the sphere is o.\n\n"
           "Precondition\n"
           "• o != CGAL::COPLANAR. \n\n")
      ;

  if (! add_attr<Construct_line_3>(m, "Construct_line_3"))
    py::class_<Construct_line_3>(m, "Construct_line_3")
      .def(py::init<>())
      .def("__call__",
           [](Construct_line_3 ctr, const Pnt_3& p, const Pnt_3& q)
           { return ctr(p, q); },
           py::arg("p"), py::arg("q"),
           "Introduces a line passing through the points p and q.\n"
           "Line is directed from p to q.")
      .def("__call__",
           [](Construct_line_3 ctr, const Pnt_3& p, const Vec_3& v)
           { return ctr(p, v); },
           py::arg("p"), py::arg("v"),
           "Introduces a line passing through point p and oriented by v.")
      .def("__call__",
           [](Construct_line_3 ctr, const Pnt_3& p, const Dir_3& d)
           { return ctr(p, d); },
           py::arg("p"), py::arg("d"),
           "Introduces a line passing through point p with direction d.")
      .def("__call__",
           [](Construct_line_3 ctr, const Seg_3& s) { return ctr(s); },
           py::arg("s"),
           "Returns the line supporting the segment s, oriented from source to target.")
      .def("__call__",
           [](Construct_line_3 ctr, const Ray_3& r)
                     { return ctr(r); },
           py::arg("r"),
           "Returns the line supporting the ray r, with the same orientation.")
      ;

  if (! add_attr<Construct_circle_2>(m, "Construct_circle_2"))
    py::class_<Construct_circle_2>(m, "Construct_circle_2")
      .def(py::init<>())
      .def("__call__",
           [](Construct_circle_2 ctr, const Pnt_2& center,
              const FT& squared_radius, const Orientation& orientation)
           { return ctr(center, squared_radius, orientation); },
           py::arg("center"), py::arg("squared_radius"),
           py::arg("orientation") = CGAL::COUNTERCLOCKWISE,
           "Introduces a circle initialized to the circle with center center, squared radius squared_radius and orientation orientation.\n\n"
           "Precondition\n"
           "• orientation != CGAL::COLLINEAR and squared_radius >= 0.")
      .def("__call__",
           [](Construct_circle_2 ctr, const Pnt_2& center,
              const Orientation& orientation)
           { return ctr(center, orientation); },
           py::arg("center"), py::arg("orientation") = CGAL::COUNTERCLOCKWISE,
           "Introduces a circle initialized to the circle with center center, squared radius zero and orientation orientation.\n\n"
           "Precondition\n"
           "• orientation != CGAL::COLLINEAR.\n\n"
           "Postcondition\n"
           "• .is_degenerate() = true.")
      .def("__call__",
           [](Construct_circle_2 ctr, const Pnt_2& p, const Pnt_2& q,
              const Pnt_2& r)
           { return ctr(p, q, r); },
           py::arg("p"), py::arg("q"), py::arg("r"),
           "Introduces a circle initialized to the unique circle which passes through the points p, q and r.\n\n"
           "The orientation of the circle is the orientation of the point triple p, q, r.\n\n"
           "Precondition\n"
           "• p, q, and r are not collinear.")
      .def("__call__",
           [](Construct_circle_2 ctr, const Pnt_2& p, const Pnt_2& q,
              const Orientation& orientation)
           { return ctr(p, q, orientation); },
           py::arg("p"), py::arg("q"),
           py::arg("orientation") = CGAL::COUNTERCLOCKWISE,
           "Introduces a circle initialized to the circle with diameter pq and orientation orientation.\n\n"
           "Precondition\n"
           "• orientation != CGAL::COLLINEAR.")
      ;

  if (! add_attr<Construct_point_on_3>(m, "Construct_point_on_3"))
    py::class_<Construct_point_on_3>(m, "Construct_point_on_3")
      .def(py::init<>())
      .def("__call__",
           [](Construct_point_on_3 ctr, const Line_3& l, const FT& i)
           { return ctr(l, i); },
           py::arg("l"), py::arg("i"),
           "return an arbitrary point on l.\n"
           "It holds point(i) == point(j), iff i==j. Furthermore, is directed from point(i) to point(j), for all i < j.")
      .def("__call__",
           [](Construct_point_on_3 ctr, const Line_3& l) { return ctr(l); },
           py::arg("l"),
           "return point(0) on l, identical to operator()(l,0).")
      .def("__call__",
           [](Construct_point_on_3 ctr, const Plane_3& h) { return ctr(h); },
           py::arg("h"),
           "return an arbitrary point on h.")
      .def("__call__",
           [](Construct_point_on_3 ctr, const Ray_3& r, const FT& i)
           { return ctr(r, i); },
           py::arg("r"), py::arg("i"),
           "return a point on r.\n"
           "point(0) is the source, point(i), with i>0, is different from the source.\n\n"
           "Precondition\n"
           "• i >= 0.")
      .def("__call__",
           [](Construct_point_on_3 ctr, const Segment_3& s, int i)
           { return ctr(s, i); },
           py::arg("s"), py::arg("i"),
           "return source or target of s: point(0) returns the source of s, point(1) returns the target of s."
           "The parameter i is taken modulo 2, which gives easy access to the other end point.")
      ;

  if (! add_attr<Compute_x_2>(m, "Compute_x_2"))
    py::class_<Compute_x_2>(m, "Compute_x_2")
      .def(py::init<>())
      .def("__call__",
           [](Compute_x_2 ctr, const Point_2& p) { return ctr(p); },
           py::arg("p"),
           "returns the x-coordinate of the point.")
      .def("__call__",
           [](Compute_x_2 ctr, const Vector_2& v) { return ctr(v); },
           py::arg("v"),
           "returns the x-coordinate of the vector.")
      ;

  if (! add_attr<Compute_y_2>(m, "Compute_y_2"))
    py::class_<Compute_y_2>(m, "Compute_y_2")
      .def(py::init<>())
      .def("__call__",
           [](Compute_y_2 ctr, const Point_2& p) { return ctr(p); },
           py::arg("p"),
           "returns the y-coordinate of the point.")
      .def("__call__",
           [](Compute_y_2 ctr, const Vector_2& v) { return ctr(v); },
           py::arg("v"),
           "returns the y-coordinate of the vector.")
      ;

  if (! add_attr<Compute_x_3>(m, "Compute_x_3"))
    py::class_<Compute_x_3>(m, "Compute_x_3")
      .def(py::init<>())
      .def("__call__",
           [](Compute_x_3 ctr, const Point_3& p) { return ctr(p); },
           py::arg("p"),
           "returns the x-coordinate of the point.")
      .def("__call__",
           [](Compute_x_3 ctr, const Vector_3& v) { return ctr(v); },
           py::arg("v"),
           "returns the x-coordinate of the vector.")
      ;

  if (! add_attr<Compute_y_3>(m, "Compute_y_3"))
    py::class_<Compute_y_3>(m, "Compute_y_3")
      .def(py::init<>())
      .def("__call__",
           [](Compute_y_3 ctr, const Point_3& p) { return ctr(p); },
           py::arg("p"),
           "returns the y-coordinate of the point.")
      .def("__call__",
           [](Compute_y_3 ctr, const Vector_3& v) { return ctr(v); },
           py::arg("v"),
           "returns the y-coordinate of the vector.")
      ;

  if (! add_attr<Compute_z_3>(m, "Compute_z_3"))
    py::class_<Compute_z_3>(m, "Compute_z_3")
      .def(py::init<>())
      .def("__call__",
           [](Compute_z_3 ctr, const Point_3& p) { return ctr(p); },
           py::arg("p"),
           "returns the z-coordinate of the point.")
      .def("__call__",
           [](Compute_z_3 ctr, const Vector_3& v) { return ctr(v); },
           py::arg("v"),
           "returns the z-coordinate of the vector.")
      ;

  if (! add_attr<Compute_squared_length_2>(m, "Compute_squared_length_2"))
    py::class_<Compute_squared_length_2>(m, "Compute_squared_length_2")
      .def(py::init<>())
      .def("__call__",
           [](Compute_squared_length_2 ctr, const Vector_2& v)
           { return ctr(v); },
           py::arg("v"),
           "returns the squared length of v.")
      .def("__call__",
           [](Compute_squared_length_2 ctr, const Seg_2& s) { return ctr(s); },
         py::arg("p"),
           "returns the squared length of s.")
      ;

  if (! add_attr<Compute_squared_length_3>(m, "Compute_squared_length_3"))
    py::class_<Compute_squared_length_3>(m, "Compute_squared_length_3")
      .def(py::init<>())
      .def("__call__",
           [](Compute_squared_length_3 ctr, const Vector_3& v)
           { return ctr(v); },
           py::arg("v"),
           "returns the squared length of v.")
      .def("__call__",
           [](Compute_squared_length_3 ctr, const Seg_3& s) { return ctr(s); },
           py::arg("p"),
           "returns the squared length of s.")
      ;

  if (! add_attr<Construct_scaled_vector_2>(m, "Construct_scaled_vector_2"))
    py::class_<Construct_scaled_vector_2>(m, "Construct_scaled_vector_2")
      .def(py::init<>())
      .def("__call__",
           [](Construct_scaled_vector_2 ctr, const Vector_2& v, const RT& scale)
           { return ctr(v, scale); },
           py::arg("v"), py::arg("scale"),
           "produces the vector v scaled by a factor scale.")
      .def("__call__",
           [](Construct_scaled_vector_2 ctr, const Vector_2& v, const FT& scale)
           { return ctr(v, scale); },
           py::arg("v"), py::arg("scale"),
           "produces the vector v scaled by a factor scale.")
      ;

  if (! add_attr<Construct_scaled_vector_3>(m, "Construct_scaled_vector_3"))
    py::class_<Construct_scaled_vector_3>(m, "Construct_scaled_vector_3")
      .def(py::init<>())
      .def("__call__",
           [](Construct_scaled_vector_3 ctr, const Vector_3& v, const RT& scale)
           { return ctr(v, scale); },
           py::arg("v"), py::arg("scale"),
           "produces the vector v scaled by a factor scale.")
      .def("__call__",
           [](Construct_scaled_vector_3 ctr, const Vector_3& v, const FT& scale)
           { return ctr(v, scale); },
           py::arg("v"), py::arg("scale"),
           "produces the vector v scaled by a factor scale.")
      ;

  if (! add_attr<Construct_sum_of_vectors_2>(m, "Construct_sum_of_vectors_2"))
    py::class_<Construct_sum_of_vectors_2>(m, "Construct_sum_of_vectors_2")
      .def(py::init<>())
      .def("__call__",
           [](Construct_sum_of_vectors_2 ctr, const Vector_2& v1,
              const Vector_2& v2)
           { return ctr(v1, v2); },
           py::arg("v1"), py::arg("v2"),
           "introduces the vector v1 + v2.")
      ;

  if (! add_attr<Construct_sum_of_vectors_3>(m, "Construct_sum_of_vectors_3"))
    py::class_<Construct_sum_of_vectors_3>(m, "Construct_sum_of_vectors_3")
      .def(py::init<>())
      .def("__call__",
           [](Construct_sum_of_vectors_3 ctr, const Vector_3& v1,
              const Vector_3& v2)
           { return ctr(v1, v2); },
           py::arg("v1"), py::arg("v2"),
           "introduces the vector v1 + v2.")
      ;

  if (! add_attr<Compute_scalar_product_2>(m, "Compute_scalar_product_2"))
    py::class_<Compute_scalar_product_2>(m, "Compute_scalar_product_2")
      .def(py::init<>())
      .def("__call__",
           [](Compute_scalar_product_2 ctr, const Vector_2& v, const Vector_2& w)
           { return ctr(v, w); },
           py::arg("v"), py::arg("w"),
           "returns the scalar (inner) product of the two vectors v and w.")
      ;

  if (! add_attr<Compute_scalar_product_3>(m, "Compute_scalar_product_3"))
    py::class_<Compute_scalar_product_3>(m, "Compute_scalar_product_3")
      .def(py::init<>())
      .def("__call__",
           [](Compute_scalar_product_3 ctr, const Vector_3& v, const Vector_3& w)
           { return ctr(v, w); },
           py::arg("v"), py::arg("w"),
           "returns the scalar (inner) product of the two vectors v and w.")
      ;

  if (! add_attr<Construct_cross_product_vector_3>(m, "Construct_cross_product_vector_3"))
    py::class_<Construct_cross_product_vector_3>(m, "Construct_cross_product_vector_3")
      .def(py::init<>())
      .def("__call__",
           [](Construct_cross_product_vector_3 ctr, const Vector_3& v,
              const Vector_3& w)
           { return ctr(v, w); },
           py::arg("v"), py::arg("w"),
           "computes the cross product of v and w.")
      ;

  if (! add_attr<Construct_center_2>(m, "Construct_center_2"))
    py::class_<Construct_center_2>(m, "Construct_center_2")
      .def(py::init<>())
      .def("__call__",
           [](Construct_center_2 ctr, const Circle_2& c) { return ctr(c); },
           py::arg("c"),
           "compute the center of the circle c.")
      ;

  if (! add_attr<Construct_center_3>(m, "Construct_center_3"))
    py::class_<Construct_center_3>(m, "Construct_center_3")
      .def(py::init<>())
      .def("__call__",
           [](Construct_center_3 ctr, const Sphere_3& s) { return ctr(s); }, ri,
           py::arg("s"),
           "compute the center of the sphere s.")

#if CGAL_VERSION_NR > 1060100900
      .def("__call__",
           [](Construct_center_3 ctr, const Circle_3& c) { return ctr(c); }, ri,
           py::arg("c"),
           "compute the center of the circle c.")
#endif
      ;

  if (! add_attr<Compute_squared_radius_2>(m, "Compute_squared_radius_2"))
    py::class_<Compute_squared_radius_2>(m, "Compute_squared_radius_2")
      .def(py::init<>())
      .def("__call__",
           [](Compute_squared_radius_2 ctr, const Circle_2& c)
           { return ctr(c); },
           py::arg("c"),
           "returns the squared radius of c.")
      .def("__call__",
           [](Compute_squared_radius_2 ctr, const Point_2& p,
              const Point_2& q, const Point_2& r)
           { return ctr(p, q, r); },
           py::arg("p"), py::arg("q"), py::arg("r"),
           "returns the squared radius of the circle passing through p, q and r.\n\n"
           "Precondition\n"
           "• p, q, and r are not collinear.")
      .def("__call__",
           [](Compute_squared_radius_2 ctr, const Point_2& p, const Point_2& q)
           { return ctr(p, q); },
           py::arg("p"), py::arg("q"),
           "returns the squared radius of the smallest circle passing through p, and q, i.e. one fourth of the squared distance between p and q.")
      .def("__call__",
           [](Compute_squared_radius_2 ctr, const Point_2& p)
           { return ctr(p); },
           py::arg("p"),
           "returns the squared radius of the smallest circle passing through p, i.e. 0.")
      ;

  if (! add_attr<Compute_squared_radius_3>(m, "Compute_squared_radius_3"))
    py::class_<Compute_squared_radius_3>(m, "Compute_squared_radius_3")
      .def(py::init<>())
      .def("__call__",
           [](Compute_squared_radius_3 ctr, const Sphere_3& s)
           { return ctr(s); },
           py::arg("s"),
           "returns the squared radius of s.")
      .def("__call__",
           [](Compute_squared_radius_3 ctr, const Circle_3& c)
           { return ctr(c); },
           py::arg("c"),
           "returns the squared radius of c.")
      .def("__call__",
           [](Compute_squared_radius_3 ctr, const Point_3& p,
              const Point_3& q, const Point_3& r, const Point_3& s)
           { return ctr(p, q, r, s); },
           py::arg("p"), py::arg("q"), py::arg("r"), py::arg("s"),
           "returns the squared radius of the sphere passing through p, q, r and s.\n\n"
           "Precondition\n"
           "• p, q, r, and s are not coplanar.")
      .def("__call__",
           [](Compute_squared_radius_3 ctr, const Point_3& p,
              const Point_3& q, const Point_3& r)
           { return ctr(p, q, r); },
           py::arg("p"), py::arg("q"), py::arg("r"),
           "returns the squared radius of the sphere passing through p, q and r, and whose center is in the plane defined by these three points.\n")
      .def("__call__",
           [](Compute_squared_radius_3 ctr, const Point_3& p, const Point_3& q)
           { return ctr(p, q); },
           py::arg("p"), py::arg("q"),
           "returns the squared radius of the smallest circle passing through p, and q, i.e. one fourth of the squared distance between p and q.")
      .def("__call__",
           [](Compute_squared_radius_3 ctr, const Point_3& p)
           { return ctr(p); },
           py::arg("p"),
           "returns the squared radius of the smallest circle passing through p, i.e. 0.")
      ;

  if (! add_attr<Collinear_2>(m, "Collinear_2"))
    py::class_<Collinear_2>(m, "Collinear_2")
      .def(py::init<>())
      .def("__call__",
           [](Collinear_2 ctr, const Pnt_2& p, const Pnt_2& q, const Pnt_2& r)
           { return ctr(p, q, r); },
           py::arg("p"), py::arg("q"), py::arg("r"),
           "returns true, if p, q, and r are collinear.")
      ;

  if (! add_attr<Collinear_3>(m, "Collinear_3"))
    py::class_<Collinear_3>(m, "Collinear_3")
      .def(py::init<>())
      .def("__call__",
           [](Collinear_3 ctr, const Pnt_3& p, const Pnt_3& q, const Pnt_3& r)
           { return ctr(p, q, r); },
           py::arg("p"), py::arg("q"), py::arg("r"),
           "returns true, if p, q, and r are collinear.")
      ;



  /// \name Global kernel functions
  /// @{
  m.def("abs", &CGAL::abs<FT>, py::arg("value"));

  using Angle_fnc1 = CGAL::Angle(*)(const Vec_2&, const Vec_2&);
  using Angle_fnc2 = CGAL::Angle(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Angle_fnc3 = CGAL::Angle(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&,
                                    const Pnt_2&);
  using Angle_fnc4 = CGAL::Angle(*)(const Pnt_3&, const Pnt_3&, const Pnt_3&);
  using Angle_fnc5 = CGAL::Angle(*)(const Pnt_3&, const Pnt_3&, const Pnt_3&,
                                    const Pnt_3&);
  m.def("angle", static_cast<Angle_fnc1>(&CGAL::angle<Kernel>),
        py::arg("v1"), py::arg("v2"));
  m.def("angle", static_cast<Angle_fnc2>(&CGAL::angle<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"));
  m.def("angle", static_cast<Angle_fnc3>(&CGAL::angle<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"), py::arg("s"));
  m.def("angle", static_cast<Angle_fnc4>(&CGAL::angle<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"));
  m.def("angle", static_cast<Angle_fnc5>(&CGAL::angle<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"), py::arg("s"));

  using Area_fnc = FT(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("area", static_cast<Area_fnc>(&CGAL::area<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"));

  using Aoal_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("are_ordered_along_line",
        static_cast<Aoal_fnc>(&CGAL::are_ordered_along_line<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"));

  using Asoal_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("are_strictly_ordered_along_line",
        static_cast<Asoal_fnc>(&CGAL::are_strictly_ordered_along_line<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"));

  using Bc_fnc1 = Pnt_2(*)(const Pnt_2&, const FT&, const Pnt_2&, const FT&);
  using Bc_fnc2 =
    Pnt_2(*)(const Pnt_2&, const FT&, const Pnt_2&, const FT&, const Pnt_2&,
             const FT&);
  using Bc_fnc3 =
    Pnt_2(*)(const Pnt_2&, const FT&, const Pnt_2&, const FT&, const Pnt_2&,
             const FT&, const Pnt_2&, const FT&);
  m.def("barycenter", static_cast<Bc_fnc1>(&CGAL::barycenter<Kernel>),
        py::arg("point1"), py::arg("weight1"),
        py::arg("point2"), py::arg("weight2"));
  m.def("barycenter", static_cast<Bc_fnc2>(&CGAL::barycenter<Kernel>),
        py::arg("point1"), py::arg("weight1"),
        py::arg("point2"), py::arg("weight2"),
        py::arg("point3"), py::arg("weight3"));
  m.def("barycenter", static_cast<Bc_fnc3>(&CGAL::barycenter<Kernel>),
        py::arg("point1"), py::arg("weight1"),
        py::arg("point2"), py::arg("weight2"),
        py::arg("point3"), py::arg("weight3"),
        py::arg("point4"), py::arg("weight4"));

  m.def("bbox_2", &ker::bbox_2, py::arg("points"));

  using Bisector_fnc = Line_2(*)(const Pnt_2&, const Pnt_2&);
  m.def("bisector", static_cast<Bisector_fnc>(&CGAL::bisector<Kernel>),
        py::arg("p"), py::arg("q"));

  // Requires sqrt operation
  //def<Line_2(const Line_2&, const Line_2&)>("bisector", &CGAL::bisector);

  using Cr_fnc1 = Pnt_2(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Cr_fnc2 =
    Pnt_2(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Cr_fnc3 = Pnt_2(*)(const Tri_2&);
  m.def("centroid", static_cast<Cr_fnc1>(&CGAL::centroid<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"));
  m.def("centroid", static_cast<Cr_fnc2>(&CGAL::centroid<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"), py::arg("s"));
  m.def("centroid", static_cast<Cr_fnc3>(&CGAL::centroid<Kernel>),
        py::arg("triangle"));

  using Cc_fnc1 = Pnt_2(*)(const Pnt_2&, const Pnt_2&);
  using Cc_fnc2 = Pnt_2(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Cc_fnc3 = Pnt_2(*)(const Tri_2&);
  m.def("circumcenter", static_cast<Cc_fnc1>(&CGAL::circumcenter<Kernel>),
        py::arg("p"), py::arg("q"));
  m.def("circumcenter", static_cast<Cc_fnc2>(&CGAL::circumcenter<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"));
  m.def("circumcenter", static_cast<Cc_fnc3>(&CGAL::circumcenter<Kernel>),
        py::arg("triangle"));

  using Caoral_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("collinear_are_ordered_along_line",
        static_cast<Caoral_fnc>(&CGAL::collinear_are_ordered_along_line<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"));

  using Casoral_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("collinear_are_strictly_ordered_along_line",

        static_cast<Casoral_fnc>(&CGAL::collinear_are_strictly_ordered_along_line<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"));

  using Clr_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("collinear", static_cast<Clr_fnc>(&CGAL::collinear<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"));

  using Cdtp_fnc =
    CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("compare_distance_to_point",
        static_cast<Cdtp_fnc>(&CGAL::compare_distance_to_point<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"));

  using Cl_fnc = CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&);
  m.def("compare_lexicographically",
        static_cast<Cl_fnc>(&CGAL::compare_lexicographically<Kernel>),
        py::arg("p"), py::arg("q"));

#if !defined(_MSC_VER)
  // MSVC does handle the following for some reason....
  using Csdtl_fnc1 =
    CGAL::Comparison_result(*)(const Line_2&, const Pnt_2&, const Pnt_2&);
  m.def("compare_signed_distance_to_line",
        static_cast<Csdtl_fnc1>(&CGAL::compare_signed_distance_to_line<Kernel>),
        py::arg("line"), py::arg("p"), py::arg("q"));
#endif
  using Csdtl_fnc2 =
    CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&,
                               const Pnt_2&);
  m.def("compare_signed_distance_to_line",
        static_cast<Csdtl_fnc2>(&CGAL::compare_signed_distance_to_line<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"), py::arg("s"));

  using Cs_fnc1 = CGAL::Comparison_result(*)(const Line_2&, const Line_2&);
  using Cs_fnc2 =
    CGAL::Comparison_result(*)(const Segment_2&, const Segment_2&);
  m.def("compare_slope", static_cast<Cs_fnc1>(&CGAL::compare_slope<Kernel>),
        py::arg("line1"), py::arg("line2"));
  m.def("compare_slope", static_cast<Cs_fnc2>(&CGAL::compare_slope<Kernel>),
        py::arg("segment1"), py::arg("segment2"));

  using Csd_fnc =
    CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&, const FT&);
  m.def("compare_squared_distance",
        static_cast<Csd_fnc>(&CGAL::compare_squared_distance<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("squared_distance"));

  using Cx_fnc1 = CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&);
  using Cx_fnc2 =
    CGAL::Comparison_result(*)(const Pnt_2&, const Line_2&, const Line_2&);
  using Cx_fnc3 =
    CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&);
  using Cx_fnc4 =
    CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&,
                               const Line_2&);
  m.def("compare_x", static_cast<Cx_fnc1>(&CGAL::compare_x<Kernel>),
        py::arg("p"), py::arg("q"));
  m.def("compare_x", static_cast<Cx_fnc2>(&CGAL::compare_x<Kernel>),
        py::arg("point"), py::arg("line1"), py::arg("line2"));
  m.def("compare_x", static_cast<Cx_fnc3>(&CGAL::compare_x<Kernel>),
        py::arg("line1"), py::arg("line2"), py::arg("line3"));
  m.def("compare_x", static_cast<Cx_fnc4>(&CGAL::compare_x<Kernel>),
        py::arg("line1"), py::arg("line2"), py::arg("line3"), py::arg("line4"));

  using Cxy_fnc = CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&);
  m.def("compare_xy", static_cast<Cxy_fnc>(&CGAL::compare_xy<Kernel>),
        py::arg("p"), py::arg("q"));

  using Cxaty_fnc1 = CGAL::Comparison_result(*)(const Pnt_2&, const Line_2&);
  using Cxaty_fnc2 =
    CGAL::Comparison_result(*)(const Pnt_2&, const Line_2&, const Line_2&);
  using Cxaty_fnc3 =
    CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&);
  using Cxaty_fnc4 =
    CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&,
                               const Line_2&);
  m.def("compare_x_at_y",
        static_cast<Cxaty_fnc1>(&CGAL::compare_x_at_y<Kernel>));
  m.def("compare_x_at_y",
        static_cast<Cxaty_fnc2>(&CGAL::compare_x_at_y<Kernel>));
  m.def("compare_x_at_y",
        static_cast<Cxaty_fnc3>(&CGAL::compare_x_at_y<Kernel>));
  m.def("compare_x_at_y",
        static_cast<Cxaty_fnc4>(&CGAL::compare_x_at_y<Kernel>));

  using Cyatx_fnc1 = CGAL::Comparison_result(*)(const Pnt_2&, const Line_2&);
  using Cyatx_fnc2 =
    CGAL::Comparison_result(*)(const Pnt_2&, const Line_2&, const Line_2&);
  using Cyatx_fnc3 =
    CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&);
  using Cyatx_fnc4 =
    CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&,
                               const Line_2&);
  using Cyatx_fnc5 = CGAL::Comparison_result(*)(const Pnt_2&, const Segment_2&);
  using Cyatx_fnc6 =
    CGAL::Comparison_result(*)(const Pnt_2&, const Segment_2&, const Segment_2&);
  m.def("compare_y_at_x",
        static_cast<Cyatx_fnc1>(&CGAL::compare_y_at_x<Kernel>));
  m.def("compare_y_at_x",
        static_cast<Cyatx_fnc2>(&CGAL::compare_y_at_x<Kernel>));
  m.def("compare_y_at_x",
        static_cast<Cyatx_fnc3>(&CGAL::compare_y_at_x<Kernel>));
  m.def("compare_y_at_x",
        static_cast<Cyatx_fnc4>(&CGAL::compare_y_at_x<Kernel>));
  m.def("compare_y_at_x",
        static_cast<Cyatx_fnc5>(&CGAL::compare_y_at_x<Kernel>));
  m.def("compare_y_at_x",
        static_cast<Cyatx_fnc6>(&CGAL::compare_y_at_x<Kernel>));

  using Cy_fnc1 = CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&);
  using Cy_fnc2 =
    CGAL::Comparison_result(*)(const Pnt_2&, const Line_2&, const Line_2&);
  using Cy_fnc3 =
    CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&);
  using Cy_fnc4 =
    CGAL::Comparison_result(*)(const Line_2&, const Line_2&, const Line_2&,
                               const Line_2&);
  m.def("compare_y", static_cast<Cy_fnc1>(&CGAL::compare_y<Kernel>));
  m.def("compare_y", static_cast<Cy_fnc2>(&CGAL::compare_y<Kernel>));
  m.def("compare_y", static_cast<Cy_fnc3>(&CGAL::compare_y<Kernel>));
  m.def("compare_y", static_cast<Cy_fnc4>(&CGAL::compare_y<Kernel>));

  using Cyx_fnc = CGAL::Comparison_result(*)(const Pnt_2&, const Pnt_2&);
  m.def("compare_yx", static_cast<Cyx_fnc>(&CGAL::compare_yx<Kernel>));

  using Cp_fnc3 = Vec_3(*)(const Vec_3&, const Vec_3&);
  m.def("cross_product", static_cast<Cp_fnc3>(&CGAL::cross_product<Kernel>));

  using Dt_fnc = FT(*)(const Vec_2&, const Vec_2&);
  m.def("determinant", static_cast<Dt_fnc>(&CGAL::determinant<Kernel>));

  using Hldtp_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("has_larger_distace_to_point",
        static_cast<Hldtp_fnc>(&CGAL::has_larger_distance_to_point<Kernel>));

#if !defined(_MSC_VER)
  // MSVC does handle the following for some reason....
  using Hldtl_fnc1 = bool(*)(const Line_2&, const Pnt_2&, const Pnt_2&);
  m.def("has_larger_signed_distance_to_line",
        static_cast<Hldtl_fnc1>(&CGAL::has_larger_signed_distance_to_line<Kernel>));
#endif
  using Hldtl_fnc2 =
    bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("has_larger_signed_distance_to_line",
        static_cast<Hldtl_fnc2>(&CGAL::has_larger_signed_distance_to_line<Kernel>));

  using Hsdtp_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("has_smaller_distace_to_point",
        static_cast<Hsdtp_fnc>(&CGAL::has_smaller_distance_to_point<Kernel>));

#if !defined(_MSC_VER)
  // MSVC does handle the following for some reason....
  using Hssdtl_fnc1 = bool(*)(const Line_2&, const Pnt_2&, const Pnt_2&);
  m.def("has_smaller_signed_distance_to_line",
        static_cast<Hssdtl_fnc1>(&CGAL::has_smaller_signed_distance_to_line<Kernel>));
#endif

  using Hssdtl_fnc2 = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("has_smaller_signed_distance_to_line",
        static_cast<Hssdtl_fnc2>(&CGAL::has_smaller_signed_distance_to_line<Kernel>));

  using Lid_fnc = FT(*)(const Pnt_2&, const Pnt_2&);
  m.def("l_infinity_distance",
        static_cast<Lid_fnc>(&CGAL::l_infinity_distance<Kernel>));

  using Lt_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("left_turn", static_cast<Lt_fnc>(&CGAL::left_turn<Kernel>));

  using Lxyl_fnc = bool(*)(const Pnt_2&, const Pnt_2&);
  m.def("lexicographically_xy_larger",
        static_cast<Lxyl_fnc>(&CGAL::lexicographically_xy_larger<Kernel>));

  using Lxyloe_fnc = bool(*)(const Pnt_2&, const Pnt_2&);
  m.def("lexicographically_xy_larger_or_equal",
        static_cast<Lxyloe_fnc>(&CGAL::lexicographically_xy_larger_or_equal<Kernel>));

  using Lxys_fnc = bool(*)(const Pnt_2&, const Pnt_2&);
  m.def("lexicographically_xy_smaller",
        static_cast<Lxys_fnc>(&CGAL::lexicographically_xy_smaller<Kernel>));

  using Lxysoe_fnc = bool(*)(const Pnt_2&, const Pnt_2&);
  m.def("lexicographically_xy_smaller_or_equal",
        static_cast<Lxysoe_fnc>(&CGAL::lexicographically_xy_smaller_or_equal<Kernel>));

  using Maxv_fnc = Pnt_2(*)(const Iso_rectangle_2&);
  m.def("max_vertex", static_cast<Maxv_fnc>(&CGAL::max_vertex<Kernel>));

  using Mp_fnc = Pnt_2(*)(const Pnt_2&, const Pnt_2&);
  m.def("midpoint", static_cast<Mp_fnc>(&CGAL::midpoint<Kernel>));

  using Mp3_fnc = Pnt_3(*)(const Pnt_3&, const Pnt_3&);
  m.def("midpoint", static_cast<Mp3_fnc>(&CGAL::midpoint<Kernel>));

  using Minv_fnc = Pnt_2(*)(const Iso_rectangle_2&);
  m.def("min_vertex", static_cast<Minv_fnc>(&CGAL::min_vertex<Kernel>));

  using Or_fnc1 =
    CGAL::Orientation(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Or_fnc2 = CGAL::Orientation(*)(const Vec_2&, const Vec_2&);
  m.def("orientation", static_cast<Or_fnc1>(&CGAL::orientation));
  m.def("orientation", static_cast<Or_fnc2>(&CGAL::orientation));

  using Pl_fnc1 = bool(*)(const Line_2&, const Line_2&);
  using Pl_fnc2 = bool(*)(const Ray_2&, const Ray_2&);
  using Pl_fnc3 = bool(*)(const Segment_2&, const Segment_2&);
  m.def("parallel", static_cast<Pl_fnc1>(&CGAL::parallel<Line_2>));
  m.def("parallel", static_cast<Pl_fnc2>(&CGAL::parallel<Ray_2>));
  m.def("parallel", static_cast<Pl_fnc3>(&CGAL::parallel<Segment_2>));

  using Rl_fnc = Line_2 (*)(const Circle_2&, const Circle_2&);
  m.def("radical_line", static_cast<Rl_fnc>(&CGAL::radical_line<Kernel>));

  // Parameterized by FT (not Kernel)
  using Rra_fnc =
    void(*)(const RT&, const RT&, RT&, RT&, RT&, const RT&, const RT&);
  m.def("rational_rotation_approximation",
        static_cast<Rra_fnc>(&CGAL::rational_rotation_approximation<FT>));

  using Rt_fnc = bool(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("right_turn", static_cast<Rt_fnc>(&CGAL::right_turn<Kernel>));

  using Sp_fnc2 = FT(*)(const Vec_2&, const Vec_2&);
  using Sp_fnc3 = FT(*)(const Vec_3&, const Vec_3&);
  m.def("scalar_product", static_cast<Sp_fnc2>(&CGAL::scalar_product<Kernel>),
        py::arg("v1"), py::arg("v2"));
  m.def("scalar_product", static_cast<Sp_fnc3>(&CGAL::scalar_product<Kernel>),
        py::arg("v1"), py::arg("v2"));

  using Sobc_fnc1 =
    CGAL::Bounded_side(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&,
                          const Pnt_2&);
  using Sobc_fnc2 =
    CGAL::Bounded_side(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  m.def("side_of_bounded_circle",
        static_cast<Sobc_fnc1>(&CGAL::side_of_bounded_circle<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"), py::arg("s"));
  m.def("side_of_bounded_circle",
        static_cast<Sobc_fnc2>(&CGAL::side_of_bounded_circle<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"));

  using Sooc_fnc =
    CGAL::Oriented_side(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&,
                           const Pnt_2&);
  m.def("side_of_oriented_circle",
        static_cast<Sooc_fnc>(&CGAL::side_of_oriented_circle<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"), py::arg("s"));

  // squared_distance() bindings
  CGALPY::Type_list<Pnt_2, Line_2, Ray_2, Seg_2, Tri_2, Wd_pnt_2> type_list_2;
  CGALPY::Type_list<Pnt_3, Line_3, Ray_3, Seg_3, Tri_3, Pln_3, Wd_pnt_3>
    type_list_3;
  CGALPY::cartesian_product<Sd_wrapper>(m, type_list_2, type_list_2);
  CGALPY::cartesian_product<Sd_wrapper>(m, type_list_3, type_list_3);

  //
  using Sd_fnc1 = FT(*)(const Pnt_2&, const Pnt_2&, const Pnt_2&);
  using Sd_fnc2 = FT(*)(const Pnt_2&, const Pnt_2&);
  using Sd_fnc3 = FT(*)(const Pnt_2&);
  m.def("squared_radius", static_cast<Sd_fnc1>(&CGAL::squared_radius<Kernel>),
        py::arg("p"), py::arg("q"), py::arg("r"));
  m.def("squared_radius", static_cast<Sd_fnc2>(&CGAL::squared_radius<Kernel>),
        py::arg("p"), py::arg("q"));
  m.def("squared_radius", static_cast<Sd_fnc3>(&CGAL::squared_radius<Kernel>),
        py::arg("p"));

  using Xe_fnc = bool(*)(const Pnt_2&, const Pnt_2&);
  m.def("x_equal", static_cast<Xe_fnc>(&CGAL::x_equal<Kernel>),
        py::arg("p"), py::arg("q"));

  using Ye_fnc =bool(*)(const Pnt_2&, const Pnt_2&) ;
  m.def("y_equal", static_cast<Ye_fnc>(&CGAL::y_equal<Kernel>),
        py::arg("p"), py::arg("q"));

  using Do_fnc = bool(*)(const Bbox_2&, const Bbox_2&);
  m.def("do_overlap", static_cast<Do_fnc>(&CGAL::do_overlap),
        py::arg("bbox1"), py::arg("bbox2"));

  using Do_fnc3 = bool(*)(const Bbox_3&, const Bbox_3&);
  m.def("do_overlap", static_cast<Do_fnc3>(&CGAL::do_overlap),
        py::arg("bbox1"), py::arg("bbox2"));

  using Cmp3_fnc = CGAL::Comparison_result(*)(const Pnt_3&, const Pnt_3&);
  m.def("compare_z", static_cast<Cmp3_fnc>(&CGAL::compare_z<Kernel>),
        py::arg("p"), py::arg("q"));

  //! From number_utils.h. \todo move to algebraic foundations
  using Cmp_fnc = CGAL::Comparison_result(*)(const FT&, const FT&);
  m.def("compare", static_cast<Cmp_fnc>(&CGAL::compare<FT>),
        py::arg("x"), py::arg("y"));

  using Sign_fnc = CGAL::Comparison_result(*)(const FT&);
  m.def("sign", static_cast<Sign_fnc>(&CGAL::sign<FT>),
        py::arg("value"));

  using Square_res = CGAL::Algebraic_structure_traits<FT>::Square::result_type;
  using Square_fnc = Square_res(*)(const FT&);
  m.def("square", static_cast<Square_fnc>(&CGAL::square<FT>),
        py::arg("value"));

  m.def("to_double", &CGAL::to_double<FT>, py::arg("value"));

  m.def("approximate_dihedral_angle", &CGAL::approximate_dihedral_angle<Kernel>,
        py::arg("p"), py::arg("q"), py::arg("r"), py::arg("s"));
  /// @}

  using Pnt_range = typename std::vector<Point_3>;
  using Png_range = typename std::vector<std::vector<std::size_t>>;

  m.def("read_polygon_soup",
        [](const std::string& fname, Pnt_range& points, Png_range& polygons,
           const py::dict& np = py::dict()) {
          return CGAL::IO::read_polygon_soup(fname, points, polygons,
                                             internal::parse_named_parameters(np));
        },
        py::arg("fname"), py::arg("points"), py::arg("polygons"),
        py::arg("np") = py::dict(),
        "reads a polygon soup from a file.\n"
        "Supported file formats are the following:\n"
        "\n"
        "Object File Format (OFF) (.off)\n"
        "Wavefront Advanced Visualizer Object Format (OBJ) (.obj)\n"
        "STereoLithography (STL) File Format (.stl)\n"
        "Polygon File Format (PLY) (.ply)\n"
        "GOCAD (TS) File Format (.ts)\n"
        "VTK (VTU / VTP / legacy) File Formats (.vtp)\n"
        "\n"
        "The format is detected from the filename extension (letter case is not important).\n"
        "\n"
        "Parameters\n"
        "fname\tthe name of the file.\n"
        "points\tpoints of the soup of polygons\n"
        "polygons\teach element in the range describes a polygon using the indices of the vertices.\n"
        "Optional Named Parameters\n"
        "verbose\tindicates whether output warnings and error messages should be printed or not.\n"
        "\tType: Boolean\n"
        "\tDefault: false\n"
        "\n"
        "Returns\n"
        "true if reading was successful, false otherwise. \n")
    ;

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) &&                                \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) &&                      \
     (CGALPY_KERNEL != CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  m.def("write_polygon_soup",
        [](const std::string& fname, const Pnt_range& points,
           const Png_range& polygons, const py::dict& np = py::dict()) {
          return CGAL::IO::write_polygon_soup(fname, points, polygons,
                                              internal::parse_named_parameters(np));
        },
        py::arg("fname"), py::arg("points"), py::arg("polygons"),
        py::arg("np") = py::dict(),
        "writes the content of points and polygons in a file.\n"
        "Supported file formats are the following:\n"
        "\n"
        "Object File Format (OFF) (.off)\n"
        "Wavefront Advanced Visualizer Object Format (OBJ) (.obj)\n"
        "STereoLithography (STL) File Format (.stl)\n"
        "Polygon File Format (PLY) (.ply)\n"
        "GOCAD (TS) File Format (.ts)\n"
        "VTK (VTU / VTP / legacy) File Formats (.vtp)\n"
        "\n"
        "The format is detected from the filename extension (letter case is not important).\n"
        "\n"
        "Parameters\n"
        "fname\tthe name of the file.\n"
        "points\tpoints of the soup of polygons\n"
        "polygons\teach element in the range describes a polygon using the indices of the vertices.\n"
        "Optional Named Parameters\n"
        "verbose\tindicates whether output warnings and error messages should be printed or not.\n"
        "\tType: Boolean\n"
        "\tDefault: false\n"
        "\n"
        "Returns\n"
        "true if writing was successful, false otherwise. \n")
    ;
#else
  m.def("rational_rotation_approximation",
        [](const RT& dirx, const RT& diry, const RT& eps_num, const RT& eps_den)
        ->py::list {
          RT sin_num;
          RT cos_num;
          RT denom;
          CGAL::rational_rotation_approximation(dirx, diry,
                                                sin_num, cos_num, denom,
                                                eps_num, eps_den);
          py::list res;
          res.append(py::cast(sin_num));
          res.append(py::cast(cos_num));
          res.append(py::cast(denom));
          return res;
        },
        py::arg("dirx"), py::arg("diry"), py::arg("eps_num"), py::arg("eps_den"),
        "computes an approximates a given direction, such that its sine and cosine are rational numbers, and the difference between the sine and the rational approximation is bounded by a given epsilon.\n"
        "Parameters\n"
        "dirx\tthe x-coordinate of the direction.\n"
        "diry\tthe y-coordinate of the direction.\n"
        "eps_num\tthe numerator of approximation bound.\n"
        "eps_den\tthe denominator of approximation bound.\n"
        "Returns\n"
        "a list of three ring-type numbers, the numerators of the sine and cosine of the computed angle approximation and their denominator.\n");

#endif
}
