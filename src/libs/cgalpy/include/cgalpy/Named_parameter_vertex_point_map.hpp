// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_VERTEX_POINT_MAP_HPP
#define CGALPY_NAMED_PARAMETER_VERTEX_POINT_MAP_HPP

#include <stdexcept>
#include <string>

#include <boost/graph/graph_traits.hpp>

#include <CGAL/boost/graph/properties.h>

#include <nanobind/nanobind.h>

#include "cgalpy/kernel_types.hpp"
#include "cgalpy/polygon_mesh_processing_config.hpp"

namespace py = nanobind;

namespace cgalpy {

template <typename PolygonMesh>
struct Named_parameter_vertex_point_map {
  const std::string m_name = "vertex_point_map";

  template <typename NamedParameters, typename Value>
  auto operator()(NamedParameters& np, Value& value) const {
    using Vd = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
    using Vertex_point_map = typename PolygonMesh::template Property_map<Vd, Point_3>;
#elif CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
    using Vertex_point_map = typename boost::property_map<PolygonMesh, CGAL::vertex_point_t>::type;
#else
#  error Unsupported PMP polygonal mesh type
#endif

    try {
      return np.vertex_point_map(py::cast<Vertex_point_map>(value));
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Named parameter 'vertex_point_map' must be a vertex point property map.");
    }
  }
};

} // namespace cgalpy

#endif
