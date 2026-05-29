// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Nir Goren         <nirgoren@mail.tau.ac.il>

#include <boost/assert.hpp>

#include <nanobind/nanobind.h>

#include "CGALPY/types.hpp"
#include "CGALPY/alpha_shape_2_types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/stl_forward_iterator.hpp"
#include "CGALPY/make_iterator.hpp"
#include "cgalpy/As2_docstrings.hpp"

namespace py = nanobind;
namespace as2_doc = cgalpy::as2::docstrings;

namespace cgalpy {
namespace as2 {

py::object alphas(const Alpha_shape_2& as)
{ return make_iterator(as.alpha_begin(), as.alpha_end()); }

void as_init(Alpha_shape_2* as, py::list& lst) {
  auto begin = stl_forward_iterator<Point>(lst);
  auto end = stl_forward_iterator<Point>(lst, false);
  new (as) Alpha_shape_2(begin, end);   // placement new
}

std::ptrdiff_t make_alpha_shape(Alpha_shape_2& as, py::list& lst) {
  auto begin = stl_forward_iterator<Point>(lst);
  auto end = stl_forward_iterator<Point>(lst, false);
  return as.make_alpha_shape(begin, end);
}

const FT& next(Alpha_iterator it) {
  if (it == Alpha_iterator()) {
    PyErr_SetString(PyExc_StopIteration, "Invalid alpha iterator");
    py::python_error();
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

}
} // namespace cgalpy // end of as2 namespace

void export_alpha_shape_2(py::module_& m) {
  using As2 = cgalpy::as2::Alpha_shape_2;
  using Tri2 = cgalpy::tri2::Triangulation_2;
  constexpr auto ri(py::rv_policy::reference_internal);

  cgalpy::as2::size_type (As2::*number_of_solid_components1)() const                     = &As2::number_of_solid_components;
  cgalpy::as2::size_type (As2::*number_of_solid_components2)(const cgalpy::as2::FT& alpha) const = &As2::number_of_solid_components;

  cgalpy::as2::Classification_type (As2::*classify1)(const cgalpy::as2::Point& p, const cgalpy::as2::FT& alpha) const              = &As2::classify;
  cgalpy::as2::Classification_type (As2::*classify2)(const cgalpy::as2::Edge& s, const cgalpy::as2::FT& alpha) const               = &As2::classify;
  cgalpy::as2::Classification_type (As2::*classify3)(const cgalpy::as2::Vertex_handle& s, const cgalpy::as2::FT& alpha) const      = &As2::classify;
  cgalpy::as2::Classification_type (As2::*classify4)(const cgalpy::as2::Face_handle& s, const cgalpy::as2::FT& alpha) const        = &As2::classify;
  cgalpy::as2::Classification_type (As2::*classify5)(const cgalpy::as2::Face_handle& s, int i, const cgalpy::as2::FT& alpha) const = &As2::classify;

  cgalpy::as2::Classification_type (As2::*classify6)(const cgalpy::as2::Point& p) const               = &As2::classify;
  cgalpy::as2::Classification_type (As2::*classify7)(const cgalpy::as2::Edge& s) const                = &As2::classify;
  cgalpy::as2::Classification_type (As2::*classify8)(const cgalpy::as2::Vertex_handle& s) const       = &As2::classify;
  cgalpy::as2::Classification_type (As2::*classify9)(const cgalpy::as2::Face_handle& s) const         = &As2::classify;
  cgalpy::as2::Classification_type (As2::*classify10)(const cgalpy::as2::Face_handle& s, int i) const = &As2::classify;

  py::class_<As2> as2_c(m, "Alpha_shape_2",
                           as2_doc::Alpha_shape_2_class);

  py::enum_<cgalpy::as2::Mode>(as2_c, "Mode",
                                  as2_doc::Alpha_shape_2_Mode)
    .value("GENERAL", As2::GENERAL)
    .value("REGULARIZED", As2::REGULARIZED)
    .export_values()
    ;

  py::enum_<cgalpy::as2::Classification_type>(
    as2_c, "Classification_type",
    as2_doc::Alpha_shape_2_Classification_type)
    .value("EXTERIOR", As2::EXTERIOR)
    .value("SINGULAR", As2::SINGULAR)
    .value("REGULAR", As2::REGULAR)
    .value("INTERIOR", As2::INTERIOR)
    .export_values()
    ;

  as2_c.def(py::init<>(), as2_doc::Alpha_shape_2_Alpha_shape_2)
    .def(py::init<double, cgalpy::as2::Mode>(),
         py::arg("alpha"), py::arg("mode") = As2::GENERAL,
         as2_doc::Alpha_shape_2_Alpha_shape_2)
    .def(py::init<cgalpy::as2::FT&, cgalpy::as2::Mode>(),
         py::arg("alpha"), py::arg("mode") = As2::GENERAL,
         as2_doc::Alpha_shape_2_Alpha_shape_2)
    .def(py::init<Tri2&, double, cgalpy::as2::Mode>(),
         py::arg("triangulation"), py::arg("alpha"),
         py::arg("mode") = As2::GENERAL,
         as2_doc::Alpha_shape_2_Alpha_shape_2_1)
    .def(py::init<Tri2&, cgalpy::as2::FT&, cgalpy::as2::Mode>(),
         py::arg("triangulation"), py::arg("alpha"),
         py::arg("mode") = As2::GENERAL,
         as2_doc::Alpha_shape_2_Alpha_shape_2_1)
    .def("__init__", &cgalpy::as2::as_init, py::arg("points"),
         as2_doc::Alpha_shape_2_Alpha_shape_2_2)
    .def("make_alpha_shape", &cgalpy::as2::make_alpha_shape,
         py::arg("points"),
         as2_doc::Alpha_shape_2_make_alpha_shape)
    .def("clear", &As2::clear,
         as2_doc::Alpha_shape_2_clear)
    .def("set_mode", &As2::set_mode, py::arg("mode") = As2::GENERAL,
         as2_doc::Alpha_shape_2_set_mode)
    .def("set_alpha", &As2::set_alpha, py::arg("alpha"),
         as2_doc::Alpha_shape_2_set_alpha)
    .def("get_alpha", &As2::get_alpha, ri,
         as2_doc::Alpha_shape_2_get_alpha)
    .def("get_nth_alpha", &As2::get_nth_alpha, ri, py::arg("n"),
         as2_doc::Alpha_shape_2_get_nth_alpha)
    .def("number_of_alphas", &As2::number_of_alphas,
         as2_doc::Alpha_shape_2_number_of_alphas)
    .def("get_mode", &As2::get_mode,
         as2_doc::Alpha_shape_2_get_mode)
    .def("number_of_solid_components", number_of_solid_components1,
         as2_doc::Alpha_shape_2_number_of_solid_components)
    .def("number_of_solid_components", number_of_solid_components2,
         py::arg("alpha"),
         as2_doc::Alpha_shape_2_number_of_solid_components)
    .def("find_optimal_alpha", &As2::find_optimal_alpha,
         py::arg("nb_components"),
         as2_doc::Alpha_shape_2_find_optimal_alpha)
    .def("classify", classify1, py::arg("p"), py::arg("alpha"),
         as2_doc::Alpha_shape_2_classify)
    .def("classify", classify2, py::arg("edge"), py::arg("alpha"),
         as2_doc::Alpha_shape_2_classify_2)
    .def("classify", classify3, py::arg("vertex"), py::arg("alpha"),
         as2_doc::Alpha_shape_2_classify_4)
    .def("classify", classify4, py::arg("face"), py::arg("alpha"),
         as2_doc::Alpha_shape_2_classify_1)
    .def("classify", classify5, py::arg("face"), py::arg("i"),
         py::arg("alpha"),
         as2_doc::Alpha_shape_2_classify_3)
    .def("classify", classify6, py::arg("p"),
         as2_doc::Alpha_shape_2_classify)
    .def("classify", classify7, py::arg("edge"),
         as2_doc::Alpha_shape_2_classify_2)
    .def("classify", classify8, py::arg("vertex"),
         as2_doc::Alpha_shape_2_classify_4)
    .def("classify", classify9, py::arg("face"),
         as2_doc::Alpha_shape_2_classify_1)
    .def("classify", classify10, py::arg("face"), py::arg("i"),
         as2_doc::Alpha_shape_2_classify_3)
    .def("alphas", &cgalpy::as2::alphas, py::keep_alive<0, 1>(),
         "Iterate over the sorted alpha-values of the family of alpha shapes.")
    .def("alpha_find", &As2::alpha_find, py::arg("alpha"),
         as2_doc::Alpha_shape_2_alpha_find)
    .def("alpha_lower_bound", &As2::alpha_lower_bound, py::arg("alpha"),
         as2_doc::Alpha_shape_2_alpha_lower_bound)
    .def("alpha_upper_bound", &As2::alpha_upper_bound, py::arg("alpha"),
         as2_doc::Alpha_shape_2_alpha_upper_bound)
    .def("alpha_shape_edges", &cgalpy::as2::alpha_shape_edges,
         "Return the edges that belong to the alpha shape for the current alpha.")
    .def("alpha_shape_vertices", &cgalpy::as2::alpha_shape_vertices,
         "Return the vertices that belong to the alpha shape for the current alpha.")
    ;

  using Ai = cgalpy::as2::Alpha_iterator;
  add_iterator<Ai, Ai>("Alpha_iterator", as2_c);

  // Alpha_shape_vertices_iterator;
  // Alpha_shape_edges_iterator;

  // Types that have been registered already:
  if (cgalpy::tri2::is_periodic())
    // \todo: generate bindings for periodic traits
    ;
  else {
    if (! add_attr<cgalpy::as2::Gt>(as2_c, "Gt"))
      std::cerr << "'cgalpy::as2::Gt' not registered!\n";
  }
  add_attr<cgalpy::as2::Point>(as2_c, "Point");
  if (is_exact_ft()) {
    if (! add_attr<cgalpy::as2::FT>(as2_c, "FT"))
      std::cerr << "'cgalpy::as2::FT' not registered!\n";
  }
  if (! add_attr<cgalpy::as2::Tds>(as2_c, "Tds"))
    std::cerr << "'cgalpy::as2::Tds' not registered!\n";
  if (! add_attr<cgalpy::as2::Vertex>(as2_c, "Vertex"))
    std::cerr << "'cgalpy::as2::Vertex' not registered!\n";
  if (! add_attr<cgalpy::as2::Edge>(as2_c, "Edge"))
    std::cerr << "'cgalpy::as2::Edge' not registered!\n";
  if (! add_attr<cgalpy::as2::Vertex_handle>(as2_c, "Vertex_handle"))
    std::cerr << "'cgalpy::as2::Vertex_handle' not registered!\n";
  if (! add_attr<cgalpy::as2::Face_handle>(as2_c, "Face_handle"))
    std::cerr << "'cgalpy::as2::Face_handle' not registered!\n";
}
