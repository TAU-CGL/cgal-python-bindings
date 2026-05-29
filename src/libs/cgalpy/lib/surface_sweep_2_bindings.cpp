// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Sweep_line_2_algorithms.h>

#include "CGALPY/surface_sweep_2_types.hpp"
#include "CGALPY/stl_forward_iterator.hpp"

#include "cgalpy/Ss2_docstrings.hpp"

namespace py = nanobind;
namespace ss2_doc = cgalpy::ss2::docstrings;

namespace cgalpy {
namespace ss2 {

// Binding for computing all intersection points induced by a range of input curves.
py::list compute_intersection_points0(py::list& curves, bool report_endpoints = false) {
  py::list points;
  if (curves.size() == 0) return points;
  auto op = [&] (const Point_2& p) mutable { points.append(p); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  if (py::isinstance<X_monotone_curve_2>(curves[0])) {
    auto begin = stl_forward_iterator<X_monotone_curve_2>(curves);
    auto end = stl_forward_iterator<X_monotone_curve_2>(curves, false);
    CGAL::compute_intersection_points(begin, end, it, report_endpoints);
  }
  else if (py::isinstance<Curve_2>(curves[0])) {
    auto begin = stl_forward_iterator<Curve_2>(curves);
    auto end = stl_forward_iterator<Curve_2>(curves, false);
    CGAL::compute_intersection_points(begin, end, it, report_endpoints);
  }
  else {
    PyErr_SetString(PyExc_StopIteration, "Invalid curve iterator");
    py::python_error();
  }
  return points;
}

// Binding for computing all intersection points induced by a range of input curves.
py::list compute_intersection_points1(py::list& curves, bool report_endpoints, const Geometry_traits_2& traits) {
  py::list points;
  if (curves.size() == 0) return points;
  auto op = [&] (const Point_2& p) mutable { points.append(p); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  if (py::isinstance<X_monotone_curve_2>(curves[0])) {
    auto begin = stl_forward_iterator<X_monotone_curve_2>(curves);
    auto end = stl_forward_iterator<X_monotone_curve_2>(curves, false);
    CGAL::compute_intersection_points(begin, end, it, report_endpoints, traits);
  }
  else if (py::isinstance<Curve_2>(curves[0])) {
    auto begin = stl_forward_iterator<Curve_2>(curves);
    auto end = stl_forward_iterator<Curve_2>(curves, false);
    CGAL::compute_intersection_points(begin, end, it, report_endpoints, traits);
  }
  else {
    PyErr_SetString(PyExc_StopIteration, "Invalid curve iterator");
    py::python_error();
  }
  return points;
}

// Binding for computing all x-monotone subcurves that are disjoint in their interiors
// induced by a range of input curves.
py::list compute_subcurves0(py::list& curves, bool mult_overlaps = false) {
  py::list subcurves;
  if (curves.size() == 0) return subcurves;
  auto op = [&] (const X_monotone_curve_2& xcv) mutable { subcurves.append(xcv); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  if (py::isinstance<X_monotone_curve_2>(curves[0])) {
    auto begin = stl_forward_iterator<X_monotone_curve_2>(curves);
    auto end = stl_forward_iterator<X_monotone_curve_2>(curves, false);
    CGAL::compute_subcurves(begin, end, it, mult_overlaps);
  }
  else if (py::isinstance<Curve_2>(curves[0])) {
    auto begin = stl_forward_iterator<Curve_2>(curves);
    auto end = stl_forward_iterator<Curve_2>(curves, false);
    CGAL::compute_subcurves(begin, end, it, mult_overlaps);
  }
  else {
    PyErr_SetString(PyExc_StopIteration, "Invalid curve iterator");
    py::python_error();
  }
  return subcurves;
}

// Binding for computing all x-monotone subcurves that are disjoint in their interiors
// induced by a range of input curves.
py::list compute_subcurves1(py::list& curves, bool mult_overlaps, const Geometry_traits_2& traits) {
  py::list subcurves;
  if (curves.size() == 0) return subcurves;
  auto op = [&] (const X_monotone_curve_2& xcv) mutable { subcurves.append(xcv); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  if (py::isinstance<X_monotone_curve_2>(curves[0])) {
    auto begin = stl_forward_iterator<X_monotone_curve_2>(curves);
    auto end = stl_forward_iterator<X_monotone_curve_2>(curves, false);
    CGAL::compute_subcurves(begin, end, it, mult_overlaps, traits);
  }
  else if (py::isinstance<Curve_2>(curves[0])) {
    auto begin = stl_forward_iterator<Curve_2>(curves);
    auto end = stl_forward_iterator<Curve_2>(curves, false);
    CGAL::compute_subcurves(begin, end, it, mult_overlaps, traits);
  }
  else {
    PyErr_SetString(PyExc_StopIteration, "Invalid curve iterator");
    py::python_error();
  }
  return subcurves;
}

// Binding for determining whether an intersection occurs between any pair
// of curves in a given range.
bool do_intersect0(py::list& curves, bool consider_common_endpoints = true) {
  // CGAL 6.1 exposes this algorithm as CGAL::do_curves_intersect().
  // Newer CGAL headers expose CGAL::Surface_sweep_2::do_intersect(), but the
  // current build include order uses the 6.1 Homebrew header. Keep the Python
  // argument for API compatibility; the older CGAL function has no endpoint flag.
  (void)consider_common_endpoints;

  if (py::isinstance<X_monotone_curve_2>(curves[0])) {
    auto begin = stl_forward_iterator<X_monotone_curve_2>(curves);
    auto end = stl_forward_iterator<X_monotone_curve_2>(curves, false);
    return CGAL::do_curves_intersect(begin, end);
  }
  else if (py::isinstance<Curve_2>(curves[0])) {
    auto begin = stl_forward_iterator<Curve_2>(curves);
    auto end = stl_forward_iterator<Curve_2>(curves, false);
    return CGAL::do_curves_intersect(begin, end);
  }
  else {
    PyErr_SetString(PyExc_StopIteration, "Invalid curve iterator");
    py::python_error();
  }
  return false;
}

// Binding for determining whether an intersection occurs between any pair
// of curves in a given range.
bool do_intersect1(py::list& curves, bool consider_common_endpoints, const Geometry_traits_2& traits) {
  // See do_intersect0() above. The older CGAL function has no endpoint flag.
  (void)consider_common_endpoints;

  if (py::isinstance<X_monotone_curve_2>(curves[0])) {
    auto begin = stl_forward_iterator<X_monotone_curve_2>(curves);
    auto end = stl_forward_iterator<X_monotone_curve_2>(curves, false);
    return CGAL::do_curves_intersect(begin, end, traits);
  }
  else if (py::isinstance<Curve_2>(curves[0])) {
    auto begin = stl_forward_iterator<Curve_2>(curves);
    auto end = stl_forward_iterator<Curve_2>(curves, false);
    return CGAL::do_curves_intersect(begin, end, traits);
  }
  else {
    PyErr_SetString(PyExc_StopIteration, "Invalid curve iterator");
    py::python_error();
  }
  return false;
}

}
} // namespace cgalpy

//
void export_surface_sweep_2(py::module_& m) {
  m.def("compute_intersection_points", &cgalpy::ss2::compute_intersection_points0,
        py::arg("curves"), py::arg("report_endpoints") = false,
        ss2_doc::compute_intersection_points)
    .def("compute_intersection_points", &cgalpy::ss2::compute_intersection_points1,
         py::arg("curves"), py::arg("report_endpoints"), py::arg("traits"),
         ss2_doc::compute_intersection_points_1)
    ;

  m.def("compute_subcurves", &cgalpy::ss2::compute_subcurves0,
        py::arg("curves"), py::arg("mult_overlaps") = false,
        ss2_doc::compute_subcurves)
    .def("compute_subcurves", &cgalpy::ss2::compute_subcurves1,
         py::arg("curves"), py::arg("mult_overlaps"), py::arg("traits"),
         ss2_doc::compute_subcurves_1)
    ;

  m.def("do_intersect", &cgalpy::ss2::do_intersect0,
        py::arg("curves"), py::arg("consider_common_endpoints") = true,
        ss2_doc::Surface_sweep_2_do_intersect)
    .def("do_intersect", &cgalpy::ss2::do_intersect1,
         py::arg("curves"), py::arg("consider_common_endpoints"),
         py::arg("traits"), ss2_doc::Surface_sweep_2_do_intersect_1)
    ;
}
