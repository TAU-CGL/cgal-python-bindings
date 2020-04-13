// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <boost/static_assert.hpp>

#include "CGALPY/config.hpp"
#ifdef CGALPY_ALPHA_SHAPES_3_BINDINGS
#include "CGALPY/common.hpp"

// Config:
#define CGALPY_TRI3_VERTEX_BASE_PLAIN                                 0
#define CGALPY_TRI3_VERTEX_BASE_PLAIN_WITH_INFO                       1
#define CGALPY_TRI3_VERTEX_BASE_REGULAR                               2
#define CGALPY_TRI3_VERTEX_BASE_REGULAR_WITH_INFO                     3
#define CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE                           4
#define CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_WITH_INFO                 5
#define CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR                   6
#define CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO         7
#define CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE                     8
#define CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_WITH_INFO           9
#define CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR             10
#define CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO   11

#define CGALPY_TRI3_CELL_BASE_PLAIN                                   0
#define CGALPY_TRI3_CELL_BASE_PLAIN_WITH_INFO                         1
#define CGALPY_TRI3_CELL_BASE_REGULAR                                 2
#define CGALPY_TRI3_CELL_BASE_REGULAR_WITH_INFO                       3
#define CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE                             4
#define CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_WITH_INFO                   5
#define CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR                     6
#define CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO           7
#define CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE                       8
#define CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_WITH_INFO             9
#define CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR               10
#define CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO     11

#define CGALPY_TRI3_TRAITS_KERNEL                       0
#define CGALPY_TRI3_TRAITS_PERIODIC3_DELAUNAY           1

#define CGALPY_TRI3_PLAIN                               0
#define CGALPY_TRI3_REGULAR                             1
#define CGALPY_TRI3_DELAUNAY                            2
#define CGALPY_TRI3_PERIODIC3_DELAUNAY                  3

#define CGALPY_ALPHA_SHAPE_EXACT_COMPARISON_TRUE        0
#define CGALPY_ALPHA_SHAPE_EXACT_COMPARISON_FALSE       1

#define CGALPY_ALPHA_SHAPE_PLAIN                        0
#define CGALPY_ALPHA_SHAPE_FIXED                        1

// Alpha shape type
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
#include <CGAL/Alpha_shape_3.h>
#elif CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_FIXED
#include <CGAL/Fixed_alpha_shape_3.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_ALPHA_SHAPE");
#endif

// 3D triangulation vertex-base type
#if ((CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE) || \
     (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE) || \
     (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR) || \
     (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR))
// Nothing to include
#elif ((CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_WITH_INFO) || \
       (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_WITH_INFO) || \
       (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO) || \
       (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO))
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_VERTEX_BASE");
#endif

#if ((CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE) || \
     (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_WITH_INFO) || \
     (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR) || \
     (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO))
#include <CGAL/Alpha_shape_vertex_base_3.h>
#elif ((CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE) || \
       (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_WITH_INFO) || \
       (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR) || \
       (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO))
#include <CGAL/Fixed_alpha_shape_vertex_base_3.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_VERTEX_BASE");
#endif

// 3D triangulation cell-base type
#if ((CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE) || \
     (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE) || \
     (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR) || \
     (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR))
// Nothing to include
#elif ((CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_WITH_INFO) || \
       (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_WITH_INFO) || \
       (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO) || \
       (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO))
#include <CGAL/Triangulation_cell_base_with_info_3.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_CELL_BASE");
#endif

#if ((CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE) ||    \
     (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_WITH_INFO) || \
     (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR) || \
     (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO))
#include <CGAL/Alpha_shape_cell_base_3.h>
#elif ((CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE) || \
       (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_WITH_INFO) || \
       (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR) || \
       (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO))
#include <CGAL/Fixed_alpha_shape_cell_base_3.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_CELL_BASE");
#endif

// 3D triangulation traits
#if CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_KERNEL
// Nothing to include
#elif CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_PERIODIC3_DELAUNAY
#include <CGAL/Periodic_3_Delaunay_triangulation_traits_3.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_TRAITS");
#endif

// 3D Triangulation
#if CGALPY_TRI3 == CGALPY_TRI3_PLAIN
#include <CGAL/Triangulation_3.h>
#elif CGALPY_TRI3 == CGALPY_TRI3_REGULAR
#include <CGAL/Regular_triangulation_3.h>
#elif CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY
#include <CGAL/Delaunay_triangulation_3.h>
#elif CGALPY_TRI3 == CGALPY_TRI3_PERIODIC3_DELAUNAY
#include <CGAL/eriodic_3_Delaunay_triangulation_3.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3");
#endif

