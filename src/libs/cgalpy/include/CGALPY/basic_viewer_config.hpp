// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <boost/graph/graph_traits.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/polyhedron_3_types.hpp"

#ifndef CGALPY_BASIC_VIEWER_CONFIG_HPP
#define CGALPY_BASIC_VIEWER_CONFIG_HPP

#define CGALPY_BVR_AOS_2_DATA_STRUCTURE                 0
#define CGALPY_BVR_AOS_WITH_HISTORY_2_DATA_STRUCTURE    1
#define CGALPY_BVR_POLYHEDRON_3_DATA_STRUCTURE          2

namespace bvr {

template <int id> struct Ds_params {};

template <> struct Ds_params<CGALPY_BVR_AOS_2_DATA_STRUCTURE> {
  using Ds = aos2::Arrangement_2;
  using Vertex_descriptor = aos2::Vertex_handle;
  using Edge_descriptor = aos2::Halfedge_handle;
  using Face_descriptor = aos2::Face_handle;
  using Volume_descriptor = void;
};

template <> struct Ds_params<CGALPY_BVR_POLYHEDRON_3_DATA_STRUCTURE> {
  using Ds = pol3::Polyhedron_3;
  using Vertex_descriptor = typename boost::graph_traits<Ds>::vertex_descriptor;
  using Edge_descriptor = typename boost::graph_traits<Ds>::edge_descriptor;
  using Face_descriptor = typename boost::graph_traits<Ds>::face_descriptor;
  using Volume_descriptor = void;
};

}

#endif
