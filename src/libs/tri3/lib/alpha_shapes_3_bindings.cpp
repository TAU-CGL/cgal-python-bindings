// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <boost/static_assert.hpp>

#include <CGAL/iterator.h>

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

#define CGALPY_TRI3_LOCATION_POLICY_FAST                0
#define CGALPY_TRI3_LOCATION_POLICY_COMPACT             1

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
typedef Kernel                                                     Tri3_traits;
#elif CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_PERIODIC3_DELAUNAY
typedef CGAL::Periodic_3_Delaunay_triangulation_traits_3<Kernel>   Tri3_traits;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_TRAITS");
#endif

// 3D triangulation location policy
#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY
#if CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_FAST
typedef CGAL::Fast_location                                        Location_policy;
#elif CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_COMPACT
typedef CGAL::Compact_location                                     Location_policy;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_CONCURRENCY");
#endif
#endif

// 3D triangulation
#if CGALPY_TRI3 == CGALPY_TRI3_PLAIN
typedef CGAL::Triangulation_3<Tri3_traits, Tds>                           Triangulation_3;
#elif CGALPY_TRI3 == CGALPY_TRI3_REGULAR
typedef CGAL::Regular_triangulation_3<Tri3_traits, Tds>                   Triangulation_3;
typedef Triangulation_3::Weighted_point                                   Tri3_weighted_point;
typedef Triangulation_3::Bare_point                                       Tri3_bare_point;
#elif CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY
typedef CGAL::Delaunay_triangulation_3<Tri3_traits, Tds, Location_policy> Delaunay_triangulation_3;
typedef Delaunay_triangulation_3                                          Triangulation_3;
#elif CGALPY_TRI3 == CGALPY_TRI3_PERIODIC3_DELAUNAY
typedef CGAL::Periodic_3_Delaunay_triangulation_3<Tri3_traits, Tds>       Triangulation_3;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3");
#endif

typedef Triangulation_3::Point          Tri3_point;
typedef Triangulation_3::Vertex         Tri3_vertex;
typedef Triangulation_3::Cell           Tri3_cell;
typedef Triangulation_3::Facet          Tri3_facet;
typedef Triangulation_3::Edge           Tri3_edge;

typedef Triangulation_3::Vertex_handle  Tri3_vertex_handle;
typedef Triangulation_3::Cell_handle    Tri3_cell_handle;

typedef Triangulation_3::Locate_type    Tri3_locate_type;

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

typedef Alpha_shape_3::Classification_type      As_classification_type;

#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
typedef Alpha_shape_3::Mode                     As_mode;
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

Alpha_shape_3* as_init1(boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<As_point>(lst);
  auto end = boost::python::stl_input_iterator<As_point>();
  return new Alpha_shape_3(begin, end);
}

Alpha_shape_3* as_init2(boost::python::list& lst, const As_nt& alpha)
{
  auto begin = boost::python::stl_input_iterator<As_point>(lst);
  auto end = boost::python::stl_input_iterator<As_point>();
  return new Alpha_shape_3(begin, end, alpha);
}

#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
Alpha_shape_3* as_init3(boost::python::list& lst, const As_nt& alpha, As_mode m)
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

#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY

Delaunay_triangulation_3* dt3_init(boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<Tri3_point>(lst);
  auto end = boost::python::stl_input_iterator<Tri3_point>();
  return new Delaunay_triangulation_3(begin, end);
}

std::ptrdiff_t insert_points(Delaunay_triangulation_3& dt, boost::python::list& lst)
{
  if (! lst) return 0;
  if (! extract<Tri3_point>(lst[0]).check()) return 0;
  auto begin = boost::python::stl_input_iterator<Tri3_point>(lst);
  auto end = boost::python::stl_input_iterator<Tri3_point>();
  return dt.insert(begin, end);
}

#if ((CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY) && \
     (CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_COMPACT))

