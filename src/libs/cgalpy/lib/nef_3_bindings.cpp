// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>

//! \todo why is this needed?
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <CGAL/convex_decomposition_3.h>

#ifdef CGALPY_HAS_VISUAL
#include <CGAL/draw_nef_3.h>
#endif

#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/nef_3_types.hpp"

namespace py = nanobind;

namespace nef3 {

//!
auto my_volumes(Nef_polyhedron_3& np)
{ return make_iterator(np.volumes_begin(), np.volumes_end()); }

//!
Polygonal_mesh
convert_inner_shell_to_polyhedron(Nef_polyhedron_3& np,
                                  const Nef_polyhedron_3::Volume& volume) {
  Nef_polyhedron_3::Shell_entry_const_iterator it = volume.shells_begin();
  Polygonal_mesh pm;
  np.convert_inner_shell_to_polyhedron(it, pm);
  return pm;
}

}

void export_nef_3(py::module_& m) {
  using Pm = nef3::Polygonal_mesh;
  using Np3 = nef3::Nef_polyhedron_3;
  using Boundary = Np3::Boundary;
  using Content = Np3::Content;
  using Im = Np3::Intersection_mode;
  using Volume = Np3::Volume;
  using Mark = Np3::Mark;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (! add_attr<Np3>(m, "Nef_polyhedron_3")) {
    py::class_<Np3> np3_c(m, "Nef_polyhedron_3");

    const py::handle info_boundary = py::type<Boundary>();
    if (info_boundary.is_valid()) m.attr("Boundary") = info_boundary;
    else {
      py::enum_<Boundary>(m, "Boundary")
        .value("EXCLUDED", Np3::EXCLUDED)
        .value("INCLUDED", Np3::INCLUDED)
        .export_values()
        ;
    }

    const py::handle info_content = py::type<Content>();
    if (info_content.is_valid()) m.attr("Content") = info_content;
    else {
      py::enum_<Content>(m, "Content")
        .value("EMPTY", Np3::EMPTY)
        .value("COMPLETE", Np3::COMPLETE)
        .export_values()
        ;
    }

    const py::handle info_intersection_mode = py::type<Im>();
    if (info_intersection_mode.is_valid()) m.attr("Im") = info_intersection_mode;
    else {
      py::enum_<Im>(m, "Im")
        .value("CLOSED_HALFSPACE", Np3::CLOSED_HALFSPACE)
        .value("OPEN_HALFSPACE", Np3::OPEN_HALFSPACE)
        .value("PLANE_ONLY", Np3::PLANE_ONLY)
        .export_values()
        ;
    }

    if (! add_attr<Volume>(np3_c, "Volume")) {
      py::class_<Volume>(np3_c, "Volume")
        .def(py::init<>())
        .def("mark", [](Volume& vol) -> Mark& { return vol.mark(); }, ri)
        ;
    }

    using Vci = Np3::Volume_const_iterator;
    add_iterator<Vci, Vci, const Volume&>("Volume_iterator", np3_c);

    np3_c.def(py::init<>())
      .def(py::init<Content>(), py::arg("content") = Content::EMPTY)
      .def(py::init<const Pm&>(), py::arg("pm"))
      .def("volumes", &nef3::my_volumes, py::keep_alive<0, 1>())
      .def("convert_inner_shell_to_polyhedron",
           &nef3::convert_inner_shell_to_polyhedron)
    ;

  }

  m.def("convex_decomposition_3", &CGAL::convex_decomposition_3<Np3>);
}