// Type definitions:

// 3D triangulation vertex base
#if CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE
typedef CGAL::Alpha_shape_vertex_base_3<Kernel>                 Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_WITH_INFO
typedef CGAL::Triangulation_vertex_base_with_info_3<size_t, Kernel> Vb0;
typedef CGAL::Alpha_shape_vertex_base_3<Kernel, Vb0>            Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR
typedef CGAL::Regular_triangulation_vertex_base_3<Kernel>       Vb0;
typedef CGAL::Alpha_shape_vertex_base_3<Kernel, Vb0>            Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO
typedef CGAL::Regular_triangulation_vertex_base_3<Kernel>       Vb0;
typedef CGAL::Triangulation_vertex_base_with_info_3<size_t, Kernel, Vb0> Vb1;
typedef CGAL::Fixed_alpha_shape_vertex_base_3<Kernel, Vb1>      Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE
typedef CGAL::Alpha_shape_vertex_base_3<Kernel>                 Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_WITH_INFO
typedef CGAL::Triangulation_vertex_base_with_info_3<size_t, Kernel> Vb0;
typedef CGAL::Fixed_alpha_shape_vertex_base_3<Kernel, Vb0>      Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR
typedef CGAL::Regular_triangulation_vertex_base_3<Kernel>       Vb0;
typedef CGAL::Fixed_alpha_shape_vertex_base_3<Kernel, Vb0>      Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO
typedef CGAL::Regular_triangulation_vertex_base_3<Kernel>       Vb0;
typedef CGAL::Triangulation_vertex_base_with_info_3<size_t, Kernel, Vb0> Vb1;
typedef CGAL::Fixed_alpha_shape_vertex_base_3<Kernel, Vb1>      Vertex_base;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_VERTEX_BASE");
#endif

// 3D triangulation cell base
#if CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE
typedef CGAL::Alpha_shape_cell_base_3<Kernel>                   Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_WITH_INFO
typedef CGAL::Triangulation_cell_base_with_info_3<size_t, Kernel> Cb0;
typedef CGAL::Alpha_shape_cell_base_3<Kernel, Cb0>              Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR
typedef CGAL::Regular_triangulation_cell_base_3<Kernel>         Cb0;
typedef CGAL::Alpha_shape_cell_base_3<Kernel, Cb0>              Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO
typedef CGAL::Regular_triangulation_cell_base_3<Kernel>         Cb0;
typedef CGAL::Triangulation_cell_base_with_info_3<size_t, Kernel, Cb0> Cb1;
typedef CGAL::Fixed_alpha_shape_cell_base_3<Kernel, Cb1>        Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE
typedef CGAL::Alpha_shape_cell_base_3<Kernel>                   Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_WITH_INFO
typedef CGAL::Triangulation_cell_base_with_info_3<size_t, Kernel> Cb0;
typedef CGAL::Fixed_alpha_shape_cell_base_3<Kernel, Cb0>        Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR
typedef CGAL::Regular_triangulation_cell_base_3<Kernel>         Cb0;
typedef CGAL::Fixed_alpha_shape_cell_base_3<Kernel, Cb0>        Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO
typedef CGAL::Regular_triangulation_cell_base_3<Kernel>         Cb0;
typedef CGAL::Triangulation_cell_base_with_info_3<size_t, Kernel, Cb0> Cb1;
typedef CGAL::Fixed_alpha_shape_cell_base_3<Kernel, Cb1>        Cell_base;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_CELL_BASE");
#endif

typedef CGAL::Triangulation_data_structure_3<Vertex_base, Cell_base> Tds;

// 3D triangulation traits
#if CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_KERNEL
typedef Kernel                                                   Tri_traits;
#elif CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_PERIODIC3_DELAUNAY
typedef CGAL::Periodic_3_Delaunay_triangulation_traits_3<Kernel> Tri_traits;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_TRAITS");
#endif

// 3D triangulation
#if CGALPY_TRI3 == CGALPY_TRI3_PLAIN
typedef CGAL::Triangulation_3<Tri_traits, Tds>                     Triangulation_3;
#elif CGALPY_TRI3 == CGALPY_TRI3_REGULAR
typedef CGAL::Regular_triangulation_3<Tri_traits, Tds>             Triangulation_3;
#elif CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY
typedef CGAL::Delaunay_triangulation_3<Tri_traits, Tds>            Triangulation_3;
#elif CGALPY_TRI3 == CGALPY_TRI3_PERIODIC3_DELAUNAY
typedef CGAL::Periodic_3_Delaunay_triangulation_3<Tri_traits, Tds> Triangulation_3;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3");
#endif

