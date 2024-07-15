// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <CGAL/boost/graph/Face_filtered_graph.h>
#define CGAL_USE_BASIC_VIEWER

#include <string>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/boost/graph/generators.h>
#ifdef CGALPY_HAS_VISUAL
#include <CGAL/draw_surface_mesh.h>
#endif

#include "CGALPY/add_attr.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/surface_mesh_types.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"

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

//
template <typename SurfaceMesh>
bool write_polygon_mesh(std::string fname, const SurfaceMesh& pm,
                        const py::dict& parameters = py::dict()) {
  return CGAL::IO::write_polygon_mesh(fname, pm);
}

// Draw a surface mesh.
#ifdef CGALPY_HAS_VISUAL
template <typename SurfaceMesh>
void draw(const SurfaceMesh& sm) { CGAL::draw(sm); }
#endif

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

//
template <typename SurfaceMesh>
typename SurfaceMesh::Halfedge_index halfedge(const typename SurfaceMesh::Face_index& f,
                                              const SurfaceMesh& sm)
{ return CGAL::halfedge(f, sm); }

//
template <typename SurfaceMesh>
bool is_triangle(const typename SurfaceMesh::Halfedge_index& h, const SurfaceMesh& sm)
{ return CGAL::is_triangle(h, sm); }

/// \name Iterators
/// @{

//
template <typename SurfaceMesh>
py::object vertices(const SurfaceMesh& sm)
{ return make_iterator(sm.vertices_begin(), sm.vertices_end()); }

//
template <typename SurfaceMesh>
py::object halfedges(const SurfaceMesh& sm)
{ return make_iterator(sm.halfedges_begin(), sm.halfedges_end()); }

//
template <typename SurfaceMesh>
py::object edges(const SurfaceMesh& sm)
{ return make_iterator(sm.edges_begin(), sm.edges_end()); }

//
template <typename SurfaceMesh>
py::object faces(const SurfaceMesh& sm)
{ return make_iterator(sm.faces_begin(), sm.faces_end()); }

/// @}

} // namespace sm

//
template <typename SurfaceMesh, typename C>
void add_sm_index(C& c) {
  using Sm = SurfaceMesh;
  using size_type = typename Sm::size_type;
  using Sm_i = typename C::Type;

  c.def(py::init<>())
    .def(py::init<size_type>())
    .def("id", &Sm_i::idx)
    .def("idx", &Sm_i::idx)
    ;
}