Tri3_vertex_handle insert4(Delaunay_triangulation_3& dt, const Tri3_point& p, Tri3_cell_handle start)
{ return dt.insert(p, start); }

Tri3_vertex_handle insert5(Delaunay_triangulation_3& dt, const Tri3_point& p, Tri3_vertex_handle hint)
{ return dt.insert(p, hint); }

Tri3_vertex_handle insert6(Delaunay_triangulation_3& dt, const Tri3_point& p, Tri3_locate_type lt, Tri3_cell_handle c, int li, int lj)
{ return dt.insert(p, lt, c, li, lj); }

#endif

void export_delaunay_triangulation_3()
{
  using namespace boost::python;

  CGAL::Bounded_side(Delaunay_triangulation_3::*side_of_sphere)(Tri3_cell_handle, const Tri3_point&, bool) const =
    &Delaunay_triangulation_3::side_of_sphere;
  CGAL::Bounded_side(Delaunay_triangulation_3::*side_of_circle1)(const Tri3_facet&, const Tri3_point&, bool) const =
    &Delaunay_triangulation_3::side_of_circle;
  CGAL::Bounded_side(Delaunay_triangulation_3::*side_of_circle2)(Tri3_cell_handle, int, const Tri3_point& p, bool) const =
    &Delaunay_triangulation_3::side_of_circle;

#if CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_COMPACT
  Tri3_vertex_handle(Delaunay_triangulation_3::*insert1)(const Tri3_point&, Tri3_cell_handle, bool*) =
    &Delaunay_triangulation_3::insert;
  Tri3_vertex_handle(Delaunay_triangulation_3::*insert2)(const Tri3_point&, Tri3_vertex_handle, bool*) =
    &Delaunay_triangulation_3::insert;
  Tri3_vertex_handle(Delaunay_triangulation_3::*insert3)(const Tri3_point&, Tri3_locate_type, Tri3_cell_handle, int, int, bool*) =
    &Delaunay_triangulation_3::insert;
#else
  Tri3_vertex_handle(Delaunay_triangulation_3::*insert1)(const Tri3_point&, Tri3_cell_handle) =
    &Delaunay_triangulation_3::insert;
  Tri3_vertex_handle(Delaunay_triangulation_3::*insert2)(const Tri3_point&, Tri3_vertex_handle) =
    &Delaunay_triangulation_3::insert;
  Tri3_vertex_handle(Delaunay_triangulation_3::*insert3)(const Tri3_point&, Tri3_locate_type, Tri3_cell_handle, int, int) =
    &Delaunay_triangulation_3::insert;
#endif

  Tri3_vertex_handle(Delaunay_triangulation_3::*nearest_vertex)(const Tri3_point&, Tri3_cell_handle) const =
    &Delaunay_triangulation_3::nearest_vertex;

  class_<Delaunay_triangulation_3>("Delaunay_triangulation_3")
    .def(init<>())
    .def(init<const Tri3_traits&>())
    .def("__init__", make_constructor(&dt3_init))
    // Insertion
    .def("insert", insert1)
    .def("insert", insert2)
    .def("insert", insert3)
#if CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_COMPACT
    .def("insert", &insert4)
    .def("insert", &insert5)
    .def("insert", &insert6)
#endif
    .def("insert_points", &insert_points)

    // template<class PointWithInfoInputIterator >
    // std::ptrdiff_t 	insert (PointWithInfoInputIterator first, PointWithInfoInputIterator last)

    // Displacement
    .def("move_if_no_collision", &Delaunay_triangulation_3::move_if_no_collision)
    .def("move", &Delaunay_triangulation_3::move)

    // Removal
    .def<void(Delaunay_triangulation_3::*)(Tri3_vertex_handle)>("remove", &Delaunay_triangulation_3::remove)
    // .def<bool(Delaunay_triangulation_3::*)(Tri3_vertex_handle, bool*)>("remove", &Delaunay_triangulation_3::remove)

    // template<typename InputIterator >
    // int remove (InputIterator first, InputIterator beyond)

    // template<typename InputIterator >
    // int remove_cluster (InputIterator first, InputIterator beyond)

    // Queries
    .def("side_of_sphere", side_of_sphere)
    .def("side_of_circle", side_of_circle1)
    .def("side_of_circle", side_of_circle2)
    .def("nearest_vertex", nearest_vertex)
    .def("nearest_vertex_in_cell", &Delaunay_triangulation_3::nearest_vertex_in_cell)
    ;
}

