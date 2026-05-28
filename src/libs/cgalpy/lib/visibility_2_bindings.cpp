// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/visibility_2_types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"

#include "cgalpy/Vis2_docstrings.hpp"

namespace py = nanobind;
namespace vis2_doc = cgalpy::vis2::docstrings;

namespace cgalpy {
namespace vis2 {

template <typename Visibility>
const Face& compute_visibility1(Visibility& vis,
                                const Point_2& q, const Face& f,
                                Arrangement_2& arr)
{ return *(vis.compute_visibility(q, Face_const_handle(&f), arr)); }

template <typename Visibility>
const Face& compute_visibility2(Visibility& vis,
                                const Point_2& q, const Halfedge& h,
                                Arrangement_2& arr)
{ return *(vis.compute_visibility(q, Halfedge_const_handle(&h), arr)); }

}
} // namespace cgalpy

void export_visibility_2(py::module_& m) {
  using Spv = cgalpy::vis2::Simple_polygon_visibility_2;
  using Arr = cgalpy::vis2::Arrangement_2;
  using Point = cgalpy::vis2::Point_2;
  using Face = cgalpy::vis2::Face;

  if (! add_attr<Spv>(m, "Simple_polygon_visibility_2")) {
    py::class_<Spv> spv_c(m, "Simple_polygon_visibility_2",
                          vis2_doc::Simple_polygon_visibility_2_class);
    spv_c.def(py::init<>(), vis2_doc::Visibility_2_Visibility_2)
      .def(py::init<const Arr&>(), py::arg("arr"),
           vis2_doc::Visibility_2_Visibility_2_1)
      .def("is_attached", &Spv::is_attached,
           vis2_doc::Visibility_2_is_attached)
      .def("attach", &Spv::attach, py::arg("arr"),
           vis2_doc::Simple_polygon_visibility_2_attach)
      .def("detach", &Spv::detach,
           vis2_doc::Visibility_2_detach)
      .def("arrangement_2", &Spv::arrangement_2,
           vis2_doc::Visibility_2_arrangement_2)
      .def("compute_visibility", cgalpy::vis2::compute_visibility1<Spv>,
           py::arg("q"), py::arg("face"), py::arg("out_arr"),
           vis2_doc::Visibility_2_compute_visibility)
      .def("compute_visibility", cgalpy::vis2::compute_visibility2<Spv>,
           py::arg("q"), py::arg("halfedge"), py::arg("out_arr"),
           vis2_doc::Visibility_2_compute_visibility_1)
      ;
  }

  using Tev = cgalpy::vis2::Triangular_expansion_visibility_2;
  if (! add_attr<Tev>(m, "Triangular_expansion_visibility_2")) {
    py::class_<Tev> tev_c(m, "Triangular_expansion_visibility_2",
                          vis2_doc::Triangular_expansion_visibility_2_class);
    tev_c.def(py::init<>(), vis2_doc::Visibility_2_Visibility_2)
      .def(py::init<const Arr&>(), py::arg("arr"),
           vis2_doc::Visibility_2_Visibility_2_1)
      .def("is_attached", &Tev::is_attached,
           vis2_doc::Visibility_2_is_attached)
      .def("attach", &Tev::attach, py::arg("arr"),
           vis2_doc::Triangular_expansion_visibility_2_attach)
      .def("detach", &Tev::detach,
           vis2_doc::Visibility_2_detach)
      .def("arrangement_2", &Tev::arrangement_2,
           vis2_doc::Visibility_2_arrangement_2)
      .def("compute_visibility", cgalpy::vis2::compute_visibility1<Tev>,
           py::arg("q"), py::arg("face"), py::arg("out_arr"),
           vis2_doc::Visibility_2_compute_visibility)
      .def("compute_visibility", cgalpy::vis2::compute_visibility2<Tev>,
           py::arg("q"), py::arg("halfedge"), py::arg("out_arr"),
           vis2_doc::Visibility_2_compute_visibility_1)
      ;
  }
}
