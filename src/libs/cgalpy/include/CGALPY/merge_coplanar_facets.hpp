// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_MERGE_COPLANAR_FACETS_HPP
#define CGALPY_MERGE_COPLANAR_FACETS_HPP

#include <list>
#include <unordered_map>

#include <boost/graph/graph_traits.hpp>

#include <CGAL/boost/graph/named_params_helper.h>
#include <CGAL/Named_function_parameters.h>
#include <CGAL/Union_find.h>

#include <CGAL/boost/graph/helpers.h>
#include <CGAL/boost/graph/properties.h>
#include <CGAL/Dynamic_property_map.h>
#include <CGAL/Origin.h>

// #include <boost/graph/graph_traits.hpp>

// #include <CGAL/boost/graph/helpers.h>
// #include <CGAL/boost/graph/properties.h>
// #include <CGAL/Named_function_parameters.h>

namespace CGAL {
namespace Polygon_mesh_processing {

template <typename FaceDescriptor, typename HandleMap>
typename CGAL::Union_find<FaceDescriptor>::handle
uf_get_handle(FaceDescriptor f, CGAL::Union_find<FaceDescriptor>& uf_faces, HandleMap& handles) {
  using Face_descriptor =  FaceDescriptor;
  using Uf_faces = CGAL::Union_find<Face_descriptor>;
  using Uf_handle = typename Uf_faces::handle;

  auto insert_res = handles.emplace(f, Uf_handle());
  if (insert_res.second) insert_res.first->second = uf_faces.make_set(f);
  return insert_res.first->second;
}

template <typename FaceDescriptor, typename HandleMap>
bool uf_join_faces(FaceDescriptor f1, FaceDescriptor f2, CGAL::Union_find<FaceDescriptor>& uf_faces,
                   HandleMap& handles) {
  using Face_descriptor = FaceDescriptor;
  using Uf_faces = CGAL::Union_find<Face_descriptor>;
  using Uf_handle = typename Uf_faces::handle;

  Uf_handle h1 = uf_get_handle(f1, uf_faces, handles);
  Uf_handle h2 = uf_get_handle(f2, uf_faces, handles);
  bool same_set = uf_faces.same_set(h1, h2);
  if (! same_set) uf_faces.unify_sets(h1, h2);
  return ! same_set;
}

/*! Merge coplanar facets.
 */
template <typename PolygonMesh, typename FaceNormalMap,
          typename NamedParameters = parameters::Default_named_parameters>
void merge_coplanar_facets(PolygonMesh& mesh, FaceNormalMap normals,
                           const NamedParameters& np = parameters::default_values()) {
  namespace parms = CGAL::parameters;

  using Pm = PolygonMesh;
  using Np = NamedParameters;
  using Graph_traits = boost::graph_traits<Pm>;
  using halfedge_descriptor = typename Graph_traits::halfedge_descriptor;
  using face_descriptor = typename Graph_traits::face_descriptor;
  using Uf_faces = CGAL::Union_find<face_descriptor>;

  Uf_faces uf_faces;
  std::unordered_map<face_descriptor, typename Uf_faces::handle> uf_handles;
  std::list<halfedge_descriptor> hds;

  //! \todo Pass the kernel via the named-parameters.
  using Kernel = typename CGAL::GetGeomTraits<Pm, Np>::type;
  const auto& param = parms::get_parameter(np, CGAL::internal_np::geom_traits);
  auto kernel = parms::choose_parameter<Kernel>(param);

  auto eq = kernel.equal_3_object();

  // Group coplanar facets in subsets
  for (auto ed : CGAL::edges(mesh)) {
    auto hd = CGAL::halfedge(ed, mesh);
    auto fd = CGAL::face(hd, mesh);
    auto oed = CGAL::opposite(hd, mesh);
    auto ofd = CGAL::face(oed, mesh);
    const auto& dir = get(normals, fd).direction();
    const auto& odir = get(normals, ofd).direction();
    if ((fd != ofd) && eq(dir, odir)) {
       if (uf_join_faces(fd, ofd, uf_faces, uf_handles)) hds.emplace_back(hd);
    }
  }
  for (auto hd : hds) CGAL::Euler::join_face(hd, mesh);

  // Traverse all vertices and remove antenas.
  bool done;
  do {
    done = true;
    for (auto vd : CGAL::vertices(mesh)) {
      if (CGAL::degree(vd, mesh) != 1) continue;
      auto hd = CGAL::halfedge(vd, mesh);
      CGAL::Euler::remove_center_vertex(hd, mesh);
      done = false;
    }
  } while (! done);

  // Traverse all vertices and remove vertices of degree 2.
  for (auto vd : CGAL::vertices(mesh)) {
    if (CGAL::degree(vd, mesh) != 2) continue;
    auto hd = CGAL::halfedge(vd, mesh);
    CGAL::Euler::join_vertex(CGAL::opposite(hd, mesh), mesh);
  }
}

}
}

#endif
