#ifndef CGALPY_PMP_HELPERS_HPP
#define CGALPY_PMP_HELPERS_HPP

#include <nanobind/nanobind.h>
#include "CGALPY/kernel_types.hpp"
#include <boost/graph/graph_traits.hpp>

namespace py = nanobind;

// map getters
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
template <typename PolygonMesh>
auto get_vertex_point_map(const PolygonMesh& pm, const py::dict& np = py::dict()) {
  using Vd = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;
  using vpmap = typename PolygonMesh::template Property_map<Vd, Point_3>;
  auto vpm = np.contains("vertex_point_map") ? py::cast<vpmap>(np["vertex_point_map"]) : pm.points();
  return vpm;
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
  return dict_key.is_none() ? pm.template add_property_map<K, V>(map_name, V()).first :
    py::cast<map_type>(dict_key);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
#if CGALPY_PMP_POLYGONAL_MESH == 0 //polyhedron
  // boost::property_map<Mesh, CGAL::dynamic_vertex_property_t<Epic_kernel::FT>>::type
template <typename PolygonMesh, typename V>
auto get_vertex_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const V& default_value = V()) {
  using dynamic_prop = typename CGAL::dynamic_vertex_property_t<V>;
  using map_type = typename boost::property_map<PolygonMesh, dynamic_prop>::type;
  return dict_key.is_none() ? get(dynamic_prop(), pm) :
    py::cast<map_type>(dict_key);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 0

#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
template <typename PolygonMesh, typename F>
auto get_edge_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const F& default_value = F()) {
  using K = typename boost::graph_traits<PolygonMesh>::edge_descriptor;
  using map_type = typename PolygonMesh::template Property_map<K, F>;
  return dict_key.is_none() ? pm.template add_property_map<K, F>(map_name, F()).first :
    py::cast<map_type>(dict_key);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
#if CGALPY_PMP_POLYGONAL_MESH == 0 //polyhedron
template <typename PolygonMesh, typename F>
auto get_edge_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const F& default_value = F()) {
  using dynamic_prop = typename CGAL::dynamic_edge_property_t<F>;
  using map_type = typename boost::property_map<PolygonMesh, dynamic_prop>::type;
  return dict_key.is_none() ? get(dynamic_prop(), pm) :
    py::cast<map_type>(dict_key);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 0
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
template <typename PolygonMesh, typename F>
auto get_face_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const F& default_value = F()) {
  using K = typename boost::graph_traits<PolygonMesh>::face_descriptor;
  using map_type = typename PolygonMesh::template Property_map<K, F>;
  return dict_key.is_none() ? pm.template add_property_map<K, F>(map_name, F()).first :
    py::cast<map_type>(dict_key);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
#if CGALPY_PMP_POLYGONAL_MESH == 0 //polyhedron
template <typename PolygonMesh, typename F>
auto get_face_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const F& default_value = F()) {
  using dynamic_prop = typename CGAL::dynamic_face_property_t<F>;
  using map_type = typename boost::property_map<PolygonMesh, dynamic_prop>::type;
  return dict_key.is_none() ? get(dynamic_prop(), pm) :
    py::cast<map_type>(dict_key);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 0

#endif // CGALPY_PMP_HELPERS_HPP
