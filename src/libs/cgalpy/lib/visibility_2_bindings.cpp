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
#include "CGALPY/arrangement_on_surface_2_types.hpp"

namespace py = nanobind;

namespace vis2 {

const Face& compute_visibility1(Simple_polygon_visibility_2& spv,
                                const Point_2& q, const Face& f,
                                Arrangement_2& arr)
{ return *(spv.compute_visibility(q, Face_const_handle(&f), arr)); }

const Face& compute_visibility2(Simple_polygon_visibility_2& spv,
                                const Point_2& q, const Halfedge& h,
                                Arrangement_2& arr)
{ return *(spv.compute_visibility(q, Halfedge_const_handle(&h), arr)); }

}

void export_visibility_2(py::module_& m) {
  using Spv = vis2::Simple_polygon_visibility_2;
  using Arr = vis2::Arrangement_2;
  using Point = vis2::Point_2;
  using Face = vis2::Face;

  py::class_<Spv> spv_c(m, "Simple_polygon_visibility_2");
  spv_c.def(py::init<>())
    .def(py::init<const Arr&>())
    .def("is_attached", &Spv::is_attached)
    .def("attach", &Spv::attach)
    .def("detach", &Spv::detach)
    .def("arrangement_2", &Spv::arrangement_2)
    .def("compute_visibility", vis2::compute_visibility1)
    .def("compute_visibility", vis2::compute_visibility2)
    ;
  // non_regular_visibility


}
