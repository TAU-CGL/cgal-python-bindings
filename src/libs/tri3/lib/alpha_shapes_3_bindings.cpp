// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <boost/static_assert.hpp>

#include "CGALPY/config.hpp"
#include "CGALPY/common.hpp"
#include "CGALPY/python_iterator_templates.hpp"

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

#define CGALPY_TRI3_TRAITS_SEQUENTIAL                   0
#define CGALPY_TRI3_TRAITS_PARALLEL                     1

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

// 3D triangulation concurrency
#if CGALPY_TRI3_CONCURRENCY == CGALPY_TRI3_TRAITS_SEQUENTIAL
typedef CGAL::Sequential_tag                                       Concurrency_tag;
#elif CGALPY_TRI3_CONCURRENCY == CGALPY_TRI3_TRAITS_PARALLEL
typedef CGAL::Parallel_tag                                         Concurrency_tag;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_CONCURRENCY");
#endif

typedef CGAL::Triangulation_data_structure_3<Vertex_base, Cell_base, Concurrency_tag> Tds;

// 3D triangulation traits
#if CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_KERNEL
typedef Kernel                                                     Tri_traits;
#elif CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_PERIODIC3_DELAUNAY
typedef CGAL::Periodic_3_Delaunay_triangulation_traits_3<Kernel>   Tri_traits;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_TRAITS");
#endif

// 3D triangulation
#if CGALPY_TRI3 == CGALPY_TRI3_PLAIN
typedef CGAL::Triangulation_3<Tri_traits, Tds>                     Triangulation_3;
#elif CGALPY_TRI3 == CGALPY_TRI3_REGULAR
typedef CGAL::Regular_triangulation_3<Tri_traits, Tds>             Triangulation_3;
typedef Triangulation_3::Weighted_point                            Weighted_point;
typedef Triangulation_3::Bare_point                                Bare_point;
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

typedef Alpha_shape_3::size_type                size_type;
typedef Alpha_shape_3::NT                       NT;
typedef Alpha_shape_3::Alpha_iterator           Alpha_iterator;
typedef Alpha_shape_3::Mode                     Mode;
typedef Alpha_shape_3::Classification_type      Classification_type;

typedef Alpha_shape_3::Cell_handle              Cell_handle;
typedef Alpha_shape_3::Vertex_handle            Vertex_handle;
typedef Alpha_shape_3::Facet                    Facet;
typedef Alpha_shape_3::Edge                     Edge;

typedef CGAL::Alpha_status<FT>                  Alpha_status;

void make_alpha_shape(Alpha_shape_3& as, boost::python::list& lst)
{
  if (! lst) return;
  if (! extract<Point_2>(lst[0]).check()) return;
  auto begin = boost::python::stl_input_iterator<Point_3>(lst);
  auto end = boost::python::stl_input_iterator<Point_3>();
  // auto v = std::vector<Point_3>(begin, end);
  // as.make_alpha_shape(v.begin(), v.end());
  as.make_alpha_shape(begin, end);
}

Alpha_shape_3* init1(boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<Point_3>(lst);
  auto end = boost::python::stl_input_iterator<Point_3>();
  return new Alpha_shape_3(begin, end);
}

Alpha_shape_3* init2(boost::python::list& lst, const FT& alpha)
{
  auto begin = boost::python::stl_input_iterator<Point_3>(lst);
  auto end = boost::python::stl_input_iterator<Point_3>();
  return new Alpha_shape_3(begin, end, alpha);
}

Alpha_shape_3* init3(boost::python::list& lst, const FT& alpha, Alpha_shape_3::Mode m)
{
  auto begin = boost::python::stl_input_iterator<Point_3>(lst);
  auto end = boost::python::stl_input_iterator<Point_3>();
  return new Alpha_shape_3(begin, end, alpha, m);
}

typedef Alpha_shape_3                           As_3;
size_type (As_3::*number_of_solid_components1)() const                = &As_3::number_of_solid_components;
size_type (As_3::*number_of_solid_components2)(const NT& alpha) const = &As_3::number_of_solid_components;

Classification_type (As_3::*classify1)(const Point_3& p, const FT& alpha) const            = &As_3::classify;
Classification_type (As_3::*classify2)(const Edge& s, const FT& alpha) const               = &As_3::classify;
Classification_type (As_3::*classify3)(const Facet& s, const FT& alpha) const              = &As_3::classify;
Classification_type (As_3::*classify4)(const Vertex_handle& s, const FT& alpha) const      = &As_3::classify;
Classification_type (As_3::*classify5)(const Cell_handle& s, const FT& alpha) const        = &As_3::classify;
Classification_type (As_3::*classify6)(const Cell_handle& s, int i, const FT& alpha) const = &As_3::classify;

