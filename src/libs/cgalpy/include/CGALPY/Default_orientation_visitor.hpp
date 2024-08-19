#ifndef CGALPY_DEFAULT_ORIENTATION_VISITOR_HPP
#define CGALPY_DEFAULT_ORIENTATION_VISITOR_HPP

#include <nanobind/nanobind.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

//
struct Default_orientation_visitor : PMP::Default_orientation_visitor {

  Default_orientation_visitor() :
    nme(),
    nmv(),
    dv(),
    viipr(),
    por(),
    lcp()
  {}

  void non_manifold_edge(std::size_t id1, std::size_t id2, std::size_t nb_poly){nme(id1, id2, nb_poly);}
  void non_manifold_vertex(std::size_t id, std::size_t nb_cycles){nmv(id, nb_cycles);}
  void duplicated_vertex(std::size_t v1, std::size_t v2){dv(v1, v2);}
  void vertex_id_in_polygon_replaced(std::size_t p_id, std::size_t i1, std::size_t i2){viipr(p_id, i1, i2);}
  void polygon_orientation_reversed(std::size_t p_id){por(p_id);}
  void link_connected_polygons(std::size_t id, const std::vector<std::size_t>& v){
    lcp(id, v);
  }

  void set_non_manifold_edge(const std::function<void(std::size_t, std::size_t, std::size_t)>& fn) {nme = fn;}
  void set_non_manifold_vertex(const std::function<void(std::size_t, std::size_t)>& fn) {nmv = fn;}
  void set_duplicated_vertex(const std::function<void(std::size_t, std::size_t)>& fn) {dv = fn;}
  void set_vertex_id_in_polygon_replaced(const std::function<void(std::size_t, std::size_t, std::size_t)>& fn) {viipr = fn;}
  void set_polygon_orientation_reversed(const std::function<void(std::size_t)>& fn) {por = fn;}
  void set_link_connected_polygons(const std::function<void(std::size_t, std::vector<std::size_t>)>& fn) {lcp = fn;}

private:
  std::function<void(std::size_t, std::size_t, std::size_t)> nme;
  std::function<void(std::size_t, std::size_t)> nmv;
  std::function<void(std::size_t, std::size_t)> dv;
  std::function<void(std::size_t, std::size_t, std::size_t)> viipr;
  std::function<void(std::size_t)> por;
  std::function<void(std::size_t, std::vector<std::size_t>)> lcp;
};


} // namespace pmp

#endif // CGALPY_DEFAULT_ORIENTATION_VISITOR_HPP
