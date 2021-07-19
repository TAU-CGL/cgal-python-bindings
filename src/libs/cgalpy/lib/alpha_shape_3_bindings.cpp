// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>

#include <CGAL/iterator.h>

#include "CGALPY/config.hpp"
#include "CGALPY/triangulation_3_types.hpp"
#include "CGALPY/alpha_shape_3_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"

namespace bp = boost::python;

namespace as3 {

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
void make_alpha_shape(Alpha_shape_3& as, bp::list& lst) {
  if (! lst) return;
  if (! bp::extract<Point>(lst[0]).check()) return;
  auto begin = bp::stl_input_iterator<Point>(lst);
  auto end = bp::stl_input_iterator<Point>();
  // auto v = std::vector<Point>(begin, end);
  // as.make_alpha_shape(v.begin(), v.end());
  as.make_alpha_shape(begin, end);
}
#endif

Alpha_shape_3* as_init1(bp::list& lst) {
  auto begin = bp::stl_input_iterator<Point>(lst);
  auto end = bp::stl_input_iterator<Point>();
  return new Alpha_shape_3(begin, end);
}

Alpha_shape_3* as_init2(bp::list& lst, const FT& alpha) {
  auto begin = bp::stl_input_iterator<Point>(lst);
  auto end = bp::stl_input_iterator<Point>();
  return new Alpha_shape_3(begin, end, alpha);
}

Alpha_shape_3* as_init3(bp::list& lst, double alpha) {
  auto begin = bp::stl_input_iterator<Point>(lst);
  auto end = bp::stl_input_iterator<Point>();
  return new Alpha_shape_3(begin, end, alpha);
}

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
Alpha_shape_3* as_init4(bp::list& lst, const FT& alpha, Mode m) {
  auto begin = bp::stl_input_iterator<Point>(lst);
  auto end = bp::stl_input_iterator<Point>();
  return new Alpha_shape_3(begin, end, alpha, m);
}

Alpha_shape_3* as_init5(bp::list& lst, double alpha, Mode m) {
  auto begin = bp::stl_input_iterator<Point>(lst);
  auto end = bp::stl_input_iterator<Point>();
  return new Alpha_shape_3(begin, end, alpha, m);
}

const FT& next(Alpha_iterator it) {
  if (it == Alpha_iterator()) {
    PyErr_SetString(PyExc_StopIteration, "Invalid alpha iterator");
    bp::throw_error_already_set();
  }
  return *it++;
}

#endif

template <typename AlphaShape_3>
class Alpha_shape_3_test {
private:
  typedef AlphaShape_3                                     Alpha_shape_3;
  typedef typename Alpha_shape_3::Classification_type      Classification_type;
  typedef typename Alpha_shape_3::FT                       FT;
  typedef typename Alpha_shape_3::Finite_cells_iterator    Finite_cells_iterator;
  typedef typename Alpha_shape_3::Finite_facets_iterator   Finite_facets_iterator;
  typedef typename Alpha_shape_3::Finite_edges_iterator    Finite_edges_iterator;
  typedef typename Alpha_shape_3::Finite_vertices_iterator Finite_vertices_iterator;

  const Alpha_shape_3& m_alpha_shape;
  Classification_type m_type;
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  const FT& m_alpha;
#endif

public:
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  Alpha_shape_3_test(const Alpha_shape_3& as, Classification_type type,
                     const FT& alpha) :
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

