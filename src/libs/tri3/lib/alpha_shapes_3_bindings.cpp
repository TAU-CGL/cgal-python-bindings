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

// Exact comparison
#if CGALPY_EXACT_COMPARISON == 0
typedef CGAL::Tag_false         Exact_comparison;
#elif CGALPY_EXACT_COMPARISON == 1
typedef CGAL::Tag_true          Exact_comparison;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_EXACT_COMPARISON");
#endif

// 3D triangulation vertex base
#if CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE
typedef CGAL::Triangulation_vertex_base_3<Kernel>               Vb0;
typedef CGAL::Alpha_shape_vertex_base_3<Kernel, Vb0, Exact_comparison> Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_WITH_INFO
typedef CGAL::Triangulation_vertex_base_with_info_3<size_t, Kernel> Vb0;
typedef CGAL::Alpha_shape_vertex_base_3<Kernel, Vb0, Exact_comparison> Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR
typedef CGAL::Regular_triangulation_vertex_base_3<Kernel>       Vb0;
typedef CGAL::Alpha_shape_vertex_base_3<Kernel, Vb0, Exact_comparison> Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO
typedef CGAL::Regular_triangulation_vertex_base_3<Kernel>       Vb0;
typedef CGAL::Triangulation_vertex_base_with_info_3<size_t, Kernel, Vb0> Vb1;
typedef CGAL::Fixed_alpha_shape_vertex_base_3<Kernel, Vb1>             Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE
typedef CGAL::Triangulation_vertex_base_3<Kernel>               Vb0;
typedef CGAL::Alpha_shape_vertex_base_3<Kernel, Vb0, Exact_comparison> Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_WITH_INFO
typedef CGAL::Triangulation_vertex_base_with_info_3<size_t, Kernel> Vb0;
typedef CGAL::Fixed_alpha_shape_vertex_base_3<Kernel, Vb0>             Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR
typedef CGAL::Regular_triangulation_vertex_base_3<Kernel>       Vb0;
typedef CGAL::Fixed_alpha_shape_vertex_base_3<Kernel, Vb0>             Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO
typedef CGAL::Regular_triangulation_vertex_base_3<Kernel>       Vb0;
typedef CGAL::Triangulation_vertex_base_with_info_3<size_t, Kernel, Vb0> Vb1;
typedef CGAL::Fixed_alpha_shape_vertex_base_3<Kernel, Vb1>             Vertex_base;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_VERTEX_BASE");
#endif

// 3D triangulation cell base
#if CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE
typedef CGAL::Triangulation_cell_base_3<Kernel>                 Cb0;
typedef CGAL::Alpha_shape_cell_base_3<Kernel, Cb0, Exact_comparison>   Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_WITH_INFO
typedef CGAL::Triangulation_cell_base_with_info_3<size_t, Kernel> Cb0;
typedef CGAL::Alpha_shape_cell_base_3<Kernel, Cb0, Exact_comparison>   Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR
typedef CGAL::Regular_triangulation_cell_base_3<Kernel>         Cb0;
typedef CGAL::Alpha_shape_cell_base_3<Kernel, Cb0, Exact_comparison>   Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO
typedef CGAL::Regular_triangulation_cell_base_3<Kernel>         Cb0;
typedef CGAL::Triangulation_cell_base_with_info_3<size_t, Kernel, Cb0> Cb1;
typedef CGAL::Fixed_alpha_shape_cell_base_3<Kernel, Cb1>               Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE
typedef CGAL::Triangulation_cell_base_3<Kernel>                 Cb0;
typedef CGAL::Alpha_shape_cell_base_3<Kernel, Cb0, Exact_comparison>   Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_WITH_INFO
typedef CGAL::Triangulation_cell_base_with_info_3<size_t, Kernel> Cb0;
typedef CGAL::Fixed_alpha_shape_cell_base_3<Kernel, Cb0>               Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR
typedef CGAL::Regular_triangulation_cell_base_3<Kernel>         Cb0;
typedef CGAL::Fixed_alpha_shape_cell_base_3<Kernel, Cb0>               Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO
typedef CGAL::Regular_triangulation_cell_base_3<Kernel>         Cb0;
typedef CGAL::Triangulation_cell_base_with_info_3<size_t, Kernel, Cb0> Cb1;
typedef CGAL::Fixed_alpha_shape_cell_base_3<Kernel, Cb1>               Cell_base;
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

