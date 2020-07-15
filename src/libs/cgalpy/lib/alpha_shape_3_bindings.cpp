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
#include "CGALPY/triangulation_3_types.hpp"
#include "CGALPY/alpha_shape_3_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"

namespace bp = boost::python;

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
void make_alpha_shape(as3::Alpha_shape_3& as, boost::python::list& lst)
{
  if (! lst) return;
  if (! bp::extract<as3::Point>(lst[0]).check()) return;
  auto begin = boost::python::stl_input_iterator<as3::Point>(lst);
  auto end = boost::python::stl_input_iterator<as3::Point>();
  // auto v = std::vector<as3::point>(begin, end);
  // as.make_alpha_shape(v.begin(), v.end());
  as.make_alpha_shape(begin, end);
}
#endif

as3::Alpha_shape_3* as_init1(boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<as3::Point>(lst);
  auto end = boost::python::stl_input_iterator<as3::Point>();
  return new as3::Alpha_shape_3(begin, end);
}

as3::Alpha_shape_3* as_init2(boost::python::list& lst, const as3::NT& alpha)
{
  auto begin = boost::python::stl_input_iterator<as3::Point>(lst);
  auto end = boost::python::stl_input_iterator<as3::Point>();
  return new as3::Alpha_shape_3(begin, end, alpha);
}

as3::Alpha_shape_3* as_init3(boost::python::list& lst, double alpha)
{
  auto begin = boost::python::stl_input_iterator<as3::Point>(lst);
  auto end = boost::python::stl_input_iterator<as3::Point>();
  return new as3::Alpha_shape_3(begin, end, alpha);
}

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
as3::Alpha_shape_3* as_init4(boost::python::list& lst,
                             const as3::NT& alpha, as3::Mode m)
{
  auto begin = boost::python::stl_input_iterator<as3::Point>(lst);
  auto end = boost::python::stl_input_iterator<as3::Point>();
  return new as3::Alpha_shape_3(begin, end, alpha, m);
}

as3::Alpha_shape_3* as_init5(boost::python::list& lst,
                             double alpha, as3::Mode m)
{
  auto begin = boost::python::stl_input_iterator<as3::Point>(lst);
  auto end = boost::python::stl_input_iterator<as3::Point>();
  return new as3::Alpha_shape_3(begin, end, alpha, m);
}