// Export halfedge
// template <typename C>
// void export_halfedge(C& sm_c) {
//   using Sm = sm::Surface_mesh_3;
//   using Halfedge = typename Sm::Halfedge;
//   constexpr auto ri(py::rv_policy::reference_internal);
//
//   if (add_attr<Halfedge>(sm_c, "Halfedge")) return;
//
//   py::class_<Halfedge> halfedge_c(sm_c, "Halfedge");
//   halfedge_c.def(py::init<>())
//     .def("vertex", &sm::vertex<Sm>, ri)
//     .def("opposite", &sm::opposite<Sm>, ri)
//     .def("next", &sm::next<Sm>, ri)
//     .def("prev", &sm::prev<Sm>, ri)
//     .def("is_border", [](const Halfedge& e){ return e.is_border(); })
//     ;
// }

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
  using Gt = boost::graph_traits<Sm>;
  using Fd = typename Gt::face_descriptor;
  using Hd = typename Gt::halfedge_descriptor;
  using Vd = typename Gt::vertex_descriptor;

  constexpr auto ri(py::rv_policy::reference_internal);

  // Vertex_index
  using Sm_vi = typename CGAL::SM_Index<Vi>;
  if (! add_attr<Sm_vi>(m, "SM_Index_vertex_index")) {
    py::class_<Sm_vi> sm_vi_c(m, "SM_vertex_index");
    add_sm_index<Sm>(sm_vi_c);
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

  // 

  // Halfedge_index
  using Sm_hi = typename CGAL::SM_Index<Hi>;
  if (! add_attr<Sm_hi>(m, "SM_halfedge_index")) {
    py::class_<Sm_hi> sm_hi_c(m, "SM_halfedge_index");
    add_sm_index<Sm>(sm_hi_c);
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
    add_sm_index<Sm>(sm_fi_c);
  }

  if (! add_attr<Fi>(m, "Face_index")) {
    py::class_<Fi, Sm_fi>(m, "Face_index")
      .def(py::init<>())
      .def(py::init<size_type>())
      .def("__str__", [](const Fi& smi){ return std::to_string(smi.idx()); })
      .def("__repr__", [](const Fi& smi){ return std::to_string(smi.idx()); })
      ;
  }

  // Face descriptor
  if (! add_attr<Fd>(m, "Face_descriptor")) {
    py::class_<Fd>(m, "Face_descriptor")
      .def(py::init<>())
      .def(py::init<Fi>())
      .def("idx", &Fd::idx)
      .def("is_valid", &Fd::is_valid)
      .def("__str__", [](const Fd& fd){ return std::to_string(fd.idx()); })
      .def("__repr__", [](const Fd& fd){ return std::to_string(fd.idx()); })
      ;
  }

  // Halfedge descriptor
  if (! add_attr<Hd>(m, "Halfedge_descriptor")) {
    py::class_<Hd>(m, "Halfedge_descriptor")
      .def(py::init<>())
      .def(py::init<Hi>())
      .def("idx", &Hd::idx)
      .def("is_valid", &Hd::is_valid)
      .def("__str__", [](const Hd& hd){ return std::to_string(hd.idx()); })
      .def("__repr__", [](const Hd& hd){ return std::to_string(hd.idx()); })
      ;
  }

  // Vertex_descriptor
  if (! add_attr<Vd>(m, "Vertex_descriptor")) {
    py::class_<Vd>(m, "Vertex_descriptor")
      .def(py::init<>())
      .def(py::init<Vi>())
      .def("idx", &Vd::idx)
      .def("is_valid", &Vd::is_valid)
      .def("__str__", [](const Vd& vd){ return std::to_string(vd.idx()); })
      .def("__repr__", [](const Vd& vd){ return std::to_string(vd.idx()); })
      ;
  }

  // Surface mesh
  if (! add_attr<Sm>(m, name)) {

    py::class_<Sm> sm_c(m, name);
    sm_c.def(py::init<>())
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
      .def("is_empty", &Sm::is_empty)
      // void clear_without_removing_property_maps();
      // void clear();
      // void reserve(size_type nvertices, size_type nedges, size_type nfaces )
      // void resize(size_type nvertices, size_type nedges, size_type nfaces )
      // join(const Surface_mesh& other)

      .def("is_valid", py::overload_cast<bool>(&Sm::is_valid, py::const_))
#if CGAL_VERSION_NR >= 1050600000
      .def("is_valid", py::overload_cast<Vi, bool>(&Sm::is_valid, py::const_))
      .def("is_valid", py::overload_cast<Ei, bool>(&Sm::is_valid, py::const_))
      .def("is_valid", py::overload_cast<Hi, bool>(&Sm::is_valid, py::const_))
      .def("is_valid", py::overload_cast<Fi, bool>(&Sm::is_valid, py::const_))
#endif
      ;

    add_attr<Vi>(sm_c, "Vertex_index");
    add_attr<Ei>(sm_c, "Edge_index");
    add_attr<Hi>(sm_c, "Halfedge_index");
    add_attr<Fi>(sm_c, "Face_index");
    // add_attr<Halfedge>(sm_c, "Halfedge");

    using Vci = typename Sm::Vertex_iterator;
    using Hci = typename Sm::Halfedge_iterator;
    using Eci = typename Sm::Edge_iterator;
    using Fci = typename Sm::Face_iterator;

    add_iterator<Vci, Vci>("Vertex_iterator", sm_c);
    add_iterator<Hci, Hci>("Halfedge_iterator", sm_c);
    add_iterator<Eci, Eci>("Edge_iterator", sm_c);
    add_iterator<Fci, Fci>("Face_iterator", sm_c);

    sm_c.def("vertices", &sm::vertices<Sm>, py::keep_alive<0, 1>())
      .def("halfedges", &sm::halfedges<Sm>, py::keep_alive<0, 1>())
      .def("edges", &sm::edges<Sm>, py::keep_alive<0, 1>())
      .def("faces", &sm::faces<Sm>, py::keep_alive<0, 1>())
      ;

    add_insertion(sm_c, "__str__");
    add_insertion(sm_c, "__repr__");
  }

#ifdef CGALPY_HAS_VISUAL
  m.def("draw", &sm::draw<Sm>);
#endif
  m.def("null_face", &sm::null_face<Sm>);
}

// Export Surface_mesh<Pnt>
void export_surface_mesh(py::module_& m) {
  // 2D Surface mesh
  // using Sm_2 = sm::Surface_mesh_2;
  // export_surface_mesh_impl<Sm_2>(m, "Surface_mesh_2");

  // 3D Surface mesh
  using Sm_3 = sm::Surface_mesh_3;

  export_surface_mesh_impl<Sm_3>(m, "Surface_mesh_3");

  // export_halfedge(m);

  m.def("Halfedge", &sm::Halfedge<Sm_3>);

  m.def("read_polygon_mesh", &sm::read_polygon_mesh<Sm_3>);
  m.def("make_tetrahedron", &sm::make_tetrahedron<Sm_3>);
  m.def("write_polygon_mesh", &sm::write_polygon_mesh<Sm_3>,
        py::arg("fname"), py::arg("pm"), py::arg("parameters") = py::dict());
  m.def("halfedge", &sm::halfedge<Sm_3>);
  m.def("is_triangle", &sm::is_triangle<Sm_3>);
  m.def("is_triangle_mesh", &CGAL::is_triangle_mesh<Sm_3>);
}