typedef Triangulation_3::Point          Tri3_point;
typedef Triangulation_3::Vertex         Tri3_vertex;
typedef Triangulation_3::Cell           Tri3_cell;

typedef Triangulation_3::Vertex_handle  Tri3_vertex_handle;
typedef Triangulation_3::Cell_handle    Tri3_cell_handle;

// Alpha shape type
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
typedef CGAL::Alpha_shape_3<Triangulation_3, Exact_comparison>     Alpha_shape_3;
#elif CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_FIXED
typedef CGAL::Fixed_alpha_shape_3<Triangulation_3>                 Alpha_shape_3;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_ALPHA_SHAPE");
#endif

typedef Alpha_shape_3::Point                    As_point;

typedef Alpha_shape_3::Cell_handle              As_cell_handle;
typedef Alpha_shape_3::Vertex_handle            As_vertex_handle;
typedef Alpha_shape_3::Facet                    As_facet;
typedef Alpha_shape_3::Edge                     As_edge;

typedef Alpha_shape_3::Cell_circulator          As_cell_circulator;
typedef Alpha_shape_3::Facet_circulator         As_facet_circulator;

typedef Alpha_shape_3::Cell_iterator            As_cell_iterator;
typedef Alpha_shape_3::Facet_iterator           As_facet_iterator;
typedef Alpha_shape_3::Edge_iterator            As_edge_iterator;
typedef Alpha_shape_3::Vertex_iterator          As_vertex_iterator;

typedef Alpha_shape_3::Finite_cells_iterator    As_finite_cells_iterator;
typedef Alpha_shape_3::Finite_facets_iterator   As_finite_facets_iterator;
typedef Alpha_shape_3::Finite_edges_iterator    As_finite_edges_iterator;
typedef Alpha_shape_3::Finite_vertices_iterator As_finite_vertices_iterator;

typedef Alpha_shape_3::size_type                As_size_type;
typedef Alpha_shape_3::Locate_type              As_locate_type;
typedef Alpha_shape_3::Weighted_tag             As_weighted_tag;

typedef Alpha_shape_3::NT                       As_nt;

#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
typedef Alpha_shape_3::Mode                     As_mode;
typedef Alpha_shape_3::Classification_type      As_classification_type;
typedef Alpha_shape_3::Alpha_iterator           As_alpha_iterator;
typedef CGAL::Alpha_status<As_nt>               As_alpha_status;
#endif

#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
void make_alpha_shape(Alpha_shape_3& as, boost::python::list& lst)
{
  if (! lst) return;
  if (! extract<As_point>(lst[0]).check()) return;
  auto begin = boost::python::stl_input_iterator<As_point>(lst);
  auto end = boost::python::stl_input_iterator<As_point>();
  // auto v = std::vector<As_point>(begin, end);
  // as.make_alpha_shape(v.begin(), v.end());
  as.make_alpha_shape(begin, end);
}
#endif

Alpha_shape_3* init1(boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<As_point>(lst);
  auto end = boost::python::stl_input_iterator<As_point>();
  return new Alpha_shape_3(begin, end);
}

Alpha_shape_3* init2(boost::python::list& lst, const As_nt& alpha)
{
  auto begin = boost::python::stl_input_iterator<As_point>(lst);
  auto end = boost::python::stl_input_iterator<As_point>();
  return new Alpha_shape_3(begin, end, alpha);
}

#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
Alpha_shape_3* init3(boost::python::list& lst, const As_nt& alpha, As_mode m)
{
  auto begin = boost::python::stl_input_iterator<As_point>(lst);
  auto end = boost::python::stl_input_iterator<As_point>();
  return new Alpha_shape_3(begin, end, alpha, m);
}

const As_nt& next(As_alpha_iterator it)
{
  if (it == As_alpha_iterator()) {
    PyErr_SetString(PyExc_StopIteration, "Invalid alpha iterator");
    bp::throw_error_already_set();
  }
  return *it++;
}

#endif

template <typename Handle_>
const typename Handle_::value_type& value(Handle_ handle) { return *handle; }

