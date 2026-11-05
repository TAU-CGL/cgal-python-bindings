// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_VERTEX_POINT_MAP_HPP
#define CGALPY_NAMED_PARAMETER_VERTEX_POINT_MAP_HPP

#include <stdexcept>
#include <string>

#include <nanobind/nanobind.h>

#include <boost/graph/graph_traits.hpp>

#include <CGAL/boost/graph/properties.h>

#include "cgalpy/is_surface_mesh.hpp"
#include "cgalpy/is_polyhedron_3.hpp"
#include "cgalpy/kernel_types.hpp"
#include "cgalpy/polygon_mesh_processing_config.hpp"

namespace py = nanobind;

namespace cgalpy {

//! Always false, but dependent on `T`, so it fires only when instantiated.
template <typename T>
inline constexpr bool dependent_false_v = false;

//!
template <typename PolygonMesh>
struct Named_parameter_vertex_point_map {
  using Pm = PolygonMesh;

  static constexpr const char* m_name = "vertex_point_map";

  template <typename NamedParameters, typename Value>
  auto operator()(NamedParameters& np, Value& value) const {
    using Vd = typename boost::graph_traits<Pm>::vertex_descriptor;

    if constexpr (is_surface_mesh_v<Pm>) {
      using Vertex_point_map = typename PolygonMesh::template Property_map<Vd, Point_3>;
      return apply<Vertex_point_map>(np, value);
    }
    else if constexpr (is_polyhedron_3_v<Pm>) {
      using Vertex_point_map = typename boost::property_map<Pm, CGAL::vertex_point_t>::type;
      return apply<Vertex_point_map>(np, value);
    }
    else {
      static_assert(dependent_false_v<Pm>, "Named_parameter_vertex_point_map: unsupported polygonal mesh type");
    }
  }

private:
  template <typename Vertex_point_map, typename NamedParameters, typename Value>
  static auto apply(NamedParameters& np, Value& value) {
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
