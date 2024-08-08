#ifndef CGALPY_POLYHEDRAL_ENVELOPE_HPP
#define CGALPY_POLYHEDRAL_ENVELOPE_HPP

#include <nanobind/nanobind.h>
#include "CGALPY/pmp_np_parser.hpp"
#include "CGALPY/pmp_helpers.hpp"
#include "CGALPY/helpers.hpp"
#include <CGAL/Polyhedral_envelope.h>

namespace py = nanobind;

namespace pmp {

template <typename TriangleMesh, typename GeomTraits>
struct Polyhedral_envelope : CGAL::Polyhedral_envelope<GeomTraits> {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Fd = typename Gt::face_descriptor;
  Polyhedral_envelope(TriangleMesh& tmesh, double epsilon, const py::dict& np = py::dict())// {
    : CGAL::Polyhedral_envelope<GeomTraits>
    (tmesh, epsilon,
     internal::parse_pmp_np<TriangleMesh>(np)
     .face_epsilon_map(
     get_face_prop_map<TriangleMesh, double>
     (tmesh, "INTERNAL_MAP0",
      np.contains("face_epsilon_map") ? np["face_epsilon_map"] : py::none(),
      epsilon)))
  {
    // the map isn't deleted! and there is no way to delete it
  }

  Polyhedral_envelope(const py::list& face_range, TriangleMesh& tmesh, double epsilon, const py::dict& np = py::dict())// {
    : CGAL::Polyhedral_envelope<GeomTraits>
     (list2vec<Fd>(face_range), tmesh, epsilon,
      internal::parse_pmp_np<TriangleMesh>(np)
      .face_epsilon_map(
      get_face_prop_map<TriangleMesh, double>
      (tmesh, "INTERNAL_MAP0",
       np.contains("face_epsilon_map") ? np["face_epsilon_map"] : py::none(),
       epsilon)))
  {
    // the map isn't deleted! and there is no way to delete it
  }
};

} // namespace pmp


#endif // CGALPY_POLYHEDRAL_ENVELOPE_HPP