void export_triangulation_3()
{
  using namespace boost::python;

  class_<Tri3_vertex_handle>("Vertex_handle")
    .def(init<>())
    .def("value", &value<Tri3_vertex_handle>, return_value_policy<reference_existing_object>())
    ;

  class_<Tri3_cell_handle>("Cell_handle")
    .def(init<>())
    .def("value", &value<Tri3_cell_handle>, return_value_policy<reference_existing_object>())
    ;

  class_<Tri3_vertex>("Vertex")
    .def(init<>())
    // Access Functions
    .def<Tri3_cell_handle(Tri3_vertex::*)()const>("cell", &Tri3_vertex::cell)
    .def<const Tri3_point&(Tri3_vertex::*)() const>("point", &Tri3_vertex::point, return_internal_reference<>())
    // Setting
    .def("set_cell", &Tri3_vertex::set_cell)
    .def("set_point", &Tri3_vertex::set_point)
    // Checking
    .def("is_valid", &Tri3_vertex::is_valid)
    ;

  void(Tri3_cell::*set_vertices)(Tri3_vertex_handle, Tri3_vertex_handle, Tri3_vertex_handle, Tri3_vertex_handle) =
    &Tri3_cell::set_vertices;
  void(Tri3_cell::*set_neighbors)(Tri3_cell_handle, Tri3_cell_handle, Tri3_cell_handle, Tri3_cell_handle) =
    &Tri3_cell::set_neighbors;

  class_<Tri3_cell>("Cell")
    .def(init<>())
    // Access Functions
    .def("vertex", &Tri3_cell::vertex)
    .def<int(Tri3_cell::*)(Tri3_vertex_handle) const>("index", &Tri3_cell::index)
    .def<int(Tri3_cell::*)(Tri3_cell_handle) const>("index", &Tri3_cell::index)
    .def<bool(Tri3_cell::*)(Tri3_vertex_handle) const>("has_vertex", &Tri3_cell::has_vertex)
    .def<bool(Tri3_cell::*)(Tri3_vertex_handle, int&) const>("has_vertex", &Tri3_cell::has_vertex)
    .def("neighbor", &Tri3_cell::neighbor)
    .def<bool(Tri3_cell::*)(Tri3_cell_handle n) const>("has_neighbor", &Tri3_cell::has_neighbor)
    .def<bool(Tri3_cell::*)(Tri3_cell_handle n, int &i) const>("has_neighbor", &Tri3_cell::has_neighbor)
    // Setting
    .def("set_vertex", &Tri3_cell::set_vertex)
    .def("set_vertices", set_vertices)
    .def("set_neighbor", &Tri3_cell::set_neighbor)
    .def("set_neighbors", set_neighbors)
    //  Checking
    .def("is_valid", &Tri3_cell::is_valid)
    ;
}

