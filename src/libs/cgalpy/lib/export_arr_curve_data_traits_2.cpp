// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_curve_data_traits_2.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

void export_arr_curve_data_traits_2(py::module_& m) {
  using Agt = aos2::Agt;
  using Cgt = aos2::Cgt;
  using Base_cv = Agt::Curve_2;
  using Cv = Cgt::Curve_2;
  using Base_xcv = Agt::X_monotone_curve_2;
  using Xcv = Cgt::X_monotone_curve_2;

  if (add_attr<Cgt>(m, "Arr_curve_data_traits_2")) return;

  py::class_<Cgt, Agt> traits_c(m, "Arr_curve_data_traits_2");
  traits_c.def(py::init<>());

  if (! add_attr<Cv>(traits_c, "Curve_2")) {
    py::class_<Cv, Base_cv>(traits_c, "Curve_2")
      .def(py::init<>())
      .def(py::init<const Base_cv&>())
      .def(py::init<const Base_cv&, py::object>())
      .def("data", py::overload_cast<>(&Cv::data, py::const_))
      .def("set_data", &Cv::set_data)
      ;
  }

  if (! add_attr<Xcv>(traits_c, "X_monotone_curve_2")) {
    py::class_<Xcv, Base_xcv>(traits_c, "X_monotone_curve_2")
      .def(py::init<>())
      .def(py::init<const Base_xcv&>())
      .def(py::init<const Base_xcv&, py::object>())
      .def("data", py::overload_cast<>(&Xcv::data, py::const_))
      .def("set_data", &Xcv::set_data)
      ;
  }
}
