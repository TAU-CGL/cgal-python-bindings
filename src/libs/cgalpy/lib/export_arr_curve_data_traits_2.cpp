// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Nir Goren         <nirgoren@mail.tau.ac.il>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_curve_data_traits_2.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

void export_arr_curve_data_traits_2(py::module_& m) {
  using Gt = aos2::Cd_geometry_traits_2;
  using Base_gt = Gt::Base_traits_2;
  using Cv = Gt::Curve_2;
  using Cv_data = Gt::Curve_data;
  using Base_cv = Base_gt::Curve_2;
  using Xcv_data = Gt::X_monotone_curve_data;
  using Xcv = Gt::X_monotone_curve_2;
  using Base_xcv = Base_gt::X_monotone_curve_2;

  if (add_attr<Gt>(m, "Arr_curve_data_traits_2")) return;

  py::class_<Gt, Base_gt> traits_c(m, "Arr_curve_data_traits_2");
  traits_c.def(py::init<>());

  if (! add_attr<Cv>(traits_c, "Curve_2")) {
    py::class_<Cv, Base_cv>(traits_c, "Curve_2")
      .def(py::init<>())
      .def(py::init_implicit<const Base_cv&>())
      .def(py::init<const Base_cv&, Cv_data>())
      .def("data", py::overload_cast<>(&Cv::data, py::const_))
      .def("set_data", &Cv::set_data)
      ;
  }

  if (! add_attr<Xcv>(traits_c, "X_monotone_curve_2")) {
    py::class_<Xcv, Base_xcv>(traits_c, "X_monotone_curve_2")
      .def(py::init<>())
      .def(py::init_implicit<const Base_xcv&>())
      .def(py::init<const Base_xcv&, Xcv_data>())
      .def("data", py::overload_cast<>(&Xcv::data, py::const_))
      .def("set_data", &Xcv::set_data)
      ;
  }
}
