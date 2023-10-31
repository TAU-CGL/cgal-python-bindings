// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Env_surface_data_traits_3.h>

#include "CGALPY/envelope_3_types.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

//
template <typename T>
py::list mk_xy_monotone_3_call_operator(const typename T::Make_xy_monotone_3& m,
                                        const typename T::Surface_3& s,
                                        bool is_lower) {
  py::list res;
  return res;
}

//
void export_env_surface_data_traits_3(py::module_& m) {
  using Egt = env3::Egt;
  if (add_attr<Egt>(m, "Env_surface_data_traits_3")) return;

  using Mk_xy_monotone_3 = Egt::Make_xy_monotone_3;

  py::class_<Mk_xy_monotone_3>(m, "Make_xy_monotone_3")
    .def("__call__", &mk_xy_monotone_3_call_operator<Egt>)
    ;

  using Egt_base = env3::Egt_base;
  py::class_<Egt, Egt_base> traits_c(m, "Env_surface_data_traits_3");
  traits_c.def(py::init<>())
    .def(py::init<env3::Egt_base>())
    .def("make_xy_monotone_3_object", &Egt::make_xy_monotone_3_object)
    ;

  using Base_srf = Egt_base::Surface_3;
  using Srf = Egt::Surface_3;

  if (! add_attr<Srf>(traits_c, "Surface_3")) {
    py::class_<Srf, Base_srf>(traits_c, "Surface_3")
      .def(py::init<>())
      .def(py::init<const Base_srf&>())
      .def(py::init<const Base_srf&, py::object>())
      .def("data", py::overload_cast<>(&Srf::data, py::const_))
      .def("set_data", &Srf::set_data)
      ;
  }
}
