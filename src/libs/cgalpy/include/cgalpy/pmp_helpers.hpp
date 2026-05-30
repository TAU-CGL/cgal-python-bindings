#ifndef CGALPY_PMP_HELPERS_HPP
#define CGALPY_PMP_HELPERS_HPP

#include <nanobind/nanobind.h>
#include "cgalpy/kernel_types.hpp"
#include <boost/graph/graph_traits.hpp>
#include <stdexcept>

namespace py = nanobind;

// map getters
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
template <typename PolygonMesh>
auto get_vertex_point_map(const PolygonMesh& pm, const py::dict& np = py::dict()) {
  using Vd = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;
  using vpmap = typename PolygonMesh::template Property_map<Vd, Point_3>;
  // auto vpm = np.contains("vertex_point_map") ? py::cast<vpmap>(np["vertex_point_map"]) : pm.points();
  // return vpm;
  // improve cast error handling
  if(np.contains("vertex_point_map")) {
    try {
      return py::cast<vpmap>(np["vertex_point_map"]);
    }
    catch(py::cast_error& e) {
      throw std::runtime_error("Failed to cast vertex_point_map to desired type");
    }
  }
  else {
    return pm.points();
  }
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
#if CGALPY_PMP_POLYGONAL_MESH == 0 //polyhedron
template <typename PolygonMesh>
auto get_vertex_point_map(PolygonMesh& pm, const py::dict& np = py::dict()) {
  // TODO: get the map from the dict
  return get(CGAL::vertex_point, pm);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 0

#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
template <typename PolygonMesh, typename V>
auto get_vertex_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const V& default_value = V()) {
  using K = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;
  using map_type = typename PolygonMesh::template Property_map<K, V>;
  if (dict_key.is_none()) {
    return pm.template add_property_map<K, V>(map_name, default_value).first;
  }
  else {
    try {
      return py::cast<map_type>(dict_key);
    }
    catch(py::cast_error& e) {
      throw std::runtime_error("Failed to cast vertex property map to desired type");
    }
  }
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
#if CGALPY_PMP_POLYGONAL_MESH == 0 //polyhedron
  // boost::property_map<Mesh, CGAL::dynamic_vertex_property_t<Epic_kernel::FT>>::type
template <typename PolygonMesh, typename V>
auto get_vertex_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const V& default_value = V()) {
  using dynamic_prop = typename CGAL::dynamic_vertex_property_t<V>;
  using map_type = typename boost::property_map<PolygonMesh, dynamic_prop>::type;
  // return dict_key.is_none() ? get(dynamic_prop(), pm) :
  //   py::cast<map_type>(dict_key);
  if (dict_key.is_none()) {
    return get(dynamic_prop(), pm);
  }
  else {
    try {
      return py::cast<map_type>(dict_key);
    }
    catch(py::cast_error& e) {
      throw std::runtime_error("Failed to cast vertex property map to desired type");
    }
  }
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 0

#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
template <typename PolygonMesh, typename F>
auto get_edge_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const F& default_value = F()) {
  using K = typename boost::graph_traits<PolygonMesh>::edge_descriptor;
  using map_type = typename PolygonMesh::template Property_map<K, F>;
  if (dict_key.is_none()) {
    return pm.template add_property_map<K, F>(map_name, default_value).first;
  }
  else {
    try {
      return py::cast<map_type>(dict_key);
    }
    catch(py::cast_error& e) {
      throw std::runtime_error("Failed to cast edge property map to desired type");
    }
  }
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
#if CGALPY_PMP_POLYGONAL_MESH == 0 //polyhedron
template <typename PolygonMesh, typename F>
auto get_edge_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const F& default_value = F()) {
  using dynamic_prop = typename CGAL::dynamic_edge_property_t<F>;
  using map_type = typename boost::property_map<PolygonMesh, dynamic_prop>::type;
  if (dict_key.is_none()) {
    return get(dynamic_prop(), pm);
  }
  else {
    try {
      return py::cast<map_type>(dict_key);
    }
    catch(py::cast_error& e) {
      throw std::runtime_error("Failed to cast edge property map to desired type");
    }
  }
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 0
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
template <typename PolygonMesh, typename F>
auto get_face_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const F& default_value = F()) {
  using K = typename boost::graph_traits<PolygonMesh>::face_descriptor;
  using map_type = typename PolygonMesh::template Property_map<K, F>;
  if (dict_key.is_none()) {
    return pm.template add_property_map<K, F>(map_name, default_value).first;
  }
  else {
    try {
      return py::cast<map_type>(dict_key);
    }
    catch(py::cast_error& e) {
      throw std::runtime_error("Failed to cast face property map to desired type");
    }
  }
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
#if CGALPY_PMP_POLYGONAL_MESH == 0 //polyhedron
template <typename PolygonMesh, typename F>
auto get_face_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const F& default_value = F()) {
  using dynamic_prop = typename CGAL::dynamic_face_property_t<F>;
  using map_type = typename boost::property_map<PolygonMesh, dynamic_prop>::type;
  if (dict_key.is_none()) {
    return get(dynamic_prop(), pm);
  }
  else {
    try {
      return py::cast<map_type>(dict_key);
    }
    catch(py::cast_error& e) {
      throw std::runtime_error("Failed to cast face property map to desired type");
    }
  }
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 0
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
template <typename PolygonMesh, typename F>
auto get_halfedge_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const F& default_value = F()) {
  using K = typename boost::graph_traits<PolygonMesh>::halfedge_descriptor;
  using map_type = typename PolygonMesh::template Property_map<K, F>;
  if (dict_key.is_none()) {
    return pm.template add_property_map<K, F>(map_name, default_value).first;
  }
  else {
    try {
      return py::cast<map_type>(dict_key);
    }
    catch(py::cast_error& e) {
      throw std::runtime_error("Failed to cast halfedge property map to desired type");
    }
  }
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
#if CGALPY_PMP_POLYGONAL_MESH == 0 //polyhedron
template <typename PolygonMesh, typename F>
auto get_halfedge_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const F& default_value = F()) {
  using dynamic_prop = typename CGAL::dynamic_halfedge_property_t<F>;
  using map_type = typename boost::property_map<PolygonMesh, dynamic_prop>::type;
  if (dict_key.is_none()) {
    return get(dynamic_prop(), pm);
  }
  else {
    try {
      return py::cast<map_type>(dict_key);
    }
    catch(py::cast_error& e) {
      throw std::runtime_error("Failed to cast halfedge property map to desired type");
    }
  }
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 0

#endif // CGALPY_PMP_HELPERS_HPP
