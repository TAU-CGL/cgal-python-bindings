// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/nanobind_object_equality.hpp"

#include <CGAL/Arr_curve_data_traits_2.h>
#include <CGAL/Arr_consolidated_curve_data_traits_2.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/make_iterator.hpp"
#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_doc = cgalpy::aos2::docstrings;

namespace cgalpy {
namespace aos2 {

template <typename C>
py::object items(const C& c) { return make_iterator(c.begin(), c.end()); }

}
} // namespace cgalpy

void export_arr_consolidated_curve_data_traits_2(py::module_& m) {
  using Gt = cgalpy::aos2::Ccd_geometry_traits_2;
  using Base_gt = cgalpy::aos2::Cd_geometry_traits_2;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (! add_attr<Gt>(m, "Arr_consolidated_curve_data_traits_2")) {
    py::class_<Gt, Base_gt> ccdt_c(
      m, "Arr_consolidated_curve_data_traits_2",
      aos2_doc::Arr_consolidated_curve_data_traits_2_class);
    ccdt_c.def(py::init<>(),
               "Construct a default consolidated curve-data traits object.");
  }

  using Data = Gt::Data;
  using Ul = CGAL::_Unique_list<Data>;
  using Cul = CGAL::_Consolidate_unique_lists<Data>;

  if (! add_attr<Ul>(m, "Unique_list")) {
    py::class_<Ul> ul_c(
      m, "Unique_list",
      aos2_doc::Arr_consolidated_curve_data_traits_2_Data_container_class);
    ul_c.def(py::init<>(),
             aos2_doc::Arr_consolidated_curve_data_traits_2_Data_container_Data_container)
      .def(py::init<const Data&>(), py::arg("data"),
           aos2_doc::Arr_consolidated_curve_data_traits_2_Data_container_Data_container_1)
      .def("size", &Ul::size,
           aos2_doc::Arr_consolidated_curve_data_traits_2_Data_container_size)
      .def("front", &Ul::front, ri,
           aos2_doc::Arr_consolidated_curve_data_traits_2_Data_container_front)
      .def("back", &Ul::back, ri,
           aos2_doc::Arr_consolidated_curve_data_traits_2_Data_container_back)
      .def("items", &cgalpy::aos2::items<Ul>, py::keep_alive<0, 1>(),
           "Return an iterator over the unique data items.")
      ;

    using It = Ul::const_iterator;
    add_iterator<It, It>("iterator", ul_c);
  }
}
