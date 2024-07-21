#ifndef CGALPY_ADAPTIVE_SIZING_FIELD_HPP
#define CGALPY_ADAPTIVE_SIZING_FIELD_HPP

#include <nanobind/nanobind.h>
#include <CGAL/Polygon_mesh_processing/Adaptive_sizing_field.h>

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

template <typename Pm>
struct Adaptive_sizing_field : public PMP::Adaptive_sizing_field<Pm> {
  using my_fd = typename boost::graph_traits<Pm>::face_descriptor;
private:
  static auto list2vec(const py::list& lst) {
    std::vector<my_fd> vec;
    vec.reserve(py::len(lst));
    for (const auto& item : lst) {
      vec.push_back(py::cast<my_fd>(item));
    }
    return vec;
  }

public:
  Adaptive_sizing_field(double tol, py::tuple edge_min_max, py::list faces, Pm& mesh) :
    PMP::Adaptive_sizing_field<Pm>(tol,
                                    std::make_pair(py::cast<double>(edge_min_max[0]), py::cast<double>(edge_min_max[1])),
                                    list2vec(faces),
                                    mesh) {};

};

} // namespace pmp

#endif // CGALPY_ADAPTIVE_SIZING_FIELD_HPP

