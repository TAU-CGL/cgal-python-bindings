// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_EDGE_IS_CONSTRAINED_MAP_HPP
#define CGALPY_NAMED_PARAMETER_EDGE_IS_CONSTRAINED_MAP_HPP

#include <stdexcept>
#include <string>

#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>

#include <CGAL/boost/graph/properties.h>

#include <nanobind/nanobind.h>

#include "cgalpy/polygon_mesh_processing_config.hpp"

namespace py = nanobind;

namespace cgalpy {

template <typename PolygonMesh>
struct Named_parameter_edge_is_constrained_map {
  const std::string m_name = "edge_is_constrained_map";

  template <typename NamedParameters, typename Value>
  auto operator()(NamedParameters& np, Value& value) const {
    using Ed = typename boost::graph_traits<PolygonMesh>::edge_descriptor;

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
    using Edge_is_constrained_map = typename PolygonMesh::template Property_map<Ed, bool>;
#elif CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
    using Edge_is_constrained_map =
      typename boost::property_map<PolygonMesh, CGAL::dynamic_edge_property_t<bool>>::type;
#else
#  error Unsupported PMP polygonal mesh type
#endif

    try {
      return np.edge_is_constrained_map(py::cast<Edge_is_constrained_map>(value));
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Named parameter 'edge_is_constrained_map' must be an edge bool property map.");
    }
  }
};

} // namespace cgalpy

#endif