void export_alpha_shapes_3()
{
  using namespace boost::python;

#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
  enum_<As_mode>("Mode")
    .value("GENERAL", Alpha_shape_3::GENERAL)
    .value("REGULARIZED", Alpha_shape_3::REGULARIZED)
    .export_values()
    ;

  enum_<As_classification_type>("Classification_type")
    .value("EXTERIOR", Alpha_shape_3::EXTERIOR)
    .value("SINGULAR", Alpha_shape_3::SINGULAR)
    .value("REGULAR", Alpha_shape_3::REGULAR)
    .value("INTERIOR", Alpha_shape_3::INTERIOR)
    .export_values()
    ;

  class_<As_alpha_status>("Alpha_status")
    .def(init<>())
    // Modifiers
    .def("set_is_Gabriel", &As_alpha_status::set_is_Gabriel)
    .def("set_is_on_chull", &As_alpha_status::set_is_on_chull)
    .def("set_alpha_min", &As_alpha_status::set_alpha_min)
    .def("set_alpha_mid", &As_alpha_status::set_alpha_mid)
    .def("set_alpha_max", &As_alpha_status::set_alpha_max)
    // Access Functions
    .def("is_Gabriel", &As_alpha_status::is_Gabriel)
    .def("is_on_chull", &As_alpha_status::is_on_chull)
    .def("alpha_min", &As_alpha_status::alpha_min)
    .def("alpha_mid", &As_alpha_status::alpha_mid)
    .def("alpha_max", &As_alpha_status::alpha_max)
    ;

  class_<As_alpha_iterator>("Alpha_iterator")
    .def("__iter__", &pass_through)
    .def("__next__", &next, return_value_policy<copy_const_reference>())
    ;

  typedef Alpha_shape_3                           As_3;
  As_size_type (As_3::*number_of_solid_components1)() const                   = &As_3::number_of_solid_components;
  As_size_type (As_3::*number_of_solid_components2)(const As_nt& alpha) const = &As_3::number_of_solid_components;

  As_classification_type (As_3::*classify1)(const As_point& p, const As_nt& alpha) const              = &As_3::classify;
  As_classification_type (As_3::*classify2)(const As_edge& s, const As_nt& alpha) const               = &As_3::classify;
  As_classification_type (As_3::*classify3)(const As_facet& s, const As_nt& alpha) const              = &As_3::classify;
  As_classification_type (As_3::*classify4)(const As_vertex_handle& s, const As_nt& alpha) const      = &As_3::classify;
  As_classification_type (As_3::*classify5)(const As_cell_handle& s, const As_nt& alpha) const        = &As_3::classify;
  As_classification_type (As_3::*classify6)(const As_cell_handle& s, int i, const As_nt& alpha) const = &As_3::classify;

  As_classification_type (As_3:: *classify7)(const As_point& p) const              = &As_3::classify;
  As_classification_type (As_3:: *classify8)(const As_edge& s) const               = &As_3::classify;
  As_classification_type (As_3:: *classify9)(const As_facet& s) const              = &As_3::classify;
  As_classification_type (As_3::*classify10)(const As_vertex_handle& s) const      = &As_3::classify;
  As_classification_type (As_3::*classify11)(const As_cell_handle& s) const        = &As_3::classify;
  As_classification_type (As_3::*classify12)(const As_cell_handle& s, int i) const = &As_3::classify;

  As_alpha_status (As_3::*get_alpha_status1)(const As_edge& e) const  = &As_3::get_alpha_status;
  As_alpha_status (As_3::*get_alpha_status2)(const As_facet& f) const = &As_3::get_alpha_status;

#endif

  class_<Alpha_shape_3, boost::noncopyable>("Alpha_shape_3")
    .def(init<>())
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
    .def(init<optional<double, Alpha_shape_3::Mode>>())
    .def(init<optional<As_nt&, Alpha_shape_3::Mode>>())
    .def(init<Triangulation_3&, optional<double, Alpha_shape_3::Mode>>())
    .def(init<Triangulation_3&, optional<As_nt&, Alpha_shape_3::Mode>>())
#endif
    .def("__init__", make_constructor(&init1))
    .def("__init__", make_constructor(&init2))
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
    .def("__init__", make_constructor(&init3))
#endif
    // Modifiers
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
    .def("make_alpha_shape", &make_alpha_shape)
    .def("set_mode", &Alpha_shape_3::set_mode)
    .def("set_alpha", &Alpha_shape_3::set_alpha)
#endif
    .def("clear", &Alpha_shape_3::clear)
    // Query Functions
    .def("get_alpha", &Alpha_shape_3::get_alpha, return_value_policy<copy_const_reference>())
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
    .def("get_mode", &Alpha_shape_3::get_mode)
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
#endif
    // .def("get_alpha_shape_cells", &Alpha_shape_3::get_alpha_shape_cells)
    // .def("get_alpha_shape_facets", &Alpha_shape_3::get_alpha_shape_facets)
    // .def("get_alpha_shape_edges", &Alpha_shape_3::get_alpha_shape_edges)
    // .def("get_alpha_shape_vertices", &Alpha_shape_3::get_alpha_shape_vertices)
    // .def("filtration", &Alpha_shape_3::filtration)
    // .def("filtration_with_alpha_values", &Alpha_shape_3::filtration_with_alpha_values)
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
    // Traversal of the alpha-Values
    .def("alpha_begin", &Alpha_shape_3::alpha_begin)
    .def("alpha_end", &Alpha_shape_3::alpha_end)
    .def("alphas", range(&Alpha_shape_3::alpha_begin, &Alpha_shape_3::alpha_end))
    .def("alpha_find", &Alpha_shape_3::alpha_find)
    .def("alpha_lower_bound", &Alpha_shape_3::alpha_lower_bound)
    .def("alpha_upper_bound", &Alpha_shape_3::alpha_upper_bound)
    // // Operations
    .def("number_of_solid_components", number_of_solid_components1)
    .def("number_of_solid_components", number_of_solid_components2)
    .def("find_optimal_alpha", &Alpha_shape_3::find_optimal_alpha)
    .def("find_alpha_solid", &Alpha_shape_3::find_alpha_solid)
#endif
    ;
}
