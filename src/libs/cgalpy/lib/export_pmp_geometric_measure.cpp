// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#include <tuple>
#include <vector>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/tuple.h>

#include <CGAL/Polygon_mesh_processing/measure.h>

#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "cgalpy/Pmp_docstrings.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace pmp_doc = cgalpy::pmp::docstrings;

namespace cgalpy {
namespace pmp {

//!
template <typename TriangleMesh>
auto area(const TriangleMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::area(tm);
}

//!
template <typename TriangleMesh>
auto area_f(const std::vector<typename boost::graph_traits<TriangleMesh>::face_descriptor>& face_range,
            const TriangleMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<TriangleMesh>;
  using Fd = typename Gt::face_descriptor;
  return PMP::area(face_range, tm);
}

//!
template <typename TriangleMesh>
auto centroid(const TriangleMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::centroid(tm);
}

//!
template <typename TriangleMesh>
auto edge_length(typename boost::graph_traits<TriangleMesh>::halfedge_descriptor& e, const TriangleMesh& tm,
                 const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::edge_length(e, tm);
}

//!
template <typename TriangleMesh>
auto face_area(typename boost::graph_traits<TriangleMesh>::face_descriptor& f, const TriangleMesh& tm,
               const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::face_area(f, tm);
}

template <typename TriangleMesh>
auto face_aspect_ratio(typename boost::graph_traits<TriangleMesh>::face_descriptor& f, const TriangleMesh& tm,
                       const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::face_aspect_ratio(f, tm);
}

//!
template <typename TriangleMesh>
auto face_border_length(typename boost::graph_traits<TriangleMesh>::halfedge_descriptor& h, const TriangleMesh& tm,
                        const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::face_border_length(h, tm);
}

//!
template <typename TriangleMesh>
auto longest_border(const TriangleMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::longest_border(tm);
}

//!
template <typename TriangleMesh>
auto match_faces(const TriangleMesh& tm1, const TriangleMesh& tm2,
                 const py::dict& np1 = py::dict(), const py::dict& np2 = py::dict()) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Fd = typename Gt::face_descriptor;
  std::vector<std::pair<Fd, Fd>> common;
  std::vector<Fd> m1_only, m2_only;
  PMP::match_faces(tm1, tm2, std::back_inserter(common), std::back_inserter(m1_only), std::back_inserter(m2_only));
  return std::make_tuple(common, m1_only, m2_only);
}

//!
template <typename TriangleMesh>
auto squared_edge_length(typename boost::graph_traits<TriangleMesh>::edge_descriptor& e,
                         const TriangleMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::squared_edge_length(e, tm);
}

//!
template <typename TriangleMesh>
auto squared_face_area(typename boost::graph_traits<TriangleMesh>::face_descriptor& f,
                       const TriangleMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::squared_face_area(f, tm);
}

template <typename TriangleMesh>
auto volume(const TriangleMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::volume(tm);
}

}
} // namespace cgalpy

//!
void export_pmp_geometric_measure(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;

  m.def("area", &cgalpy::pmp::area<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_area_1);
  m.def("area", &cgalpy::pmp::area_f<Pm>,
        py::arg("face_range"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_area);
  m.def("centroid", &cgalpy::pmp::centroid<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_centroid);
  m.def("edge_length", &cgalpy::pmp::edge_length<Pm>,
        py::arg("h"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_edge_length);
  m.def("face_area", &cgalpy::pmp::face_area<Pm>,
        py::arg("f"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_face_area);
  m.def("face_aspect_ratio", &cgalpy::pmp::face_aspect_ratio<Pm>,
        py::arg("f"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_face_aspect_ratio);
  m.def("face_border_length", &cgalpy::pmp::face_border_length<Pm>,
        py::arg("h"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_face_border_length);
  m.def("longest_border", &cgalpy::pmp::longest_border<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_longest_border);
  m.def("match_faces", &cgalpy::pmp::match_faces<Pm>,
        py::arg("m1"), py::arg("m2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        pmp_doc::Polygon_mesh_processing_match_faces);
  m.def("squared_edge_length", &cgalpy::pmp::squared_edge_length<Pm>,
        py::arg("e"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_squared_edge_length);
  m.def("squared_face_area", &cgalpy::pmp::squared_face_area<Pm>,
        py::arg("f"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_squared_face_area);
  m.def("volume", &cgalpy::pmp::volume<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_volume);
}
