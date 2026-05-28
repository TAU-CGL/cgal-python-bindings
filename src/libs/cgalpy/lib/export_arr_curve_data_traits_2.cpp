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
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/Curve_data_merge.hpp"
#include "CGALPY/has_istream_operator.hpp"

namespace py = nanobind;

//!
template <typename T>
void export_extraction(T& xcv_c, std::false_type) {}

//!
template <typename T>
void export_extraction(T& xcv_c, std::true_type) { add_extraction(xcv_c); }

//
void export_arr_curve_data_traits_2(py::module_& m) {
  using Gt = cgalpy::aos2::Cd_geometry_traits_2;
  using Base_gt = Gt::Base_traits_2;
  using Cv = Gt::Curve_2;
  using Cv_data = Gt::Curve_data;
  using Base_cv = Base_gt::Curve_2;
  using Xcv_data = Gt::X_monotone_curve_data;
  using Xcv = Gt::X_monotone_curve_2;
  using Base_xcv = Base_gt::X_monotone_curve_2;
  using Cdm = cgalpy::aos2::Curve_data_merge;
  constexpr auto ri(py::rv_policy::reference_internal);

  /* Wrap the Curve_data_merge class. The interface is vased on static member
   * function and data members only. Thus, we do not wrap any constructor.On the
   * other hand, every time that two curves are merged, a new object os this
   * class is constructed, and the operator() is called immediately after. Thus,
   * wrapping a constructor is redundant and actually misleading.
   *
   * In any case, the use of static object makes this whole scheme limited. Once
   * a callback function is set by a user (that is, a Python script) it is
   * applied to all arrangement objects constructed by any script that imports
   * the same binding library.
   */
  if (! add_attr<Cdm>(m, "Curve_data_merge")) {
    py::class_<Cdm>(m, "Curve_data_merge")
      .def_static("reset_func", &Cdm::reset_func)
      .def_static("set_func", &Cdm::set_func)
      .def_prop_rw_static("func",
                          [](py::handle /*unused*/) { return Cdm::func() ; },
                          [](py::handle /*unused*/, py::object func) { Cdm::set_func(func); })
      ;
  }

  if (add_attr<Gt>(m, "Arr_curve_data_traits_2")) return;

  py::class_<Gt, Base_gt> traits_c(m, "Arr_curve_data_traits_2");
  traits_c.def(py::init<>());

  if (! add_attr<Cv>(traits_c, "Curve_2")) {
    py::class_<Cv, Base_cv>(traits_c, "Curve_2")
      .def(py::init<>())
      .def(py::init_implicit<const Base_cv&>())
      .def(py::init<const Base_cv&, Cv_data>())
      .def("data", py::overload_cast<>(&Cv::data, py::const_), ri)
      .def("set_data", &Cv::set_data)
      ;
  }

  if (! add_attr<Xcv>(traits_c, "X_monotone_curve_2")) {
    py::class_<Xcv, Base_xcv> xcv_c(traits_c, "X_monotone_curve_2");
    xcv_c.def(py::init<>())
      .def(py::init_implicit<const Base_xcv&>())
      .def(py::init<const Base_xcv&, Xcv_data>())
      .def("data", py::overload_cast<>(&Xcv::data, py::const_), ri)
      .def("set_data", &Xcv::set_data)
      ;

    add_insertion(xcv_c, "__str__");
    add_insertion(xcv_c, "__repr__");

    // Add extraction if import is supported
    export_extraction(xcv_c, typename has_istream_operator<Xcv>::type());
  }
}
