#include "CGALPY/connected_components.h"
#include "CGALPY/pmp_helpers.hpp"
#include "CGALPY/pmp_np_parser.hpp"

namespace PMP = CGAL::Polygon_mesh_processing;
namespace py = nanobind;

namespace pmp {

//
template <typename PolygonMesh>
auto connected_component(typename boost::graph_traits<PolygonMesh>::face_descriptor seed_face,
                         PolygonMesh& pm,
                         const py::dict& np) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  std::vector<Fd> faces;
  auto it = std::back_inserter(faces);

  auto eicm = get_edge_prop_map<Pm, bool>(
      pm, "INTERNAL_MAP0", np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());

  PMP::connected_component(seed_face, pm, it, internal::parse_pmp_np<PolygonMesh>(np));
  return faces;
}

}  // namespace pmp
