// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/iterator.h>

#include "cgalpy/config.hpp"
#include "cgalpy/triangulation_3_types.hpp"
#include "cgalpy/alpha_shape_3_types.hpp"
#include "cgalpy/add_attr.hpp"
#include "cgalpy/stl_forward_iterator.hpp"
#include "cgalpy/make_iterator.hpp"
#include "cgalpy/As3_docstrings.hpp"

namespace py = nanobind;
namespace as3_doc = cgalpy::as3::docstrings;

namespace cgalpy {
namespace as3 {

//
py::object alphas(const Alpha_shape_3& as)
{ return make_iterator(as.alpha_begin(), as.alpha_end()); }

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
std::ptrdiff_t make_alpha_shape(Alpha_shape_3& as, py::list& lst) {
  auto begin = stl_forward_iterator<Point>(lst);
  auto end = stl_forward_iterator<Point>(lst, false);
  return as.make_alpha_shape(begin, end);
}
#endif

  void as_init1(Alpha_shape_3* as, py::list& lst) {
  auto begin = stl_forward_iterator<Point>(lst);
  auto end = stl_forward_iterator<Point>(lst, false);
  new (as) Alpha_shape_3(begin, end);           // placement new
}

  void as_init2(Alpha_shape_3* as, py::list& lst, const FT& alpha) {
  auto begin = stl_forward_iterator<Point>(lst);
  auto end = stl_forward_iterator<Point>(lst, false);
  new (as) Alpha_shape_3(begin, end, alpha);    // placement new
}

  void as_init3(Alpha_shape_3* as, py::list& lst, double alpha) {
  auto begin = stl_forward_iterator<Point>(lst);
  auto end = stl_forward_iterator<Point>(lst, false);
  new (as) Alpha_shape_3(begin, end, alpha);    // placement new
}

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  void as_init4(Alpha_shape_3* as, py::list& lst, const FT& alpha, Mode m) {
  auto begin = stl_forward_iterator<Point>(lst);
  auto end = stl_forward_iterator<Point>(lst, false);
  new (as) Alpha_shape_3(begin, end, alpha, m); // placement new
}

  void as_init5(Alpha_shape_3* as, py::list& lst, double alpha, Mode m) {
  auto begin = stl_forward_iterator<Point>(lst);
  auto end = stl_forward_iterator<Point>(lst, false);
  new (as) Alpha_shape_3(begin, end, alpha, m); // placement new
}

const FT& next(Alpha_iterator it) {
  if (it == Alpha_iterator()) {
    PyErr_SetString(PyExc_StopIteration, "Invalid alpha iterator");
    py::python_error();
  }
  return *it++;
}

#endif

template <typename AlphaShape_3>
class Alpha_shape_3_test {
private:
  using Alpha_shape_3 = AlphaShape_3;
  using Classification_type = typename Alpha_shape_3::Classification_type;
  using FT = typename Alpha_shape_3::FT;
  using Finite_cells_iterator = typename Alpha_shape_3::Finite_cells_iterator;
  using Finite_facets_iterator = typename Alpha_shape_3::Finite_facets_iterator;
  using Finite_edges_iterator = typename Alpha_shape_3::Finite_edges_iterator;
  using Finite_vertices_iterator =
    typename Alpha_shape_3::Finite_vertices_iterator;

  const Alpha_shape_3& m_alpha_shape;
  Classification_type m_type;
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  const FT& m_alpha;
#endif

public:
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  //
  Alpha_shape_3_test(const Alpha_shape_3& as, Classification_type type,
                     const FT& alpha) :
    m_alpha_shape(as),
    m_type(type),
    m_alpha(alpha)
  {}
#else
  //
  Alpha_shape_3_test(const Alpha_shape_3& as, Classification_type type) :
    m_alpha_shape(as),
    m_type(type)
  {}
#endif

