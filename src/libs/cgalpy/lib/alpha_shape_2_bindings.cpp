// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Nir Goren         <nirgoren@mail.tau.ac.il>

#include <boost/python.hpp>

#include "CGALPY/types.hpp"

namespace bp = boost::python;

namespace as2 {

as2::Alpha_shape_2* as_init(bp::list& lst) {
  auto begin = bp::stl_input_iterator<as2::Point>(lst);
  auto end = bp::stl_input_iterator<as2::Point>();
  return new as2::Alpha_shape_2(begin, end);
}

const as2::FT& next(as2::Alpha_iterator it)
{
  if (it == as2::Alpha_iterator()) {
    PyErr_SetString(PyExc_StopIteration, "Invalid alpha iterator");
    bp::throw_error_already_set();
  }
  return *it++;
}

}

void export_alpha_shape_2() {
  typedef as2::Alpha_shape_2                    As2;
  typedef tri2::Triangulation_2                 Tri2;

  as2::size_type (As2::*number_of_solid_components1)() const                     = &As2::number_of_solid_components;
  as2::size_type (As2::*number_of_solid_components2)(const as3::NT& alpha) const = &As2::number_of_solid_components;

  as2::Classification_type (As3::*classify1)(const as2::Point& p, const as2::NT& alpha) const              = &As3::classify;
  as2::Classification_type (As3::*classify2)(const as2::Edge& s, const as2::NT& alpha) const               = &As3::classify;
  as2::Classification_type (As3::*classify3)(const as2::Facet& s, const as2::NT& alpha) const              = &As3::classify;
  as2::Classification_type (As3::*classify4)(const as2::Vertex_handle& s, const as2::NT& alpha) const      = &As3::classify;
  as2::Classification_type (As3::*classify5)(const as2::Cell_handle& s, const as2::NT& alpha) const        = &As3::classify;
  as2::Classification_type (As3::*classify6)(const as2::Cell_handle& s, int i, const as2::NT& alpha) const = &As3::classify;

  as2::Classification_type (As3:: *classify7)(const as2::Point& p) const              = &As3::classify;
  as2::Classification_type (As3:: *classify8)(const as2::Edge& s) const               = &As3::classify;
  as2::Classification_type (As3:: *classify9)(const as2::Facet& s) const              = &As3::classify;
  as2::Classification_type (As3::*classify10)(const as2::Vertex_handle& s) const      = &As3::classify;
  as2::Classification_type (As3::*classify11)(const as2::Cell_handle& s) const        = &As3::classify;
  as2::Classification_type (As3::*classify12)(const as2::Cell_handle& s, int i) const = &As3::classify;

  bp::scope aos_scope =
    bp::class_<As2, boost::noncopyable>("Alpha_shape_3")
    .def(bp::init<>())
    .def(bp::init<bp::optional<double, as2::Mode>>())
    .def(bp::init<bp::optional<as2::FT&, as2::Mode>>())
    .def(bp::init<Tri2&, bp::optional<double, as2::Mode>>())
    .def(bp::init<Tri2&, bp::optional<as2::FT&, as2::Mode>>())
    .def("__init__", make_constructor(&as_init))
    .def("clear", &as2::As2::clear)
    .def("set_mode", &as2::As2::set_mode)
    .def("set_alpha", &as2::As2::set_alpha)
    .def("get_alpha", &as2::As2::get_alpha, Copy_const_reference>())
    .def("get_nth_alpha", &as2::As2::get_nth_alpha, Copy_const_reference>())
    .def("number_of_alphas", &as2::As2::number_of_alphas)
    .def("set_mode", &as2::As2::set_mode)
    .def("number_of_solid_components", number_of_solid_components1)
    .def("number_of_solid_components", number_of_solid_components2)
    .def("find_optimal_alpha", &as2::As2::find_optimal_alpha)
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
    .def("alpha_begin", &as2::As2::alpha_begin)
    .def("alpha_end", &as2::As2::alpha_end)
    .def("alpha_find", &as2::As2::alpha_find)
    .def("alpha_lower_bound", &as2::As2::alpha_lower_bound)
    .def("alpha_upper_bound", &as2::As2::alpha_upper_bound)

    // alpha_shape_vertices_begin
    // alpha_shape_vertices_end
    // alpha_shape_edges_begin
    // alpha_shape_edges_end
    ;

  // Alpha_shape_vertices_iterator;
  // Alpha_shape_edges_iterator;
  // Dt
  // Gt::FT;
  // Gt;
  // Dt::Point;
  // size_type;

  bp::class_<as2::Alpha_iterator>("Alpha_iterator")
    .def("__iter__", &pass_through)
    .def("__next__", &next, Copy_const_reference>())
    ;

  bp::enum_<as2::Classification_type>("Classification_type")
    .value("EXTERIOR", as2::Alpha_shape_3::EXTERIOR)
    .value("SINGULAR", as2::Alpha_shape_3::SINGULAR)
    .value("REGULAR", as2::Alpha_shape_3::REGULAR)
    .value("INTERIOR", as2::Alpha_shape_3::INTERIOR)
    .export_values()
    ;

  bp::enum_<as2::Mode>("Mode")
    .value("GENERAL", as2::Alpha_shape_3::GENERAL)
    .value("REGULARIZED", as2::Alpha_shape_3::REGULARIZED)
    .export_values()
    ;}
