// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <nanobind/nanobind.h>

#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/draw_polyhedron.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/Kernel/export_point_3.hpp"
#include "CGALPY/polyhedron_3_types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

namespace pol3 {

// Read a surface mesh from a file.
template <typename Polyhedron_3>
Polyhedron_3 read_polygon_mesh(const std::string& filename) {
  using Prn = Polyhedron_3;

  Prn pol;
  if (! CGAL::IO::read_polygon_mesh(filename, pol))
    throw std::runtime_error("Cannot read file!");
  return pol;
}

// Draw a polyhedron.
void draw(const Polyhedron_3& prn) { CGAL::draw(prn); }

//
Halfedge& make_tetrahedron1(Polyhedron_3& prn,
                            const Point_3& p1, const Point_3& p2,
                            const Point_3& p3, const Point_3& p4)
{ return *(prn.make_tetrahedron(p1, p2, p3, p4)); }

//
Halfedge& make_tetrahedron2(Polyhedron_3& prn)
{ return *(prn.make_tetrahedron()); }

//
bool is_tetrahedron(const Polyhedron_3& prn, const Halfedge& h)
{ return prn.is_tetrahedron(Halfedge_const_handle(&h)); }

}

// Export Vertex
template <typename C>
void export_vertex(C& prn_c) {
  using Prn = pol3::Polyhedron_3;
  using Vertex = Prn::Vertex;

  if (! add_attr<Vertex>(prn_c, "Vertex")) {
    py::class_<Vertex> vertex_c(prn_c, "Vertex");
    vertex_c.def(py::init<>())
      ;
  }
}

// Export Vertex
template <typename C>
void export_halfedge(C& prn_c) {
  using Prn = pol3::Polyhedron_3;
  using Halfedge = Prn::Halfedge;

  if (! add_attr<Halfedge>(prn_c, "Halfedge")) {
    py::class_<Halfedge> halfedge_c(prn_c, "Halfedge");
    halfedge_c.def(py::init<>())
      ;
  }
}

// Export Vertex
template <typename C>
void export_facet(C& prn_c) {
  using Prn = pol3::Polyhedron_3;
  using Face = Prn::Face;

  if (! add_attr<Face>(prn_c, "Face")) {
    py::class_<Face> face_c(prn_c, "Face");
    face_c.def(py::init<>())
      ;
  }
}

// Export Polyhedron_3.
void export_polyhedron_3(py::module_& m) {
  using Prn = pol3::Polyhedron_3;
  using Pnt = Prn::Point_3;
  using Vertex = Prn::Vertex;
  using Halfedge = Prn::Halfedge;
  using Facet = Prn::Facet;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (! add_attr<Pnt>(m, "Point_3")) {
    py::class_<Pnt> pnt_c(m, "Point_3");
    export_point_3<Kernel>(pnt_c);
  }

  export_vertex(m);
  export_halfedge(m);
  export_facet(m);

  if (! add_attr<Prn>(m, "Polyhedron_3")) {
    py::class_<Prn> prn_c(m, "Polyhedron_3");
    prn_c.def(py::init<>())
      .def(py::init<const Prn&>())
      .def("make_tetrahedron", &pol3::make_tetrahedron1, ri)
      .def("make_tetrahedron", &pol3::make_tetrahedron2, ri)
      .def("is_tetrahedron", &pol3::is_tetrahedron)
      ;

    add_insertion(prn_c, "__str__");
    add_insertion(prn_c, "__repr__");
    add_extraction(prn_c);

    add_attr<Pnt>(prn_c, "Point_3");
    add_attr<Vertex>(prn_c, "Vertex");
    add_attr<Halfedge>(prn_c, "Halfedge");
    add_attr<Facet>(prn_c, "Facet");
  }

  m.def("draw", &pol3::draw);
  m.def("read_polygon_mesh", &pol3::read_polygon_mesh<Prn>);
}
