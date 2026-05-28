// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com
//            Efi Fogel          <efifogel@gmail.com>

#ifndef CGALPY_BGL_GLOBAL_HPP
#define CGALPY_BGL_GLOBAL_HPP

/*! \file
 * This file contains template of wrappers of free functions from the
 * "CGAL and the Boost Graph Library" package that do not involve cell
 * descriptors, cell handlers, or any other reference-counted types.
 * They are separated from the rest, because they can be used more widely.
 */

#include <boost/graph/graph_traits.hpp>

#include <CGAL/boost/graph/generators.h>

namespace cgalpy {
namespace bgl {

//!
template <typename PolygonMesh>
auto my_add_edge(PolygonMesh& p) { return add_edge(p); }

//!
template <typename PolygonMesh>
auto my_add_face(PolygonMesh& p) { return add_face(p); }

//!
template <typename PolygonMesh>
auto my_add_vertex(PolygonMesh& p) { return add_vertex(p); }

//!
template <typename PolygonMesh>
PolygonMesh my_make_tetrahedron(const typename PolygonMesh::Point& p1,
                                const typename PolygonMesh::Point& p2,
                                const typename PolygonMesh::Point& p3,
                                const typename PolygonMesh::Point& p4) {
  using Pm = PolygonMesh;

  Pm pm;
  make_tetrahedron(p1, p2, p3, p4, pm);
  return pm;
}

//!
template <typename PolygonMesh>
PolygonMesh my_make_hexahedron(const typename PolygonMesh::Point& p1,
                               const typename PolygonMesh::Point& p2,
                               const typename PolygonMesh::Point& p3,
                               const typename PolygonMesh::Point& p4,
                               const typename PolygonMesh::Point& p5,
                               const typename PolygonMesh::Point& p6,
                               const typename PolygonMesh::Point& p7,
                               const typename PolygonMesh::Point& p8) {
  using Pm = PolygonMesh;

  Pm pm;
  make_hexahedron(p1, p2, p3, p4, p5, p6, p7, p8, pm);
  return pm;
}

//!
// template <typename PolygonMesh>
// auto collect_garbage(PolygonMesh& p) { collect_garbage(p); }

//!
template <typename PolygonMesh>
auto my_num_vertices(const PolygonMesh& p) { return num_vertices(p); }

//!
template <typename PolygonMesh>
auto my_num_edges(const PolygonMesh& p) { return num_edges(p); }

//!
template <typename PolygonMesh>
auto my_num_halfedges(const PolygonMesh& p) { return num_halfedges(p); }

//!
template <typename PolygonMesh>
auto my_num_faces(const PolygonMesh& p) { return num_faces(p); }

//!
template <typename PolygonMesh>
auto my_remove_all_elements(PolygonMesh& p) { remove_all_elements(p); }

//!
template <typename PolygonMesh>
auto my_reserve(PolygonMesh& p,
                typename boost::graph_traits<PolygonMesh>::vertices_size_type nv,
                typename boost::graph_traits<PolygonMesh>::edges_size_type ne,
                typename boost::graph_traits<PolygonMesh>::faces_size_type nf)
{ reserve(p, nv, ne, nf); }

}
} // namespace cgalpy

#endif
