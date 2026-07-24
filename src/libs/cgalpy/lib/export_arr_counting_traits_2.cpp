// Copyright (c) 2024 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>

#include <CGAL/Arr_counting_traits_2.h>

#include "cgalpy/arrangement_on_surface_2_types.hpp"
#include "cgalpy/add_attr.hpp"
#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_doc = cgalpy::aos2::docstrings;

//
void export_arr_counting_traits_2(py::module_& m) {
  using Gt = cgalpy::aos2::Cnt_geometry_traits_2;

  if (add_attr<Gt>(m, "Arr_counting_traits_2")) return;

#if CGAL_VERSION_NR >= 1060300900
  using Shared_base = Gt::Shared_base;
  py::class_<Gt> traits_c(m, "Arr_counting_traits_2",
                          aos2_doc::Arr_counting_traits_2_class);
#else
  using Base_gt = Gt::Base;
  py::class_<Gt, Base_gt> traits_c(m, "Arr_counting_traits_2",
                                   aos2_doc::Arr_counting_traits_2_class);
#endif
  traits_c.def(py::init<>(),
               aos2_doc::Arr_counting_traits_2_Arr_counting_traits_2)
#if CGAL_VERSION_NR >= 1060300900
    .def(py::init<Shared_base>(), py::arg("traits"),
         aos2_doc::Arr_counting_traits_2_Arr_counting_traits_2_1)
    .def("shared_traits", &Gt::shared_traits,
         aos2_doc::Arr_counting_traits_2_shared_traits)
#endif
    ;
}