#endif

template <typename Handle_>
const typename Handle_::value_type& value(Handle_ handle) { return *handle; }

bool vertex_is_valid1(const Tri3_vertex& vertex, bool verbose, int level) { return vertex.is_valid(verbose, level); }
bool vertex_is_valid2(const Tri3_vertex& vertex, bool verbose) { return vertex.is_valid(verbose); }
bool vertex_is_valid3(const Tri3_vertex& vertex) { return vertex.is_valid(); }

bool cell_is_valid1(const Tri3_cell& cell, bool verbose, int level) { return cell.is_valid(verbose, level); }
bool cell_is_valid2(const Tri3_cell& cell, bool verbose) { return cell.is_valid(verbose); }
bool cell_is_valid3(const Tri3_cell& cell) { return cell.is_valid(); }

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
    .def("is_valid", &vertex_is_valid1)
    .def("is_valid", &vertex_is_valid2)
    .def("is_valid", &vertex_is_valid3)
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
    .def("is_valid", &cell_is_valid1)
    .def("is_valid", &cell_is_valid2)
    .def("is_valid", &cell_is_valid3)
    ;

  class_<Tri3_facet>("Facet")
    .def_readwrite("first", &Tri3_facet::first)
    .def_readwrite("second", &Tri3_facet::second)
    ;

  class_<Tri3_edge>("Edge")
    .def_readwrite("first", &Tri3_edge::first)
    .def_readwrite("second", &Tri3_edge::second)
    .def_readwrite("third", &Tri3_edge::third)
    ;

#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY
  export_delaunay_triangulation_3();
#endif
}

template <typename AlphaShape_3>
class Alpha_shape_3_test {
private:
  typedef AlphaShape_3                                          Alpha_shape_3;
  typedef typename Alpha_shape_3::Classification_type           Classification_type;
  typedef typename Alpha_shape_3::NT                            NT;
  typedef typename Alpha_shape_3::Finite_cells_iterator         Finite_cells_iterator;
  typedef typename Alpha_shape_3::Finite_facets_iterator        Finite_facets_iterator;
  typedef typename Alpha_shape_3::Finite_edges_iterator         Finite_edges_iterator;
  typedef typename Alpha_shape_3::Finite_vertices_iterator      Finite_vertices_iterator;

  const Alpha_shape_3& m_alpha_shape;
  Classification_type m_type;
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
  const NT& m_alpha;
#endif

public:
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
  Alpha_shape_3_test(const Alpha_shape_3& as, Classification_type type, const NT& alpha) :
    m_alpha_shape(as),
    m_type(type),
    m_alpha(alpha)
  {}
#else
  Alpha_shape_3_test(const Alpha_shape_3& as, Classification_type type) :
    m_alpha_shape(as),
    m_type(type)
  {}
#endif