// Alpha shape type
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
typedef CGAL::Alpha_shape_3<Triangulation_3>            Alpha_shape_3;
#elif CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_FIXED
typedef CGAL::Fixed_alpha_shape_3<Triangulation_3>      Alpha_shape_3;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_ALPHA_SHAPE");
#endif

void make_alpha_shape(Alpha_shape_3& as, boost::python::list& lst)
{
  if (! lst) return;
  if (! extract<Point_2>(lst[0]).check()) return;
  auto begin = boost::python::stl_input_iterator<Point_3>(lst);
  auto end = boost::python::stl_input_iterator<Point_3>();
  auto v = std::vector<Point_3>(begin, end);
  as.make_alpha_shape(v.begin(), v.end());
}

void export_alpha_shapes_3()
{
  std::list<Point_3> lp;
  Alpha_shape_3 as(lp.begin(),lp.end());

  enum_<Alpha_shape_3::Mode>("Mode")
    .value("GENERAL", Alpha_shape_3::GENERAL)
    .value("REGULARIZED", Alpha_shape_3::REGULARIZED)
    .export_values()
    ;

  using namespace boost::python;
  class_<Alpha_shape_3, boost::noncopyable>("Alpha_shape_3")
    .def(init<>())
    .def(init<optional<double, Alpha_shape_3::Mode>>())
    .def(init<optional<FT&, Alpha_shape_3::Mode>>())
    .def(init<Triangulation_3&, optional<double, Alpha_shape_3::Mode>>())
    .def(init<Triangulation_3&, optional<FT&, Alpha_shape_3::Mode>>())
    // .def(init<begin, end, <optional<double, Alpha_shape_3::Mode>>())
    // .def(init<begin, end, optional<FT&, Alpha_shape_3::Mode>>())
    // Modifiers
    .def("make_alpha_shape", &make_alpha_shape)
    .def("clear", &Alpha_shape_3::clear)
    .def("set_alpha", &Alpha_shape_3::set_alpha)
    .def("set_mode", &Alpha_shape_3::set_mode)
    // Query Functions
    .def("get_mode", &Alpha_shape_3::get_mode)
    .def("get_alpha", &Alpha_shape_3::get_alpha, return_value_policy<copy_const_reference>())
    .def("get_nth_alpha", &Alpha_shape_3::get_nth_alpha, return_value_policy<copy_const_reference>())
    .def("number_of_alphas", &Alpha_shape_3::number_of_alphas)
    // .def("classify", &Alpha_shape_3::classify)
    // .def("classify", &Alpha_shape_3::classify)
    // .def("classify", &Alpha_shape_3::classify)
    // .def("classify", &Alpha_shape_3::classify)
    // .def("classify", &Alpha_shape_3::classify)
    // .def("classify", &Alpha_shape_3::classify)
    // .def("get_alpha_status", &Alpha_shape_3::get_alpha_status)
    // .def("get_alpha_status", &Alpha_shape_3::get_alpha_status)
    // .def("get_alpha_shape_cells", &Alpha_shape_3::get_alpha_shape_cells)
    // .def("get_alpha_shape_facets", &Alpha_shape_3::get_alpha_shape_facets)
    // .def("get_alpha_shape_edges", &Alpha_shape_3::get_alpha_shape_edges)
    // .def("get_alpha_shape_vertices", &Alpha_shape_3::get_alpha_shape_vertices)
    // .def("filtration", &Alpha_shape_3::filtration)
    // .def("filtration_with_alpha_values", &Alpha_shape_3::filtration_with_alpha_values)
    // // Traversal of the alpha-Values
    // .def("alpha_begin", &Alpha_shape_3::alpha_begin)
    // .def("alpha_end", &Alpha_shape_3::alpha_end)
    // .def("alpha_find", &Alpha_shape_3::alpha_find)
    // .def("alpha_lower_bound", &Alpha_shape_3::alpha_lower_bound)
    // .def("alpha_upper_bound", &Alpha_shape_3::alpha_upper_bound)
    // // Operations
    // .def("number_of_solid_components", &Alpha_shape_3::number_of_solid_components)
    // .def("find_optimal_alpha", &Alpha_shape_3::find_optimal_alpha)
    ;
}

#endif