  //
  bool operator()(Finite_cells_iterator cit) const {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    return m_alpha_shape.classify(cit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(cit) == m_type;
#endif
  }

  //
  bool operator()(Finite_facets_iterator fit) const {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    return m_alpha_shape.classify(*fit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(*fit) == m_type;
#endif
  }

  //
  bool operator()(Finite_edges_iterator eit) const {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    return m_alpha_shape.classify(*eit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(*eit) == m_type;
#endif
  }

  //
  bool operator()(Finite_vertices_iterator vit) const {
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    return m_alpha_shape.classify(vit, m_alpha) == m_type;
#else
    return m_alpha_shape.classify(vit) == m_type;
#endif
  }
};

using Test = Alpha_shape_3_test<Alpha_shape_3>;
using Filter_cell_iterator = CGAL::Filter_iterator<Finite_cells_iterator, Test>;
using Filter_facet_iterator =
  CGAL::Filter_iterator<Finite_facets_iterator, Test>;
using Filter_edge_iterator = CGAL::Filter_iterator<Finite_edges_iterator, Test>;
using Filter_vertex_iterator =
  CGAL::Filter_iterator<Finite_vertices_iterator, Test>;

//
py::list alpha_shape_cells(const Alpha_shape_3& as, Classification_type type
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
  // return py::range<return_internal_reference<>, Alpha_shape_3>(&Alpha_shape_3::finite_cells_begin,
  //                                                                         &Alpha_shape_3::finite_cells_end);
  // return py::range<return_internal_reference<>>(first, last);
  py::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

//
py::list alpha_shape_facets(const Alpha_shape_3& as,
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
  py::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

//
py::list alpha_shape_edges(const Alpha_shape_3& as,
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
  py::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

//
py::list alpha_shape_vertices(const Alpha_shape_3& as,
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
  py::list lst;
  for (auto it = first; it != last; ++it) lst.append(*it);
  return lst;
}

#ifdef CGALPY_AS3_EXACT_COMPARISON

//
const typename FT::Exact_nt& FT_exact(const FT& ft) { return ft.exact(); }

//
const typename FT::Approximate_nt& FT_approx(const FT& ft)
{ return ft.approx(); }

//
double FT_to_double(FT& ft) { return CGAL::to_double(ft); }

#endif

}
} // namespace cgalpy // end of as3 namespace

//
void export_alpha_shape_3(py::module_& m) {
  using As3 = cgalpy::as3::Alpha_shape_3;
  using Tri3 = cgalpy::tri3::Tri;
  constexpr auto ri(py::rv_policy::reference_internal);

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  cgalpy::as3::size_type (As3::*number_of_solid_components1)() const                     = &As3::number_of_solid_components;
  cgalpy::as3::size_type (As3::*number_of_solid_components2)(const cgalpy::as3::FT& alpha) const = &As3::number_of_solid_components;

  cgalpy::as3::Classification_type (As3::*classify1)(const cgalpy::as3::Point& p, const cgalpy::as3::FT& alpha) const              = &As3::classify;
  cgalpy::as3::Classification_type (As3::*classify2)(const cgalpy::as3::Edge& s, const cgalpy::as3::FT& alpha) const               = &As3::classify;
  cgalpy::as3::Classification_type (As3::*classify3)(const cgalpy::as3::Facet& s, const cgalpy::as3::FT& alpha) const              = &As3::classify;
  cgalpy::as3::Classification_type (As3::*classify4)(const cgalpy::as3::Vertex_handle& s, const cgalpy::as3::FT& alpha) const      = &As3::classify;
  cgalpy::as3::Classification_type (As3::*classify5)(const cgalpy::as3::Cell_handle& s, const cgalpy::as3::FT& alpha) const        = &As3::classify;
  cgalpy::as3::Classification_type (As3::*classify6)(const cgalpy::as3::Cell_handle& s, int i, const cgalpy::as3::FT& alpha) const = &As3::classify;

  cgalpy::as3::Classification_type (As3:: *classify7)(const cgalpy::as3::Point& p) const              = &As3::classify;
  cgalpy::as3::Classification_type (As3:: *classify8)(const cgalpy::as3::Edge& s) const               = &As3::classify;
  cgalpy::as3::Classification_type (As3:: *classify9)(const cgalpy::as3::Facet& s) const              = &As3::classify;
  cgalpy::as3::Classification_type (As3::*classify10)(const cgalpy::as3::Vertex_handle& s) const      = &As3::classify;
  cgalpy::as3::Classification_type (As3::*classify11)(const cgalpy::as3::Cell_handle& s) const        = &As3::classify;
  cgalpy::as3::Classification_type (As3::*classify12)(const cgalpy::as3::Cell_handle& s, int i) const = &As3::classify;

  cgalpy::as3::Alpha_status (As3::*get_alpha_status1)(const cgalpy::as3::Edge& e) const  = &As3::get_alpha_status;
  cgalpy::as3::Alpha_status (As3::*get_alpha_status2)(const cgalpy::as3::Facet& f) const = &As3::get_alpha_status;

#endif

  py::class_<As3> as3_c(m, "Alpha_shape_3",
                           as3_doc::Alpha_shape_3_class);

  py::enum_<cgalpy::as3::Classification_type>(
    as3_c, "Classification_type",
    as3_doc::Alpha_shape_3_Classification_type)
    .value("EXTERIOR", As3::EXTERIOR)
    .value("SINGULAR", As3::SINGULAR)
    .value("REGULAR", As3::REGULAR)
    .value("INTERIOR", As3::INTERIOR)
    .export_values()
    ;

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  py::enum_<cgalpy::as3::Mode>(as3_c, "Mode",
                                  as3_doc::Alpha_shape_3_Mode)
    .value("GENERAL", As3::GENERAL)
    .value("REGULARIZED", As3::REGULARIZED)
    .export_values()
    ;
#endif

  as3_c.def(py::init<>(), as3_doc::Alpha_shape_3_Alpha_shape_3)
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    .def(py::init<double, cgalpy::as3::Mode>(),
         py::arg("alpha"), py::arg("mode") = As3::REGULARIZED,
         as3_doc::Alpha_shape_3_Alpha_shape_3)
    .def(py::init<cgalpy::as3::FT&, cgalpy::as3::Mode>(),
         py::arg("alpha"), py::arg("mode") = As3::REGULARIZED,
         as3_doc::Alpha_shape_3_Alpha_shape_3)
    .def(py::init<Tri3&, double, cgalpy::as3::Mode>(),
         py::arg("dt"), py::arg("alpha"),
         py::arg("mode") = As3::REGULARIZED,
         as3_doc::Alpha_shape_3_Alpha_shape_3_1)
    .def(py::init<Tri3&, cgalpy::as3::FT&, cgalpy::as3::Mode>(),
         py::arg("dt"), py::arg("alpha"),
         py::arg("mode") = As3::REGULARIZED,
         as3_doc::Alpha_shape_3_Alpha_shape_3_1)
#endif
    .def("__init__", &cgalpy::as3::as_init1, py::arg("points"),
         as3_doc::Alpha_shape_3_Alpha_shape_3_2)
    .def("__init__", &cgalpy::as3::as_init2,
         py::arg("points"), py::arg("alpha"),
         as3_doc::Alpha_shape_3_Alpha_shape_3_2)
    .def("__init__", &cgalpy::as3::as_init3,
         py::arg("points"), py::arg("alpha"),
         as3_doc::Alpha_shape_3_Alpha_shape_3_2)
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    .def("__init__", &cgalpy::as3::as_init4,
         py::arg("points"), py::arg("alpha"),
         py::arg("mode") = As3::REGULARIZED,
         as3_doc::Alpha_shape_3_Alpha_shape_3_2)
    .def("__init__", &cgalpy::as3::as_init5,
         py::arg("points"), py::arg("alpha"),
         py::arg("mode") = As3::REGULARIZED,
         as3_doc::Alpha_shape_3_Alpha_shape_3_2)
#endif
    // Modifiers
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    .def("make_alpha_shape", &cgalpy::as3::make_alpha_shape,
         py::arg("points"),
         as3_doc::Alpha_shape_3_make_alpha_shape)
    .def("set_mode", &As3::set_mode,
         py::arg("mode") = As3::REGULARIZED,
         as3_doc::Alpha_shape_3_set_mode)
    .def("set_alpha", &As3::set_alpha, py::arg("alpha"),
         as3_doc::Alpha_shape_3_set_alpha)
#endif
    .def("clear", &As3::clear,
         as3_doc::Alpha_shape_3_clear)
    // Query Functions
    .def("get_alpha", &As3::get_alpha, ri,
         as3_doc::Alpha_shape_3_get_alpha)
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    .def("get_mode", &As3::get_mode,
         as3_doc::Alpha_shape_3_get_mode)
    .def("get_nth_alpha", &As3::get_nth_alpha, ri, py::arg("n"),
         as3_doc::Alpha_shape_3_get_nth_alpha)
    .def("number_of_alphas", &As3::number_of_alphas,
         as3_doc::Alpha_shape_3_number_of_alphas)
    .def("classify", classify1, py::arg("p"), py::arg("alpha"),
         as3_doc::Alpha_shape_3_classify)
    .def("classify", classify2, py::arg("edge"), py::arg("alpha"),
         as3_doc::Alpha_shape_3_classify_4)
    .def("classify", classify3, py::arg("facet"), py::arg("alpha"),
         as3_doc::Alpha_shape_3_classify_2)
    .def("classify", classify4, py::arg("vertex"), py::arg("alpha"),
         as3_doc::Alpha_shape_3_classify_5)
    .def("classify", classify5, py::arg("cell"), py::arg("alpha"),
         as3_doc::Alpha_shape_3_classify_1)
    .def("classify", classify6, py::arg("cell"), py::arg("i"),
         py::arg("alpha"),
         as3_doc::Alpha_shape_3_classify_3)
    .def("classify", classify7, py::arg("p"),
         as3_doc::Alpha_shape_3_classify)
    .def("classify", classify8, py::arg("edge"),
         as3_doc::Alpha_shape_3_classify_4)
    .def("classify", classify9, py::arg("facet"),
         as3_doc::Alpha_shape_3_classify_2)
    .def("classify", classify10, py::arg("vertex"),
         as3_doc::Alpha_shape_3_classify_5)
    .def("classify", classify11, py::arg("cell"),
         as3_doc::Alpha_shape_3_classify_1)
    .def("classify", classify12, py::arg("cell"), py::arg("i"),
         as3_doc::Alpha_shape_3_classify_3)
    .def("get_alpha_status", get_alpha_status1, py::arg("edge"),
         as3_doc::Alpha_shape_3_get_alpha_status)
    .def("get_alpha_status", get_alpha_status2, py::arg("facet"),
         as3_doc::Alpha_shape_3_get_alpha_status_1)
#endif
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    .def("alpha_shape_cells",
         [](const As3& as, cgalpy::as3::Classification_type type)
         { return cgalpy::as3::alpha_shape_cells(as, type, as.get_alpha()); },
         py::arg("type"),
         as3_doc::Alpha_shape_3_get_alpha_shape_cells)
    .def("alpha_shape_cells", &cgalpy::as3::alpha_shape_cells,
         py::arg("type"), py::arg("alpha"),
         as3_doc::Alpha_shape_3_get_alpha_shape_cells)
    .def("alpha_shape_facets",
         [](const As3& as, cgalpy::as3::Classification_type type)
         { return cgalpy::as3::alpha_shape_facets(as, type, as.get_alpha()); },
         py::arg("type"),
         as3_doc::Alpha_shape_3_get_alpha_shape_facets)
    .def("alpha_shape_facets", &cgalpy::as3::alpha_shape_facets,
         py::arg("type"), py::arg("alpha"),
         as3_doc::Alpha_shape_3_get_alpha_shape_facets)
    .def("alpha_shape_edges",
         [](const As3& as, cgalpy::as3::Classification_type type)
         { return cgalpy::as3::alpha_shape_edges(as, type, as.get_alpha()); },
         py::arg("type"),
         as3_doc::Alpha_shape_3_get_alpha_shape_edges)
    .def("alpha_shape_edges", &cgalpy::as3::alpha_shape_edges,
         py::arg("type"), py::arg("alpha"),
         as3_doc::Alpha_shape_3_get_alpha_shape_edges)
    .def("alpha_shape_vertices", &cgalpy::as3::alpha_shape_vertices,
         py::arg("type"), py::arg("alpha"),
         as3_doc::Alpha_shape_3_get_alpha_shape_vertices)
#else
    .def("alpha_shape_cells", &cgalpy::as3::alpha_shape_cells,
         py::arg("type"),
         as3_doc::Fixed_alpha_shape_3_get_alpha_shape_cells)
    .def("alpha_shape_facets", &cgalpy::as3::alpha_shape_facets,
         py::arg("type"),
         as3_doc::Fixed_alpha_shape_3_get_alpha_shape_facets)
    .def("alpha_shape_edges", &cgalpy::as3::alpha_shape_edges,
         py::arg("type"),
         as3_doc::Fixed_alpha_shape_3_get_alpha_shape_edges)
    .def("alpha_shape_vertices", &cgalpy::as3::alpha_shape_vertices,
         py::arg("type"),
         as3_doc::Fixed_alpha_shape_3_get_alpha_shape_vertices)
#endif
    // .def("filtration", &As3::filtration)
    // .def("filtration_with_alpha_values", &As3::filtration_with_alpha_values)
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
    // Traversal of the alpha-Values
    .def("alphas", &cgalpy::as3::alphas, py::keep_alive<0, 1>(),
         "Iterate over the sorted alpha-values of the family of alpha shapes.")
    .def("alpha_find", &As3::alpha_find, py::arg("alpha"),
         as3_doc::Alpha_shape_3_alpha_find)
    .def("alpha_lower_bound", &As3::alpha_lower_bound, py::arg("alpha"),
         as3_doc::Alpha_shape_3_alpha_lower_bound)
    .def("alpha_upper_bound", &As3::alpha_upper_bound, py::arg("alpha"),
         as3_doc::Alpha_shape_3_alpha_upper_bound)
    // Operations
    .def("number_of_solid_components", number_of_solid_components1,
         as3_doc::Alpha_shape_3_number_of_solid_components)
    .def("number_of_solid_components", number_of_solid_components2,
         py::arg("alpha"),
         as3_doc::Alpha_shape_3_number_of_solid_components)
    .def("find_optimal_alpha", &As3::find_optimal_alpha,
         py::arg("nb_components"),
         as3_doc::Alpha_shape_3_find_optimal_alpha)
    .def("find_alpha_solid", &As3::find_alpha_solid,
         "Return an alpha value for which the alpha shape is solid.")
#endif
    ;

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  py::class_<cgalpy::as3::Alpha_status>(
      as3_c, "Alpha_status", as3_doc::Alpha_status_class)
    .def(py::init<>(), as3_doc::Alpha_status_Alpha_status)
    // Modifiers
    .def("set_is_Gabriel", &cgalpy::as3::Alpha_status::set_is_Gabriel,
         py::arg("yesorno"), as3_doc::Alpha_status_set_is_Gabriel)
    .def("set_is_on_chull", &cgalpy::as3::Alpha_status::set_is_on_chull,
         py::arg("yesorno"), as3_doc::Alpha_status_set_is_on_chull)
    .def("set_alpha_min", &cgalpy::as3::Alpha_status::set_alpha_min,
         py::arg("alpha"), as3_doc::Alpha_status_set_alpha_min)
    .def("set_alpha_mid", &cgalpy::as3::Alpha_status::set_alpha_mid,
         py::arg("alpha"), as3_doc::Alpha_status_set_alpha_mid)
    .def("set_alpha_max", &cgalpy::as3::Alpha_status::set_alpha_max,
         py::arg("alpha"), as3_doc::Alpha_status_set_alpha_max)
    // Access Functions
    .def("is_Gabriel", &cgalpy::as3::Alpha_status::is_Gabriel,
         as3_doc::Alpha_status_is_Gabriel)
    .def("is_on_chull", &cgalpy::as3::Alpha_status::is_on_chull,
         as3_doc::Alpha_status_is_on_chull)
    .def("alpha_min", &cgalpy::as3::Alpha_status::alpha_min,
         as3_doc::Alpha_status_alpha_min)
    .def("alpha_mid", &cgalpy::as3::Alpha_status::alpha_mid,
         as3_doc::Alpha_status_alpha_mid)
    .def("alpha_max", &cgalpy::as3::Alpha_status::alpha_max,
         as3_doc::Alpha_status_alpha_max)
    ;
#endif

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  using Ai = cgalpy::as3::Alpha_iterator;
  add_iterator<Ai, Ai>("Alpha_iterator", as3_c);
#endif

  // Types that have been registered already:
#ifdef CGALPY_AS3_EXACT_COMPARISON

  // In the case of non-priodic triangulation the kernel serves as the traits.
  if (cgalpy::tri3::is_periodic())
    // \todo: generate bindings for periodic traits
    ;
  else {
    if (! add_attr<cgalpy::tri3::Geom_traits>(as3_scope, "Gt"))
      std::cerr << "'cgalpy::tri3::Geom_traits' not registered!\n";
  }
  if (! add_attr<cgalpy::as3::Point>(as3_c, "Point"))
    std::cerr << "'cgalpy::as3::Point' not registered!\n";
  if (is_exact_ft()) {
    if (! add_attr<cgalpy::as3::FT>(as3_c, "FT"))
      std::cerr << "'cgalpy::as3::FT' not registered!\n";
  }
  if (! add_attr<cgalpy::as3::Vertex>(as3_c, "Vertex"))
    std::cerr << "'cgalpy::as3::Vertex' not registered!\n";
  if (! add_attr<cgalpy::as3::Edge>(as3_c, "Edge"))
    std::cerr << "'cgalpy::as3::Edge' not registered!\n";
  if (! add_attr<cgalpy::as3::Facet>(as3_c, "Facet"))
    std::cerr << "'cgalpy::as3::Facet' not registered!\n";
  if (! add_attr<cgalpy::as3::Vertex_handle>(as3_c, "Vertex_handle"))
    std::cerr << "'cgalpy::as3::Vertex_handle' not registered!\n";
  if (! add_attr<cgalpy::as3::Cell_handle>(as3_c, "Cell_handle"))
    std::cerr << "'cgalpy::as3::Cell_handle' not registered!\n";

#endif
}
