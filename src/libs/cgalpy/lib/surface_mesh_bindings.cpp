// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <string>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <CGAL/Surface_mesh.h>
#include <CGAL/draw_surface_mesh.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

namespace sm {

//
template <typename SurfaceMesh>
SurfaceMesh make_tetrahedron(const typename SurfaceMesh::Point& p1,
                             const typename SurfaceMesh::Point& p2,
                             const typename SurfaceMesh::Point& p3,
                             const typename SurfaceMesh::Point& p4) {
  using Sm = SurfaceMesh;

  Sm sm;
  CGAL::make_tetrahedron(p1, p2, p3, p4, sm);
  return sm;
}

// Read a surface mesh from a file.
template <typename SurfaceMesh>
SurfaceMesh read_polygon_mesh(const std::string& filename) {
  using Sm = SurfaceMesh;

  Sm sm;
  if (! CGAL::IO::read_polygon_mesh(filename, sm))
    throw std::runtime_error("Cannot read file!");
  return sm;
}

// Draw a surface mesh.
template <typename SurfaceMesh>
void draw(const SurfaceMesh& sm) { CGAL::draw(sm); }

// Obtain the null face.
template <typename SurfaceMesh>
static typename SurfaceMesh::Face_index null_face()
{ return SurfaceMesh::null_face(); }

// Add a face from a list of vertices.
template <typename SurfaceMesh>
typename SurfaceMesh::Face_index add_face(SurfaceMesh& sm, py::list& lst) {
  using Sm = SurfaceMesh;
  using Vi = typename Sm::Vertex_index;

  auto begin = stl_input_iterator<Vi>(lst);
  auto end = stl_input_iterator<Vi>(lst, false);
  return sm.add_face(CGAL::make_range(begin, end));
}

template <typename SurfaceMesh, typename C>
void add_sm_index(C& c) {
  using Sm = SurfaceMesh;
  using size_type = typename Sm::size_type;
  using Sm_i = typename C::Type;

  c.def(py::init<>())
    .def(py::init<size_type>())
    .def("idx", &Sm_i::idx)
    ;
}

}