Classification_type (As_3:: *classify7)(const Point_3& p) const            = &As_3::classify;
Classification_type (As_3:: *classify8)(const Edge& s) const               = &As_3::classify;
Classification_type (As_3:: *classify9)(const Facet& s) const              = &As_3::classify;
Classification_type (As_3::*classify10)(const Vertex_handle& s) const      = &As_3::classify;
Classification_type (As_3::*classify11)(const Cell_handle& s) const        = &As_3::classify;
Classification_type (As_3::*classify12)(const Cell_handle& s, int i) const = &As_3::classify;

Alpha_status (As_3::*get_alpha_status1)(const Edge& e) const  = &As_3::get_alpha_status;
Alpha_status (As_3::*get_alpha_status2)(const Facet& f) const = &As_3::get_alpha_status;

const FT& next(Alpha_iterator it)
{
  if (it == Alpha_iterator()) {
    PyErr_SetString(PyExc_StopIteration, "Invalid alpha iterator");
    bp::throw_error_already_set();
  }
  return *it++;
}

void export_alpha_shapes_3()
{
  using namespace boost::python;

  enum_<Mode>("Mode")
    .value("GENERAL", Alpha_shape_3::GENERAL)
    .value("REGULARIZED", Alpha_shape_3::REGULARIZED)
    .export_values()
    ;

  enum_<Classification_type>("Classification_type")
    .value("EXTERIOR", Alpha_shape_3::EXTERIOR)
    .value("SINGULAR", Alpha_shape_3::SINGULAR)
    .value("REGULAR", Alpha_shape_3::REGULAR)
    .value("INTERIOR", Alpha_shape_3::INTERIOR)
    .export_values()
    ;

  class_<Alpha_status>("Alpha_status")
    .def(init<>())
    // Modifiers
    .def("set_is_Gabriel", &Alpha_status::set_is_Gabriel)
    .def("set_is_on_chull", &Alpha_status::set_is_on_chull)
    .def("set_alpha_min", &Alpha_status::set_alpha_min)
    .def("set_alpha_mid", &Alpha_status::set_alpha_mid)
    .def("set_alpha_max", &Alpha_status::set_alpha_max)
    // Access Functions
    .def("is_Gabriel", &Alpha_status::is_Gabriel)
    .def("is_on_chull", &Alpha_status::is_on_chull)
    .def("alpha_min", &Alpha_status::alpha_min)
    .def("alpha_mid", &Alpha_status::alpha_mid)
    .def("alpha_max", &Alpha_status::alpha_max)
    ;

  class_<Alpha_iterator>("Alpha_iterator")
    .def("__iter__", &pass_through)
    .def("__next__", &next, return_value_policy<copy_const_reference>())
    ;

  class_<Alpha_shape_3, boost::noncopyable>("Alpha_shape_3")
    .def(init<>())
    .def(init<optional<double, Alpha_shape_3::Mode>>())
    .def(init<optional<FT&, Alpha_shape_3::Mode>>())
    .def(init<Triangulation_3&, optional<double, Alpha_shape_3::Mode>>())
    .def(init<Triangulation_3&, optional<FT&, Alpha_shape_3::Mode>>())
    .def("__init__", make_constructor(&init1))
    .def("__init__", make_constructor(&init2))
    .def("__init__", make_constructor(&init3))
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
    .def("classify", classify1)
    .def("classify", classify2)
    .def("classify", classify3)
    .def("classify", classify4)
    .def("classify", classify5)
    .def("classify", classify6)
    .def("classify", classify7)
    .def("classify", classify8)
    .def("classify", classify9)
    .def("classify", classify10)
    .def("classify", classify11)
    .def("classify", classify12)
    .def("get_alpha_status", get_alpha_status1)
    .def("get_alpha_status", get_alpha_status2)
    // .def("get_alpha_shape_cells", &Alpha_shape_3::get_alpha_shape_cells)
    // .def("get_alpha_shape_facets", &Alpha_shape_3::get_alpha_shape_facets)
    // .def("get_alpha_shape_edges", &Alpha_shape_3::get_alpha_shape_edges)
    // .def("get_alpha_shape_vertices", &Alpha_shape_3::get_alpha_shape_vertices)
    // .def("filtration", &Alpha_shape_3::filtration)
    // .def("filtration_with_alpha_values", &Alpha_shape_3::filtration_with_alpha_values)
    // Traversal of the alpha-Values
    .def("alpha_begin", &Alpha_shape_3::alpha_begin)
    .def("alpha_end", &Alpha_shape_3::alpha_end)
    .def("alpha_find", &Alpha_shape_3::alpha_find)
    .def("alpha_lower_bound", &Alpha_shape_3::alpha_lower_bound)
    .def("alpha_upper_bound", &Alpha_shape_3::alpha_upper_bound)
    // // Operations
    .def("number_of_solid_components", number_of_solid_components1)
    .def("number_of_solid_components", number_of_solid_components2)
    .def("find_optimal_alpha", &Alpha_shape_3::find_optimal_alpha)
    .def("find_alpha_solid", &Alpha_shape_3::find_alpha_solid)
    ;
}
