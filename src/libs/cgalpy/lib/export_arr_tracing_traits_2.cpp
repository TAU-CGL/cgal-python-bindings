// Copyright (c) 2024 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_tracing_traits_2.h>

#include "cgalpy/arrangement_on_surface_2_types.hpp"
#include "cgalpy/add_attr.hpp"
#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_doc = cgalpy::aos2::docstrings;

//
void export_arr_tracing_traits_2(py::module_& m) {
  using Gt = cgalpy::aos2::Trc_geometry_traits_2;
  using Base_gt = Gt::Base;

  if (add_attr<Gt>(m, "Arr_tracing_traits_2")) return;

  py::class_<Gt, Base_gt> traits_c(m, "Arr_tracing_traits_2",
                                      aos2_doc::Arr_tracing_traits_2_class);

  // Do not use add_attr() for enums, as py::type_check(info) is false.
  const py::handle info_op_id = py::type<Gt::Operation_id>();
  if (info_op_id.is_valid()) traits_c.attr("Operation_id") = info_op_id;
  else {
    py::enum_<Gt::Operation_id>(traits_c, "Operation_id", py::is_arithmetic())
      .value("COMPARE_X_2_OP", Gt::COMPARE_X_2_OP)
      .value("COMPARE_XY_2_OP", Gt::COMPARE_XY_2_OP)
      .value("CONSTRUCT_MIN_VERTEX_2_OP", Gt::CONSTRUCT_MIN_VERTEX_2_OP)
      .value("CONSTRUCT_MAX_VERTEX_2_OP", Gt::CONSTRUCT_MAX_VERTEX_2_OP)
      .value("IS_VERTICAL_2_OP", Gt::IS_VERTICAL_2_OP)
      .value("COMPARE_Y_AT_X_2_OP", Gt::COMPARE_Y_AT_X_2_OP)
      .value("EQUAL_POINTS_2_OP", Gt::EQUAL_POINTS_2_OP)
      .value("EQUAL_CURVES_2_OP", Gt::EQUAL_CURVES_2_OP)
      .value("COMPARE_Y_AT_X_LEFT_2_OP", Gt::COMPARE_Y_AT_X_LEFT_2_OP)
      .value("COMPARE_Y_AT_X_RIGHT_2_OP", Gt::COMPARE_Y_AT_X_RIGHT_2_OP)
      .value("MAKE_X_MONOTONE_2_OP", Gt::MAKE_X_MONOTONE_2_OP)
      .value("SPLIT_2_OP", Gt::SPLIT_2_OP)
      .value("INTERSECT_2_OP", Gt::INTERSECT_2_OP)
      .value("ARE_MERGEABLE_2_OP", Gt::ARE_MERGEABLE_2_OP)
      .value("MERGE_2_OP", Gt::MERGE_2_OP)
      .value("CONSTRUCT_2_OPPOSITE_2_OP", Gt::CONSTRUCT_2_OPPOSITE_2_OP)
      .value("COMPARE_ENDPOINTS_XY_2_OP", Gt::COMPARE_ENDPOINTS_XY_2_OP)
      .value("APPROXIMATE_2_OP", Gt::APPROXIMATE_2_OP)
      .value("PARAMETER_SPACE_IN_X_2_OP", Gt::PARAMETER_SPACE_IN_X_2_OP)
      .value("IS_ON_X_IDENTIFICATION_2_OP", Gt::IS_ON_X_IDENTIFICATION_2_OP)
      .value("COMPARE_Y_ON_BOUNDARY_2_OP", Gt::COMPARE_Y_ON_BOUNDARY_2_OP)
      .value("COMPARE_Y_NEAR_BOUNDARY_2_OP", Gt::COMPARE_Y_NEAR_BOUNDARY_2_OP)
      .value("PARAMETER_SPACE_IN_Y_2_OP", Gt::PARAMETER_SPACE_IN_Y_2_OP)
      .value("IS_ON_Y_IDENTIFICATION_2_OP", Gt::IS_ON_Y_IDENTIFICATION_2_OP)
      .value("COMPARE_X_ON_BOUNDARY_2_OP", Gt::COMPARE_X_ON_BOUNDARY_2_OP)
      .value("COMPARE_X_NEAR_BOUNDARY_2_OP", Gt::COMPARE_X_NEAR_BOUNDARY_2_OP)
      .value("NUMBER_OF_OPERATIONS", Gt::NUMBER_OF_OPERATIONS)
      .export_values()
      ;
  }

  traits_c.def(py::init<>(),
               aos2_doc::Arr_tracing_traits_2_Arr_tracing_traits_2)
    .def("disable_all_traces", &Gt::disable_all_traces,
         aos2_doc::Arr_tracing_traits_2_disable_all_traces)
    .def("enable_trace", &Gt::enable_trace, py::arg("id"),
         aos2_doc::Arr_tracing_traits_2_enable_trace)
    ;
}
