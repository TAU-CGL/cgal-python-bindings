#ifndef CGALPY_CUSTOM_SIZING_FIELD_HPP
#define CGALPY_CUSTOM_SIZING_FIELD_HPP

#include <stdexcept>

#include <nanobind/nanobind.h>

#include <boost/graph/graph_traits.hpp>

#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

namespace pmp {

template <typename PolygonMesh>
struct Custom_sizing_field {
  using vertex_descriptor = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;
  using halfedge_descriptor = typename boost::graph_traits<PolygonMesh>::halfedge_descriptor;

  // set all functions to nullptr on initialization
  Custom_sizing_field<PolygonMesh>()
  {
    at_ = nullptr;
    is_too_long_ = nullptr;
    is_too_short_ = nullptr;
    split_placement_ = nullptr;
    register_split_vertex_ = nullptr;
  }

  FT at(const vertex_descriptor v, const PolygonMesh& pmesh) const
  { if (!at_) throw std::runtime_error("at function not set"); return at_(v, pmesh); }
  std::optional<FT> is_too_long(const vertex_descriptor va, const vertex_descriptor vb, const PolygonMesh& pmesh) const
  { if (!is_too_long_) throw std::runtime_error("is_too_long function not set"); return is_too_long_(va, vb, pmesh); }
  std::optional<FT> is_too_short(const halfedge_descriptor h, const PolygonMesh& pmesh) const
  { if (!is_too_short_) throw std::runtime_error("is_too_short function not set"); return is_too_short_(h, pmesh); }
  Point_3 split_placement(const halfedge_descriptor h, const PolygonMesh& pmesh) const
  { if (!split_placement_) throw std::runtime_error("split_placement function not set"); return split_placement_(h, pmesh); }
  void register_split_vertex(const vertex_descriptor v, const PolygonMesh& pmesh)
  { if (!register_split_vertex_) throw std::runtime_error("register_split_vertex function not set"); register_split_vertex_(v, pmesh); }

  void set_at(const std::function<FT(const vertex_descriptor, const PolygonMesh&)>& f) { at_ = f; }
  void set_is_too_long(const std::function<std::optional<FT>(const vertex_descriptor, const vertex_descriptor, const PolygonMesh&)>& f) { is_too_long_ = f; }
  void set_is_too_short(const std::function<std::optional<FT>(const halfedge_descriptor, const PolygonMesh&)>& f) { is_too_short_ = f; }
  void set_split_placement(const std::function<Point_3(const halfedge_descriptor, const PolygonMesh&)>& f) { split_placement_ = f; }
  void set_register_split_vertex(const std::function<void(const vertex_descriptor, const PolygonMesh&)>& f) { register_split_vertex_ = f; }


private:
  std::function<FT(const vertex_descriptor, const PolygonMesh&)> at_;
  std::function<std::optional<FT>(const vertex_descriptor, const vertex_descriptor, const PolygonMesh&)> is_too_long_;
  std::function<std::optional<FT>(const halfedge_descriptor, const PolygonMesh&)> is_too_short_;
  std::function<Point_3(const halfedge_descriptor, const PolygonMesh&)> split_placement_;
  std::function<void(const vertex_descriptor, const PolygonMesh&)> register_split_vertex_;
};

} // namespace pmp

#endif // CGALPY_CUSTOM_SIZING_FIELD_HPP