// Export Surface_mesh.
template <typename SurfaceMesh>
void export_surface_mesh_impl(py::module_& m, const char* name) {
  using Sm = SurfaceMesh;
  using Pnt = typename Sm::Point;
  using size_type = typename Sm::size_type;
  using Vi = typename Sm::Vertex_index;
  using Ei = typename Sm::Edge_index;
  using Hi = typename Sm::Halfedge_index;
  using Fi = typename Sm::Face_index;

  constexpr auto ri(py::rv_policy::reference_internal);

  // Vertex_index
  using Sm_vi = typename CGAL::SM_Index<Vi>;
  if (! add_attr<Sm_vi>(m, "SM_Index_vertex_index")) {
    py::class_<Sm_vi> sm_vi_c(m, "SM_vertex_index");
    sm::add_sm_index<Sm>(sm_vi_c);
  }

  if (! add_attr<Vi>(m, "Vertex_index")) {
    py::class_<Vi, Sm_vi>(m, "Vertex_index")
      .def(py::init<>())
      .def(py::init<size_type>())
      .def("__str__", [](const Vi& smi){ return std::to_string(smi.idx()); })
      .def("__repr__", [](const Vi& smi){ return std::to_string(smi.idx()); })
      ;
  }

  // Edge_index
  if (! add_attr<Ei>(m, "Edge_index")) {
    py::class_<Ei>(m, "Edge_index")
      .def(py::init<>())
      .def(py::init<size_type>())
      .def("halfedge", &Ei::halfedge)
      .def("idx", &Ei::idx)
      .def("reset", &Ei::reset)
      .def("is_valid", &Ei::is_valid)
      .def("__str__", [](const Ei& smi){ return std::to_string(smi.idx()); })
      .def("__repr__", [](const Ei& smi){ return std::to_string(smi.idx()); })
      ;
  }

  // Halfedge_index
  using Sm_hi = typename CGAL::SM_Index<Hi>;
  if (! add_attr<Sm_hi>(m, "SM_halfedge_index")) {
    py::class_<Sm_hi> sm_hi_c(m, "SM_halfedge_index");
    sm::add_sm_index<Sm>(sm_hi_c);
  }

  if (! add_attr<Hi>(m, "Halfedge_index")) {
    py::class_<Hi, Sm_hi>(m, "Halfedge_index")
      .def(py::init<>())
      .def(py::init<size_type>())
      .def("__str__", [](const Hi& smi){ return std::to_string(smi.idx()); })
      .def("__repr__", [](const Hi& smi){ return std::to_string(smi.idx()); })
      ;
  }

  // Face_index
  using Sm_fi = typename CGAL::SM_Index<Fi>;
  if (! add_attr<Sm_fi>(m, "SM_face_index")) {
    py::class_<Sm_fi> sm_fi_c(m, "SM_face_index");
    sm::add_sm_index<Sm>(sm_fi_c);
  }

  if (! add_attr<Fi>(m, "Face_index")) {
    py::class_<Fi, Sm_fi>(m, "Face_index")
      .def(py::init<>())
      .def(py::init<size_type>())
      .def("__str__", [](const Fi& smi){ return std::to_string(smi.idx()); })
      .def("__repr__", [](const Fi& smi){ return std::to_string(smi.idx()); })
      ;
  }

  // Surface mesh
  if (! add_attr<Sm>(m, name)) {

    py::class_<Sm> sm3_c(m, name);
    sm3_c.def(py::init<>())
      .def(py::init<const Sm&>())
      // .def("assign", &Sm::assign, ri)
      .def("add_vertex", py::overload_cast<>(&Sm::add_vertex))
      .def("add_vertex", py::overload_cast<const Pnt&>(&Sm::add_vertex))
      .def("add_edge", py::overload_cast<>(&Sm::add_edge))
      .def("add_edge", py::overload_cast<Vi, Vi>(&Sm::add_edge))
      .def("add_face", static_cast<Fi(Sm::*)()>(&Sm::add_face))
      .def("add_face", static_cast<Fi(Sm::*)(Vi, Vi, Vi)>(&Sm::add_face))
      .def("add_face", static_cast<Fi(Sm::*)(Vi, Vi, Vi, Vi)>(&Sm::add_face))
      .def("add_face", &sm::add_face<Sm>)
      // void remove_vertex(Vertex_index v)
      // void remove_edge(Edge_index e)
      // remove_face(Face_index f)
      // size_type number_of_vertices() const
      // size_type number_of_halfedges() const
      // size_type number_of_edges() const
      // size_type number_of_faces() const
      // bool is_empty() const
      // void clear_without_removing_property_maps();
      // void clear();
      // void reserve(size_type nvertices, size_type nedges, size_type nfaces )
      // void resize(size_type nvertices, size_type nedges, size_type nfaces )
      // join(const Surface_mesh& other)
      .def("is_valid", py::overload_cast<bool>(&Sm::is_valid, py::const_))
      .def("is_valid", py::overload_cast<Vi>(&Sm::is_valid, py::const_))
      .def("is_valid", py::overload_cast<Ei>(&Sm::is_valid, py::const_))
      .def("is_valid", py::overload_cast<Hi>(&Sm::is_valid, py::const_))
      .def("is_valid", py::overload_cast<Fi>(&Sm::is_valid, py::const_))
      ;

    add_attr<Vi>(sm3_c, "Vertex_index");
    add_attr<Ei>(sm3_c, "Edge_index");
    add_attr<Hi>(sm3_c, "Halfedge_index");
    add_attr<Fi>(sm3_c, "Face_index");
  }

  m.def("draw", &sm::draw<Sm>);
  m.def("null_face", &sm::null_face<Sm>);
}

// Export Surface_mesh<Pnt>
void export_surface_mesh(py::module_& m) {
  // 2D Surface mesh
  // using Sm_2 = CGAL::Surface_mesh<Kernel::Point_2>;
  // export_surface_mesh_impl<Sm_2>(m, "Surface_mesh_2");

  // 3D Surface mesh
  using Sm_3 = CGAL::Surface_mesh<Kernel::Point_3>;

  export_surface_mesh_impl<Sm_3>(m, "Surface_mesh_3");

  // Read
  m.def("read_polygon_mesh", &sm::read_polygon_mesh<Sm_3>);
  m.def("make_tetrahedron", &sm::make_tetrahedron<Sm_3>);
}

namespace py = nanobind;