  bool operator()(Finite_cells_iterator cit) const
  {
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
    return m_alpha_shape.classify(cit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(cit) == m_type;
#endif
  }

  bool operator()(Finite_facets_iterator fit) const
  {
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
    return m_alpha_shape.classify(*fit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(*fit) == m_type;
#endif
  }

  bool operator()(Finite_edges_iterator eit) const
  {
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
    return m_alpha_shape.classify(*eit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(*eit) == m_type;
#endif
  }

  bool operator()(Finite_vertices_iterator vit) const
  {
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
    return m_alpha_shape.classify(vit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(vit) == m_type;
#endif
  }
};

typedef Alpha_shape_3_test<Alpha_shape_3>                               As_test;
typedef CGAL::Filter_iterator<As_finite_cells_iterator, As_test>        As_filter_cell_iterator;
typedef CGAL::Filter_iterator<As_finite_facets_iterator, As_test>       As_filter_facet_iterator;
typedef CGAL::Filter_iterator<As_finite_edges_iterator, As_test>        As_filter_edge_iterator;
typedef CGAL::Filter_iterator<As_finite_vertices_iterator, As_test>     As_filter_vertex_iterator;


boost::python::list alpha_shape_cells(const Alpha_shape_3& as, As_classification_type type
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
                                      , const As_nt& alpha
#endif
                                      )
{
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
  As_test test_as_cell(as, type, alpha);
#else
  As_test test_as_cell(as, type);
#endif
  As_filter_cell_iterator first(as.finite_cells_end(), test_as_cell, as.finite_cells_begin());
  As_filter_cell_iterator last(as.finite_cells_end(), test_as_cell, as.finite_cells_end());
  // return boost::python::range<return_internal_reference<>, Alpha_shape_3>(&Alpha_shape_3::finite_cells_begin,
  //                                                                         &Alpha_shape_3::finite_cells_end);
  // return boost::python::range<return_internal_reference<>>(first, last);
  boost::python::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

boost::python::list alpha_shape_facets(const Alpha_shape_3& as, As_classification_type type
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
                                       , const As_nt& alpha
#endif
                                       )
{
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
  As_test test_as_facet(as, type, alpha);
#else
  As_test test_as_facet(as, type);
#endif
  As_filter_facet_iterator first(as.finite_facets_end(), test_as_facet, as.finite_facets_begin());
  As_filter_facet_iterator last(as.finite_facets_end(), test_as_facet, as.finite_facets_end());
  boost::python::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

boost::python::list alpha_shape_edges(const Alpha_shape_3& as, As_classification_type type
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
                                      , const As_nt& alpha
#endif
                                      )
{
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
  As_test test_as_edge(as, type, alpha);
#else
  As_test test_as_edge(as, type);
#endif
  As_filter_edge_iterator first(as.finite_edges_end(), test_as_edge, as.finite_edges_begin());
  As_filter_edge_iterator last(as.finite_edges_end(), test_as_edge, as.finite_edges_end());
  boost::python::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

boost::python::list alpha_shape_vertices(const Alpha_shape_3& as, As_classification_type type
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
                                         , const As_nt& alpha
#endif
                                         )
{
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
  As_test test_as_vertex(as, type, alpha);
#else
  As_test test_as_vertex(as, type);
#endif
  As_filter_vertex_iterator first(as.finite_vertices_end(), test_as_vertex, as.finite_vertices_begin());
  As_filter_vertex_iterator last(as.finite_vertices_end(), test_as_vertex, as.finite_vertices_end());
  boost::python::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
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
    .def("__init__", make_constructor(&as_init1))
    .def("__init__", make_constructor(&as_init2))
#if CGALPY_ALPHA_SHAPE == CGALPY_ALPHA_SHAPE_PLAIN
    .def("__init__", make_constructor(&as_init3))
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
    .def("alpha_shape_cells", &alpha_shape_cells)
    .def("alpha_shape_facets", &alpha_shape_facets)
    .def("alpha_shape_edges", &alpha_shape_edges)
    .def("alpha_shape_vertices", &alpha_shape_vertices)
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
    // Operations
    .def("number_of_solid_components", number_of_solid_components1)
    .def("number_of_solid_components", number_of_solid_components2)
    .def("find_optimal_alpha", &Alpha_shape_3::find_optimal_alpha)
    .def("find_alpha_solid", &Alpha_shape_3::find_alpha_solid)
#endif
    ;
}