const as3::NT& next(as3::Alpha_iterator it)
{
  if (it == as3::Alpha_iterator()) {
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
  typedef typename Alpha_shape_3::NT                       NT;
  typedef typename Alpha_shape_3::Finite_cells_iterator    Finite_cells_iterator;
  typedef typename Alpha_shape_3::Finite_facets_iterator   Finite_facets_iterator;
  typedef typename Alpha_shape_3::Finite_edges_iterator    Finite_edges_iterator;
  typedef typename Alpha_shape_3::Finite_vertices_iterator Finite_vertices_iterator;

  const Alpha_shape_3& m_alpha_shape;
  Classification_type m_type;
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  const NT& m_alpha;
#endif

public:
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
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
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    return m_alpha_shape.classify(cit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(cit) == m_type;
#endif
  }

  bool operator()(Finite_facets_iterator fit) const
  {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    return m_alpha_shape.classify(*fit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(*fit) == m_type;
#endif
  }

  bool operator()(Finite_edges_iterator eit) const
  {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    return m_alpha_shape.classify(*eit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(*eit) == m_type;
#endif
  }

  bool operator()(Finite_vertices_iterator vit) const
  {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    return m_alpha_shape.classify(vit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(vit) == m_type;
#endif
  }
};

typedef Alpha_shape_3_test<as3::Alpha_shape_3>                      Test;
typedef CGAL::Filter_iterator<as3::Finite_cells_iterator, Test>     Filter_cell_iterator;
typedef CGAL::Filter_iterator<as3::Finite_facets_iterator, Test>    Filter_facet_iterator;
typedef CGAL::Filter_iterator<as3::Finite_edges_iterator, Test>     Filter_edge_iterator;
typedef CGAL::Filter_iterator<as3::Finite_vertices_iterator, Test>  Filter_vertex_iterator;


boost::python::list alpha_shape_cells(const as3::Alpha_shape_3& as,
                                      as3::Classification_type type
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
                                      , const as3::NT& alpha
#endif
                                      )
{
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  Test test_as_cell(as, type, alpha);
#else
  Test test_as_cell(as, type);
#endif
  Filter_cell_iterator first(as.finite_cells_end(), test_as_cell, as.finite_cells_begin());
  Filter_cell_iterator last(as.finite_cells_end(), test_as_cell, as.finite_cells_end());
  // return boost::python::range<return_internal_reference<>, Alpha_shape_3>(&Alpha_shape_3::finite_cells_begin,
  //                                                                         &Alpha_shape_3::finite_cells_end);
  // return boost::python::range<return_internal_reference<>>(first, last);
  boost::python::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

  boost::python::list alpha_shape_facets(const as3::Alpha_shape_3& as,
                                         as3::Classification_type type
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
                                         , const as3::NT& alpha
#endif
                                       )
{
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  Test test_as_facet(as, type, alpha);
#else
  Test test_as_facet(as, type);
#endif
  Filter_facet_iterator first(as.finite_facets_end(), test_as_facet, as.finite_facets_begin());
  Filter_facet_iterator last(as.finite_facets_end(), test_as_facet, as.finite_facets_end());
  boost::python::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

boost::python::list alpha_shape_edges(const as3::Alpha_shape_3& as,
                                      as3::Classification_type type
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
                                      , const as3::NT& alpha
#endif
                                      )
{
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  Test test_as_edge(as, type, alpha);
#else
  Test test_as_edge(as, type);
#endif
  Filter_edge_iterator first(as.finite_edges_end(), test_as_edge, as.finite_edges_begin());
  Filter_edge_iterator last(as.finite_edges_end(), test_as_edge, as.finite_edges_end());
  boost::python::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

boost::python::list alpha_shape_vertices(const as3::Alpha_shape_3& as,
                                         as3::Classification_type type
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
                                         , const as3::NT& alpha
#endif
                                         )
{
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  Test test_as_vertex(as, type, alpha);
#else
  Test test_as_vertex(as, type);
#endif
  Filter_vertex_iterator first(as.finite_vertices_end(), test_as_vertex, as.finite_vertices_begin());
  Filter_vertex_iterator last(as.finite_vertices_end(), test_as_vertex, as.finite_vertices_end());
  boost::python::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

#ifdef CGALPY_AS3_EXACT_COMPARISON

const typename as3::NT::Exact_nt& NT_exact(const as3::NT& ft)
{ return ft.exact(); }

const typename as3::NT::Approximate_nt& NT_approx(const as3::NT& ft)
{ return ft.approx(); }

double NT_to_double(as3::NT& ft) { return CGAL::to_double(ft); }

#endif

void export_alpha_shape_3()
{
  using namespace boost::python;

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  enum_<as3::Mode>("Mode")
    .value("GENERAL", as3::Alpha_shape_3::GENERAL)
    .value("REGULARIZED", as3::Alpha_shape_3::REGULARIZED)
    .export_values()
    ;

  enum_<as3::Classification_type>("Classification_type")
    .value("EXTERIOR", as3::Alpha_shape_3::EXTERIOR)
    .value("SINGULAR", as3::Alpha_shape_3::SINGULAR)
    .value("REGULAR", as3::Alpha_shape_3::REGULAR)
    .value("INTERIOR", as3::Alpha_shape_3::INTERIOR)
    .export_values()
    ;

  class_<as3::Alpha_status>("Alpha_status")
    .def(init<>())
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

  class_<as3::Alpha_iterator>("Alpha_iterator")
    .def("__iter__", &pass_through)
    .def("__next__", &next, return_value_policy<copy_const_reference>())
    ;

  typedef as3::Alpha_shape_3                    As_3;
  as3::Size_type (As_3::*number_of_solid_components1)() const                     = &As_3::number_of_solid_components;
  as3::Size_type (As_3::*number_of_solid_components2)(const as3::NT& alpha) const = &As_3::number_of_solid_components;

  as3::Classification_type (As_3::*classify1)(const as3::Point& p, const as3::NT& alpha) const              = &As_3::classify;
  as3::Classification_type (As_3::*classify2)(const as3::Edge& s, const as3::NT& alpha) const               = &As_3::classify;
  as3::Classification_type (As_3::*classify3)(const as3::Facet& s, const as3::NT& alpha) const              = &As_3::classify;
  as3::Classification_type (As_3::*classify4)(const as3::Vertex_handle& s, const as3::NT& alpha) const      = &As_3::classify;
  as3::Classification_type (As_3::*classify5)(const as3::Cell_handle& s, const as3::NT& alpha) const        = &As_3::classify;
  as3::Classification_type (As_3::*classify6)(const as3::Cell_handle& s, int i, const as3::NT& alpha) const = &As_3::classify;

  as3::Classification_type (As_3:: *classify7)(const as3::Point& p) const              = &As_3::classify;
  as3::Classification_type (As_3:: *classify8)(const as3::Edge& s) const               = &As_3::classify;
  as3::Classification_type (As_3:: *classify9)(const as3::Facet& s) const              = &As_3::classify;
  as3::Classification_type (As_3::*classify10)(const as3::Vertex_handle& s) const      = &As_3::classify;
  as3::Classification_type (As_3::*classify11)(const as3::Cell_handle& s) const        = &As_3::classify;
  as3::Classification_type (As_3::*classify12)(const as3::Cell_handle& s, int i) const = &As_3::classify;

  as3::Alpha_status (As_3::*get_alpha_status1)(const as3::Edge& e) const  = &As_3::get_alpha_status;
  as3::Alpha_status (As_3::*get_alpha_status2)(const as3::Facet& f) const = &As_3::get_alpha_status;

#endif

#ifdef CGALPY_AS3_EXACT_COMPARISON
  // By default (last time I checked) this is mapped to:
  // CGAL::internal::Lazy_alpha_nt_3<CGAL::Epick, true, CGAL::Boolean_tag<false>>
  class_<as3::NT>("NT")
    .def(init<double>())
    // .def(init<as3::NT::NT_exact>())
    .def(init<as3::NT>())
    .def("exact", &NT_exact, return_internal_reference<>())
    .def("approx", &NT_approx, return_internal_reference<>())
    .def("to_double", &NT_to_double)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .def(self < self)
    .def(self > self)
    .def(self <= self)
    .def(self >= self)
    // .def(self + self)
    // .def(self += self)
    // .def(self - self)
    // .def(self -= self)
    // .def(self * self)
    // .def(self *= self)
    // .def(self / self)
    // .def(self /= self)
    // .def(-self)
    ;
#endif

  class_<as3::Alpha_shape_3, boost::noncopyable>("Alpha_shape_3")
    .def(init<>())
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    .def(init<optional<double, as3::Mode>>())
    .def(init<optional<as3::NT&, as3::Mode>>())
    .def(init<tri3::Triangulation_3&, optional<double, as3::Mode>>())
    .def(init<tri3::Triangulation_3&, optional<as3::NT&, as3::Mode>>())
#endif
    .def("__init__", make_constructor(&as_init1))
    .def("__init__", make_constructor(&as_init2))
    .def("__init__", make_constructor(&as_init3))
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    .def("__init__", make_constructor(&as_init4))
    .def("__init__", make_constructor(&as_init5))
#endif
    // Modifiers
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    .def("make_alpha_shape", &make_alpha_shape)
    .def("set_mode", &as3::Alpha_shape_3::set_mode)
    .def("set_alpha", &as3::Alpha_shape_3::set_alpha)
#endif
    .def("clear", &as3::Alpha_shape_3::clear)
    // Query Functions
    .def("get_alpha", &as3::Alpha_shape_3::get_alpha, return_value_policy<copy_const_reference>())
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    .def("get_mode", &as3::Alpha_shape_3::get_mode)
    .def("get_nth_alpha", &as3::Alpha_shape_3::get_nth_alpha, return_value_policy<copy_const_reference>())
    .def("number_of_alphas", &as3::Alpha_shape_3::number_of_alphas)
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
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    // Traversal of the alpha-Values
    .def("alpha_begin", &as3::Alpha_shape_3::alpha_begin)
    .def("alpha_end", &as3::Alpha_shape_3::alpha_end)
    .def("alphas", range(&as3::Alpha_shape_3::alpha_begin, &as3::Alpha_shape_3::alpha_end))
    .def("alpha_find", &as3::Alpha_shape_3::alpha_find)
    .def("alpha_lower_bound", &as3::Alpha_shape_3::alpha_lower_bound)
    .def("alpha_upper_bound", &as3::Alpha_shape_3::alpha_upper_bound)
    // Operations
    .def("number_of_solid_components", number_of_solid_components1)
    .def("number_of_solid_components", number_of_solid_components2)
    .def("find_optimal_alpha", &as3::Alpha_shape_3::find_optimal_alpha)
    .def("find_alpha_solid", &as3::Alpha_shape_3::find_alpha_solid)
#endif
    ;
}
