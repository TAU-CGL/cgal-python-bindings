// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#include <vector>
#include <functional>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/function.h>

#include <CGAL/Polygon_mesh_processing/repair_degeneracies.h>
#include <CGAL/Polygon_mesh_processing/repair.h>

#include "CGALPY/pmp_helpers.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

//!
template <typename PolygonMesh>
auto remove_almost_degenerate_faces_r(const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                                      PolygonMesh& pmesh,
                                      const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  using Pnt = Point_3;
  auto eicm = get_edge_prop_map<Pm, bool>
    (pmesh, "INTERNAL_MAP0",
     np.contains("edge_is_constrained_map") ?
     np["edge_is_constrained_map"] : py::none());
  auto vicm = get_vertex_prop_map<Pm, bool>
    (pmesh, "INTERNAL_MAP1",
     np.contains("vertex_is_constrained_map") ?
     np["vertex_is_constrained_map"] : py::none());
  std::function<bool(Pnt, Pnt, Pnt)> filter =
    [](const Point_3&, const Pnt&, const Pnt&) { return true; };
  if (np.contains("filter")) {
    try {
      filter = py::cast<std::function<bool(Pnt, Pnt, Pnt)>>(np["filter"]);
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Failed to cast filter to std::function<bool(Point_3, Point_3, Point_3)>");
    }
  }

  struct Filter {
    std::function<bool(Point_3, Point_3, Point_3)> filter;
    bool operator()(const Point_3& p0, const Point_3& p1, const Point_3& p2) const { return filter(p0, p1, p2); }
  };

  Filter f;
  f.filter = filter;
  auto retv = PMP::remove_almost_degenerate_faces(face_range, pmesh);

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  ! np.contains("edge_is_constrained_map") ?
    pmesh.remove_property_map(eicm) : void();
  ! np.contains("vertex_is_constrained_map") ?
    pmesh.remove_property_map(vicm) : void();
#endif

  return retv;
}

//!
template <typename TriangleMesh>
auto remove_almost_degenerate_faces(TriangleMesh& tmesh, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  auto eicm = get_edge_prop_map<Tm, bool>(tmesh, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ?
                                          np["edge_is_constrained_map"] : py::none());
  auto vicm = get_vertex_prop_map<TriangleMesh, bool>(tmesh, "INTERNAL_MAP1",
                                                      np.contains("vertex_is_constrained_map") ?
                                                      np["vertex_is_constrained_map"] : py::none());
  std::function<bool(Point_3, Point_3, Point_3)> filter =
    [](const Point_3&, const Point_3&, const Point_3&) { return true; };
  if (np.contains("filter")) {
    try {
      filter = py::cast<std::function<bool(Point_3, Point_3, Point_3)>>(np["filter"]);
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Failed to cast filter to std::function<bool(Point_3, Point_3, Point_3)>");
    }
  }
  struct Filter {
    std::function<bool(Point_3, Point_3, Point_3)> filter;
    bool operator()(const Point_3& p0, const Point_3& p1, const Point_3& p2) const
    { return filter(p0, p1, p2); }
  };
  Filter f; f.filter = filter;
  auto retv = PMP::remove_almost_degenerate_faces(tmesh);

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  ! np.contains("edge_is_constrained_map") ? tmesh.remove_property_map(eicm) : void();
  ! np.contains("vertex_is_constrained_map") ? tmesh.remove_property_map(vicm) : void();
#endif

  return retv;
}

//!
template <typename TriangleMesh>
auto remove_connected_components_of_negligible_size(TriangleMesh& tmesh,
                                                    const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  auto eicm = get_edge_prop_map<Tm, bool>(tmesh, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ?
                                          np["edge_is_constrained_map"] : py::none());
  bool fim_flag = np.contains("face_index_map");
  std::size_t retv;
  if (fim_flag) {
    auto fim = get_face_prop_map<Tm, std::size_t>(tmesh, "INTERNAL_MAP1", np["face_index_map"]);
    retv = PMP::remove_connected_components_of_negligible_size(tmesh);
  }
  else {
    retv = PMP::remove_connected_components_of_negligible_size(tmesh);
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  ! np.contains("edge_is_constrained_map") ? tmesh.remove_property_map(eicm) : void();
#endif

  return retv;
}

}

//!
void export_pmp_geometric_repair(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;

  m.def("remove_almost_degenerate_faces", &pmp::remove_almost_degenerate_faces_r<Pm>,
        py::arg("face_range"), py::arg("tmesh"), py::arg("np") = py::dict());
  m.def("remove_almost_degenerate_faces", &pmp::remove_almost_degenerate_faces<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict());
  m.def("remove_connected_components_of_negligible_size",
        &pmp::remove_connected_components_of_negligible_size<Pm>, // TODO: output_iterator
        py::arg("tmesh"), py::arg("np") = py::dict());
  m.def("remove_isolated_vertices", &PMP::remove_isolated_vertices<Pm>,
        py::arg("pmesh"));
}
