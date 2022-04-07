// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Nir Goren         <nirgoren@mail.tau.ac.il>

#include <nanobind/nanobind.h>
#include <boost/assert.hpp>

#include "CGALPY/types.hpp"
#include "CGALPY/alpha_shape_2_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

namespace as2 {

Alpha_shape_2* as_init(py::list& lst) {
  auto begin = py::stl_input_iterator<Point>(lst);
  auto end = py::stl_input_iterator<Point>();
  return new Alpha_shape_2(begin, end);
}

const FT& next(Alpha_iterator it) {
  if (it == Alpha_iterator()) {
    PyErr_SetString(PyExc_StopIteration, "Invalid alpha iterator");
    py::throw_error_already_set();
  }
  return *it++;
}

py::list alpha_shape_edges(const as2::Alpha_shape_2& as) {
  py::list lst;
  for (auto it = as.alpha_shape_edges_begin(); it != as.alpha_shape_edges_end();
       ++it)
    lst.append(*it);
  return lst;
}

py::list alpha_shape_vertices(const as2::Alpha_shape_2& as) {
  py::list lst;
  for (auto it = as.alpha_shape_vertices_begin();
       it != as.alpha_shape_vertices_end(); ++it)
    lst.append(*it);
  return lst;
}

} // end of as2 namespace

void export_alpha_shape_2() {
  typedef as2::Alpha_shape_2                    As2;
  typedef tri2::Triangulation_2                 Tri2;

  as2::size_type (As2::*number_of_solid_components1)() const                     = &As2::number_of_solid_components;
  as2::size_type (As2::*number_of_solid_components2)(const as2::FT& alpha) const = &As2::number_of_solid_components;

  as2::Classification_type (As2::*classify1)(const as2::Point& p, const as2::FT& alpha) const              = &As2::classify;
  as2::Classification_type (As2::*classify2)(const as2::Edge& s, const as2::FT& alpha) const               = &As2::classify;
  as2::Classification_type (As2::*classify3)(const as2::Vertex_handle& s, const as2::FT& alpha) const      = &As2::classify;
  as2::Classification_type (As2::*classify4)(const as2::Face_handle& s, const as2::FT& alpha) const        = &As2::classify;
  as2::Classification_type (As2::*classify5)(const as2::Face_handle& s, int i, const as2::FT& alpha) const = &As2::classify;

  as2::Classification_type (As2::*classify6)(const as2::Point& p) const               = &As2::classify;
  as2::Classification_type (As2::*classify7)(const as2::Edge& s) const                = &As2::classify;
  as2::Classification_type (As2::*classify8)(const as2::Vertex_handle& s) const       = &As2::classify;
  as2::Classification_type (As2::*classify9)(const as2::Face_handle& s) const         = &As2::classify;
  as2::Classification_type (As2::*classify10)(const as2::Face_handle& s, int i) const = &As2::classify;

  py::scope as2_scope =
    py::class_<As2, boost::noncopyable>("Alpha_shape_2")
    .def(py::init<>())
    .def(py::init<py::optional<double, as2::Mode>>())
    .def(py::init<py::optional<as2::FT&, as2::Mode>>())
    .def(py::init<Tri2&, py::optional<double, as2::Mode>>())
    .def(py::init<Tri2&, py::optional<as2::FT&, as2::Mode>>())
    .def("__init__", make_constructor(&as2::as_init))
    .def("clear", &As2::clear)
    .def("set_mode", &As2::set_mode)
    .def("set_alpha", &As2::set_alpha)
    .def("get_alpha", &As2::get_alpha, Copy_const_reference())
    .def("get_nth_alpha", &As2::get_nth_alpha, Copy_const_reference())
    .def("number_of_alphas", &As2::number_of_alphas)
    .def("set_mode", &As2::set_mode)
    .def("number_of_solid_components", number_of_solid_components1)
    .def("number_of_solid_components", number_of_solid_components2)
    .def("find_optimal_alpha", &As2::find_optimal_alpha)
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
    .def("alpha_begin", &As2::alpha_begin)
    .def("alpha_end", &As2::alpha_end)
    .def("alpha_find", &As2::alpha_find)
    .def("alpha_lower_bound", &As2::alpha_lower_bound)
    .def("alpha_upper_bound", &As2::alpha_upper_bound)
    .def("alpha_shape_edges", &as2::alpha_shape_edges)
    .def("alpha_shape_vertices", &as2::alpha_shape_vertices)
    ;

  // Alpha_shape_vertices_iterator;
  // Alpha_shape_edges_iterator;

  py::class_<as2::Alpha_iterator>("Alpha_iterator")
    .def("__iter__", &pass_through)
    .def("__next__", &as2::next, Copy_const_reference())
    ;

  py::enum_<as2::Classification_type>("Classification_type")
    .value("EXTERIOR", As2::EXTERIOR)
    .value("SINGULAR", As2::SINGULAR)
    .value("REGULAR", As2::REGULAR)
    .value("INTERIOR", As2::INTERIOR)
    .export_values()
    ;

  py::enum_<as2::Mode>("Mode")
    .value("GENERAL", As2::GENERAL)
    .value("REGULARIZED", As2::REGULARIZED)
    .export_values()
    ;

  // Types that have been registered already:
  if (tri2::is_periodic())
    // \todo: generate bindings for periodic traits
    ;
  else {
    if (! add_attr<as2::Gt>("Gt", as2_scope))
      std::cerr << "'as2::Gt' not registered!\n";
  }
  add_attr<as2::Point>("Point", as2_scope);
  if (is_exact_ft()) {
    if (! add_attr<as2::FT>("FT", as2_scope))
      std::cerr << "'as2::FT' not registered!\n";
  }
  if (! add_attr<as2::Tds>("Tds", as2_scope))
    std::cerr << "'as2::Tds' not registered!\n";
  if (! add_attr<as2::Vertex>("Vertex", as2_scope))
    std::cerr << "'as2::Vertex' not registered!\n";
  if (! add_attr<as2::Edge>("Edge", as2_scope))
    std::cerr << "'as2::Edge' not registered!\n";
  if (! add_attr<as2::Vertex_handle>("Vertex_handle", as2_scope))
    std::cerr << "'as2::Vertex_handle' not registered!\n";
  if (! add_attr<as2::Face_handle>("Face_handle", as2_scope))
    std::cerr << "'as2::Face_handle' not registered!\n";
}
