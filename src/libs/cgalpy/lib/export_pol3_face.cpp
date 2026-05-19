// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/export_circulator.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/polyhedron_3_types.hpp"
#include "docstrings/Polyhedron/docstrings.h"

namespace py = nanobind;
namespace doc = cgalpy::docstrings::Polyhedron;

namespace pol3 {

const Halfedge& face_halfedge(const Face& f) { return (*(f.halfedge())); }
const void face_set_halfedge(Face& f, Halfedge& h)
{ f.set_halfedge(Polyhedron_3::Halfedge_handle(&h)); }

//! Wrap the function that obtains the real circulator
auto halfedges_around_facet_circulator(const Face& f) { return f.facet_begin(); }

/*! Wrap an iterator
 * In case you wonder, there is no facet_end(), and facet_begin() indeed
 * returns a circulator
 */
auto halfedges_around_facet_iterator(const Face& f)
{ return make_iterator_from_circulator(f.facet_begin()); }

}

// Export Polyhedron Face
void export_pol3_face(py::class_<pol3::Polyhedron_3>& prn_c) {
  using Prn = pol3::Polyhedron_3;
  using Face = Prn::Face;
  using Plane_3 = Prn::Plane_3;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Face>(prn_c, "Face")) return;

  py::class_<Face> face_c(prn_c, "Face", doc::Polyhedron_3_Facet__class__);
  face_c.def(py::init<>())
    .def("facet_degree", [](const Face& f) { return f.facet_degree(); },
         doc::Polyhedron_3_Facet_facet_degree)
    .def("halfedge", &pol3::face_halfedge, ri,
         doc::Polyhedron_3_Facet_halfedge)
    .def("is_triangle", [](const Face& f) { return f.is_triangle(); },
         doc::Polyhedron_3_Facet_is_triangle)
    .def("is_quad", [](const Face& f) { return f.is_quad(); },
         doc::Polyhedron_3_Facet_is_quad)
    .def("set_halfedge", pol3::face_set_halfedge, py::arg("h"),
         doc::Polyhedron_3_Facet_set_halfedge)
    .def("halfedges", &pol3::halfedges_around_facet_iterator, py::keep_alive<0, 1>(),
         doc::Polyhedron_3_Facet_facet_begin)

    //! Wrap the function that returns the real circulator
    .def("halfedges_circulator", &pol3::halfedges_around_facet_circulator,
         doc::Polyhedron_3_Facet_Halfedge_around_facet_circulator)

#if CGALPY_POL3_GEOMETRY_TRAITS == CGALPY_POL3_WITH_NORMALS_GEOMETRY_TRAITS
    .def("normal", [](const Face& f) { return f.plane(); }, ri,
         doc::Polyhedron_3_Facet_plane)
    .def("set_normal", [](Face& f, const Vector_3& normal) { f.plane() = normal; },
         py::arg("normal"), doc::Polyhedron_3_Facet_plane)
#else
    .def("plane", [](const Face& f) { return f.plane(); }, ri,
         doc::Polyhedron_3_Facet_plane)
    .def("set_plane", [](Face& f, const Plane_3& plane) { f.plane() = plane; },
         py::arg("plane"), doc::Polyhedron_3_Facet_plane)
#endif

#ifdef CGALPY_POL3_FACE_EXTENDED
    .def("set_data", [](Face& f, py::object obj) { f.set_data(obj); })
    .def("data", [](const Face& f)->py::object { return f.data(); })
#endif
    ;

  using Hafcc = pol3::Halfedge_around_facet_const_circulator;
  add_iterator_from_circulator<Hafcc>("Halfedge_around_facet_iterator", face_c);

  //! Wrap a circulator
  export_circulator<Hafcc>(face_c, "Halfedge_around_facet_circulator");

#if defined(CGALPY_POL3_FACE_WITH_ID)
  if constexpr(pol3::face_with_id())
    face_c.def("id", [](const Face& f){ return f.id(); });
#endif
}
