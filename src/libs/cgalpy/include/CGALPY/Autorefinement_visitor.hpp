#ifndef CGALPY_AUTOREFINEMENT_VISITOR_HPP
#define CGALPY_AUTOREFINEMENT_VISITOR_HPP

#include <nanobind/nanobind.h>

#include <CGAL/Polygon_mesh_processing/autorefinement.h>

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

//
struct Autorefinement_visitor : PMP::Autorefinement::Default_visitor {
  Autorefinement_visitor() : noot(), vtc(), ns() {}

  inline void number_of_output_triangles(std::size_t nbt) { noot(nbt); }
  inline void verbatim_triangle_copy(std::size_t tgt_id, std::size_t src_id) { vtc(tgt_id, src_id); }
  inline void new_subtriangle(std::size_t tgt_id, std::size_t src_id) { ns(tgt_id, src_id); }

  void set_number_of_output_triangles(const std::function<void(std::size_t)>& fn) { noot = fn; }
  void set_verbatim_triangle_copy(const std::function<void(std::size_t, std::size_t)>& fn) { vtc = fn; }
  void set_new_subtriangle(const std::function<void(std::size_t, std::size_t)>& fn) { ns = fn; }

 private:
  std::function<void(std::size_t)> noot;
  std::function<void(std::size_t, std::size_t)> vtc;
  std::function<void(std::size_t, std::size_t)> ns;
};

}  // namespace pmp

#endif  // CGALPY_AUTOREFINEMENT_VISITOR_HPP