  bool operator()(Finite_cells_iterator cit) const {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    return m_alpha_shape.classify(cit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(cit) == m_type;
#endif
  }

  bool operator()(Finite_facets_iterator fit) const {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    return m_alpha_shape.classify(*fit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(*fit) == m_type;
#endif
  }

  bool operator()(Finite_edges_iterator eit) const {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    return m_alpha_shape.classify(*eit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(*eit) == m_type;
#endif
  }

  bool operator()(Finite_vertices_iterator vit) const {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    return m_alpha_shape.classify(vit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(vit) == m_type;
#endif
  }
};

typedef Alpha_shape_3_test<Alpha_shape_3>                      Test;
typedef CGAL::Filter_iterator<Finite_cells_iterator, Test>     Filter_cell_iterator;
typedef CGAL::Filter_iterator<Finite_facets_iterator, Test>    Filter_facet_iterator;
typedef CGAL::Filter_iterator<Finite_edges_iterator, Test>     Filter_edge_iterator;
typedef CGAL::Filter_iterator<Finite_vertices_iterator, Test>  Filter_vertex_iterator;


bp::list alpha_shape_cells(const Alpha_shape_3& as,
                           Classification_type type
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
                           , const FT& alpha
#endif
                           ) {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  Test test_as_cell(as, type, alpha);
#else
  Test test_as_cell(as, type);
#endif
  Filter_cell_iterator first(as.finite_cells_end(), test_as_cell, as.finite_cells_begin());
  Filter_cell_iterator last(as.finite_cells_end(), test_as_cell, as.finite_cells_end());
  // return bp::range<return_internal_reference<>, Alpha_shape_3>(&Alpha_shape_3::finite_cells_begin,
  //                                                                         &Alpha_shape_3::finite_cells_end);
  // return bp::range<return_internal_reference<>>(first, last);
  bp::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

  bp::list alpha_shape_facets(const Alpha_shape_3& as,
                              Classification_type type
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
                              , const FT& alpha
#endif
                              ) {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  Test test_as_facet(as, type, alpha);
#else
  Test test_as_facet(as, type);
#endif
  Filter_facet_iterator first(as.finite_facets_end(), test_as_facet, as.finite_facets_begin());
  Filter_facet_iterator last(as.finite_facets_end(), test_as_facet, as.finite_facets_end());
  bp::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

bp::list alpha_shape_edges(const Alpha_shape_3& as,
                           Classification_type type
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
                           , const FT& alpha
#endif
                           ) {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  Test test_as_edge(as, type, alpha);
#else
  Test test_as_edge(as, type);
#endif
  Filter_edge_iterator first(as.finite_edges_end(), test_as_edge, as.finite_edges_begin());
  Filter_edge_iterator last(as.finite_edges_end(), test_as_edge, as.finite_edges_end());
  bp::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

bp::list alpha_shape_vertices(const Alpha_shape_3& as,
                              Classification_type type
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
                              , const FT& alpha
#endif
                              ) {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  Test test_as_vertex(as, type, alpha);
#else
  Test test_as_vertex(as, type);
#endif
  Filter_vertex_iterator first(as.finite_vertices_end(), test_as_vertex, as.finite_vertices_begin());
  Filter_vertex_iterator last(as.finite_vertices_end(), test_as_vertex, as.finite_vertices_end());
  bp::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

#ifdef CGALPY_AS3_EXACT_COMPARISON

const typename FT::Exact_nt& FT_exact(const FT& ft)
{ return ft.exact(); }

const typename FT::Approximate_nt& FT_approx(const FT& ft)
{ return ft.approx(); }

double FT_to_double(FT& ft) { return CGAL::to_double(ft); }

#endif

} // end of as3 namespace

void export_alpha_shape_3() {
  typedef as3::Alpha_shape_3                    As3;
  typedef tri3::Triangulation_3                 Tri3;

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  as3::size_type (As3::*number_of_solid_components1)() const                     = &As3::number_of_solid_components;
  as3::size_type (As3::*number_of_solid_components2)(const as3::FT& alpha) const = &As3::number_of_solid_components;

  as3::Classification_type (As3::*classify1)(const as3::Point& p, const as3::FT& alpha) const              = &As3::classify;
  as3::Classification_type (As3::*classify2)(const as3::Edge& s, const as3::FT& alpha) const               = &As3::classify;
  as3::Classification_type (As3::*classify3)(const as3::Facet& s, const as3::FT& alpha) const              = &As3::classify;
  as3::Classification_type (As3::*classify4)(const as3::Vertex_handle& s, const as3::FT& alpha) const      = &As3::classify;
  as3::Classification_type (As3::*classify5)(const as3::Cell_handle& s, const as3::FT& alpha) const        = &As3::classify;
  as3::Classification_type (As3::*classify6)(const as3::Cell_handle& s, int i, const as3::FT& alpha) const = &As3::classify;

  as3::Classification_type (As3:: *classify7)(const as3::Point& p) const              = &As3::classify;
  as3::Classification_type (As3:: *classify8)(const as3::Edge& s) const               = &As3::classify;
  as3::Classification_type (As3:: *classify9)(const as3::Facet& s) const              = &As3::classify;
  as3::Classification_type (As3::*classify10)(const as3::Vertex_handle& s) const      = &As3::classify;
  as3::Classification_type (As3::*classify11)(const as3::Cell_handle& s) const        = &As3::classify;
  as3::Classification_type (As3::*classify12)(const as3::Cell_handle& s, int i) const = &As3::classify;

  as3::Alpha_status (As3::*get_alpha_status1)(const as3::Edge& e) const  = &As3::get_alpha_status;
  as3::Alpha_status (As3::*get_alpha_status2)(const as3::Facet& f) const = &As3::get_alpha_status;

#endif

  bp::class_<As3, boost::noncopyable>("As3")
    .def(bp::init<>())
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    .def(bp::init<bp::optional<double, as3::Mode>>())
    .def(bp::init<bp::optional<as3::FT&, as3::Mode>>())
    .def(bp::init<tri3::Triangulation_3&, bp::optional<double, as3::Mode>>())
    .def(bp::init<tri3::Triangulation_3&, bp::optional<as3::FT&, as3::Mode>>())
#endif
    .def("__init__", make_constructor(&as3::as_init1))
    .def("__init__", make_constructor(&as3::as_init2))
    .def("__init__", make_constructor(&as3::as_init3))
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    .def("__init__", make_constructor(&as3::as_init4))
    .def("__init__", make_constructor(&as3::as_init5))
#endif
    // Modifiers
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    .def("make_alpha_shape", &as3::make_alpha_shape)
    .def("set_mode", &As3::set_mode)
    .def("set_alpha", &As3::set_alpha)
#endif
    .def("clear", &As3::clear)
    // Query Functions
    .def("get_alpha", &As3::get_alpha, Copy_const_reference())
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    .def("get_mode", &As3::get_mode)
    .def("get_nth_alpha", &As3::get_nth_alpha, Copy_const_reference())
    .def("number_of_alphas", &As3::number_of_alphas)
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
    .def("alpha_shape_cells", &as3::alpha_shape_cells)
    .def("alpha_shape_facets", &as3::alpha_shape_facets)
    .def("alpha_shape_edges", &as3::alpha_shape_edges)
    .def("alpha_shape_vertices", &as3::alpha_shape_vertices)
    // .def("filtration", &As3::filtration)
    // .def("filtration_with_alpha_values", &As3::filtration_with_alpha_values)
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    // Traversal of the alpha-Values
    .def("alpha_begin", &As3::alpha_begin)
    .def("alpha_end", &As3::alpha_end)
    .def("alphas", bp::range(&As3::alpha_begin, &As3::alpha_end))
    .def("alpha_find", &As3::alpha_find)
    .def("alpha_lower_bound", &As3::alpha_lower_bound)
    .def("alpha_upper_bound", &As3::alpha_upper_bound)
    // Operations
    .def("number_of_solid_components", number_of_solid_components1)
    .def("number_of_solid_components", number_of_solid_components2)
    .def("find_optimal_alpha", &As3::find_optimal_alpha)
    .def("find_alpha_solid", &As3::find_alpha_solid)
#endif
    ;

  bp::enum_<as3::Classification_type>("Classification_type")
    .value("EXTERIOR", As3::EXTERIOR)
    .value("SINGULAR", As3::SINGULAR)
    .value("REGULAR", As3::REGULAR)
    .value("INTERIOR", As3::INTERIOR)
    .export_values()
    ;

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  bp::enum_<as3::Mode>("Mode")
    .value("GENERAL", As3::GENERAL)
    .value("REGULARIZED", As3::REGULARIZED)
    .export_values()
    ;

  bp::class_<as3::Alpha_status>("Alpha_status")
    .def(bp::init<>())
    // Modifiers
    .def("set_is_Gabriel", &as3::Alpha_status::set_is_Gabriel)
    .def("set_is_on_chull", &as3::Alpha_status::set_is_on_chull)
    .def("set_alpha_min", &as3::Alpha_status::set_alpha_min)
    .def("set_alpha_mid", &as3::Alpha_status::set_alpha_mid)
    .def("set_alpha_max", &as3::Alpha_status::set_alpha_max)
    // Access Functions
    .def("is_Gabriel", &as3::Alpha_status::is_Gabriel)
    .def("is_on_chull", &as3::Alpha_status::is_on_chull)
    .def("alpha_min", &as3::Alpha_status::alpha_min)
    .def("alpha_mid", &as3::Alpha_status::alpha_mid)
    .def("alpha_max", &as3::Alpha_status::alpha_max)
    ;

  bp::class_<as3::Alpha_iterator>("Alpha_iterator")
    .def("__iter__", &pass_through)
    .def("__next__", &as3::next, bp::return_value_policy<bp::copy_const_reference>())
    ;

#endif

  // Types that have been registered already:
#ifdef CGALPY_AS3_EXACT_COMPARISON

  // In the case of non-priodic triangulation the kernel serves as the traits.
  if (tri3::is_periodic())
    // \todo: generate bindings for periodic traits
    ;
  else add_attr<tri3::Geom_traits>("Gt", tri3_scope);
  add_attr<as3::Point>("Point", as3_scope);
  if (is_exact_ft()) add_attr<as3::FT>("FT", as3_scope);
  add_attr<as3::Vertex>("Vertex", as3_scope);
  add_attr<as3::Edge>("Edge", as3_scope);
  add_attr<as3::Facet>("Facet", as3_scope);
  add_attr<as3::Vertex_handle>("Vertex_handle", as3_scope);
  add_attr<as3::Cell_handle>("Cell_handle", as3_scope);

#endif
}
