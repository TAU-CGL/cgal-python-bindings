// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

namespace py = nanobind;

#include "CGALPY/add_attr.hpp"
#include "CGALPY/triangulation_d_types.hpp"

namespace trid {
}

//!
void export_trid_vertex(py::class_<trid::Triangulation_d>& tri_c) {
  using Fc = trid::Full_cell;
  using Point = trid::Point;
  using Vertex = trid::Vertex;

  if (add_attr<Vertex>(tri_c, "Vertex")) return;

  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Vertex>(tri_c, "Vertex")
    .def(py::init<const Point&>())
    .def("point", &Vertex::point, ri)
    .def("set_point", &Vertex::set_point)
    .def("full_cell", [](const Vertex& v)->Fc& { return *(v.full_cell()); }, ri)
    .def("set_full_cell",
         [](Vertex& v, Fc& c)->void
         {v.set_full_cell(trid::Full_cell_handle(&c)); })
    .def("is_valid",
         [](const Vertex& v, bool verbose, int level)->bool
         { return v.is_valid(verbose, level); },
         py::arg("verbose") = false, py::arg("level") = 0)

#ifdef CGALPY_TRID_VERTEX_WITH_DATA
    .def("data", py::overload_cast<>(&Vertex::data, py::const_), ri)
    .def("set_data",
         [](Vertex& v, py::object data)->void{ v.data() = data; })
#endif
    ;
}
