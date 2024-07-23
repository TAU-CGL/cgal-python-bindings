#ifndef CGALPY_DEFAULT_ORIENTATION_VISITOR_HPP
#define CGALPY_DEFAULT_ORIENTATION_VISITOR_HPP

#include <nanobind/nanobind.h>
#include <nanobind/trampoline.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

//
struct Dummy_default_orienatation_visitor : public PMP::Default_orientation_visitor {
public:

  // Construct default
  Dummy_default_orienatation_visitor() :
    m_non_manifold_edge(py::none()),
    m_non_manifold_vertex(py::none()),
    m_duplicated_vertex(py::none()),
    m_vertex_id_in_polygon_replaced(py::none()),
    m_polygon_orientation_reversed(py::none())
    // m_link_connected_polygons(py::none())
  {}

  /// Setters
  /// @{

  void set_non_manifold_edge(py::object fnc) { m_non_manifold_edge = fnc; }
  void set_non_manifold_vertex(py::object fnc) { m_non_manifold_vertex = fnc; }
  void set_duplicated_vertex(py::object fnc) { m_duplicated_vertex = fnc; }
  void set_vertex_id_in_polygon_replaced(py::object fnc) { m_vertex_id_in_polygon_replaced = fnc; }
  void set_polygon_orientation_reversed(py::object fnc) { m_polygon_orientation_reversed = fnc; }
  // void set_link_connected_polygons(py::object fnc) { m_set_link_connected_polygons = fnc; }
  /// @}

  /// \Name face notifications
  /// @{

  void non_manifold_edge(std::size_t id1, std::size_t id2, std::size_t nb_poly)
  { if (! m_non_manifold_edge.is_none()) m_non_manifold_edge(id1, id2, nb_poly); }
  void non_manifold_vertex(std::size_t i1,std::size_t i2)
  { if (! m_non_manifold_vertex.is_none()) m_non_manifold_vertex(i1, i2); }
  void duplicated_vertex(std::size_t i1, std::size_t i2)
  { if (! m_duplicated_vertex.is_none()) m_duplicated_vertex(i1, i2); }
  void vertex_id_in_polygon_replaced(std::size_t i1, std::size_t i2, std::size_t i3)
  { if (! m_vertex_id_in_polygon_replaced.is_none()) m_vertex_id_in_polygon_replaced(i1, i2, i3); }
  void polygon_orientation_reversed(std::size_t i1)
  { if (! m_polygon_orientation_reversed.is_none()) m_polygon_orientation_reversed(i1); }
  void link_connected_polygons(std::size_t, const std::vector<std::size_t>&){}

  /// @}

private:
  py::object m_non_manifold_edge;
  py::object m_non_manifold_vertex;
  py::object m_duplicated_vertex;
  py::object m_vertex_id_in_polygon_replaced;
  py::object m_polygon_orientation_reversed;
  py::object m_link_connected_polygons;
};

} // namespace pmp

#endif // CGALPY_DEFAULT_ORIENTATION_VISITOR_HPP
