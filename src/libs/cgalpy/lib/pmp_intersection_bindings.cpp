// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/pair.h>

#include <CGAL/Polygon_mesh_processing/intersection.h>
#include <CGAL/Polygon_mesh_processing/self_intersections.h>

//! \todo remove
#include "CGALPY/pmp_np_parser.hpp"

#include "CGALPY/polygon_mesh_processing_types.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

using Point_3_vec = std::vector<Point_3>;

//!
template <typename PolygonMesh>
bool does_self_intersect(const PolygonMesh& pm,
                         const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::does_self_intersect(pm, internal::parse_pmp_np<Pm>(np));
}

//!
template <typename PolygonMesh>
bool does_self_intersect_faces(const std::vector<
                               typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                               const PolygonMesh& pm,
                               const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  return PMP::does_self_intersect(face_range, pm,
                                  internal::parse_pmp_np<PolygonMesh>(np));
}

//!
template <typename PolygonMesh>
auto self_intersections(const PolygonMesh& pm,
                            const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  std::vector<std::pair<Fd, Fd>> result;
  PMP::self_intersections(pm, std::back_inserter(result),
                          internal::parse_pmp_np<Pm>(np));
  return result;
}

//!
template <typename PolygonMesh>
auto self_intersections_faces(const std::vector<
                              typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                              const PolygonMesh& pm,
                              const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  std::vector<std::pair<Fd, Fd>> result;
  PMP::self_intersections(face_range, pm, std::back_inserter(result),
                          internal::parse_pmp_np<Pm>(np));
  return result;
}

//!
auto triangle_soup_self_intersections(const Point_3_vec& points,
                                      const std::vector<std::array<std::size_t, 3>>& triangles,
                                      const py::dict& np = py::dict()) {
  std::vector<std::pair<std::size_t, std::size_t>> result;
  PMP::triangle_soup_self_intersections(points, triangles,
                                        std::back_inserter(result));
  return result;
}

//!
auto does_triangle_soup_self_intersect(const Point_3_vec& points,
                                       const std::vector<std::array<std::size_t, 3>>& triangles,
                                       const py::dict& np = py::dict()) {
  return PMP::does_triangle_soup_self_intersect(points, triangles);
}

/*! Determine whether two polylines intersect.
 * It's a shame that we cannot pass the begin1,end1,begin2,end2
 * directly to the CGAL do_intersect function.
 */
bool do_intersect_polylines(const Point_3_vec& polyline1,
                            const Point_3_vec& polyline2)
{ return PMP::do_intersect(polyline1, polyline2); }

/*! Determine whether two ranges of polylines intersect.
 */
bool do_intersect_polyline_ranges(const std::vector<Point_3_vec>& range1,
                                  const std::vector<Point_3_vec>& range2) {
  return PMP::do_intersect(range1, range2);
}

//!
template <typename PolygonMesh>
bool do_intersect_meshes(const PolygonMesh& pm1, const PolygonMesh& pm2,
                         const py::dict& np1 = py::dict(),
                         const py::dict& np2 = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::do_intersect(pm1, pm2,
                           internal::parse_pmp_np<Pm>(np1),
                           internal::parse_pmp_np<Pm>(np2));
}

//!
template <typename PolygonMesh>
bool do_intersect_mesh_polyline(const PolygonMesh& pm,
                                const Point_3_vec& polyline,
                                const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::do_intersect(pm, polyline, internal::parse_pmp_np<Pm>(np));
}

//!
template <typename PolygonMesh>
bool do_intersect_mesh_polyline_range(const PolygonMesh& pm,
                                      const std::vector<Point_3_vec>& range,
                                      const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::do_intersect(pm, range, internal::parse_pmp_np<Pm>(np));
}

//!
template <typename PolygonMesh>
auto intersecting_meshes(const std::vector<PolygonMesh>& range,
                         const py::dict& np = py::dict(),
                         const std::vector<py::dict>& nps = std::vector<py::dict>()) {
  using Pm = PolygonMesh;
  std::vector<std::pair<std::size_t, std::size_t>> result;
  PMP::intersecting_meshes(range, std::back_inserter(result),
                           internal::parse_pmp_np<Pm>(np));
  return result;
}

}

/*!
 */
void export_pmp_intersection(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;

  m.def("do_intersect", &pmp::do_intersect_polylines);
  m.def("do_intersect_polyline_ranges", &pmp::do_intersect_polyline_ranges);
  m.def("do_intersect", &pmp::do_intersect_meshes<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());
  m.def("do_intersect", &pmp::do_intersect_mesh_polyline<Pm>,
        py::arg("pm"), py::arg("lst"),
        py::arg("np") = py::dict());
  m.def("do_intersect_polyline_range",
        &pmp::do_intersect_mesh_polyline_range<Pm>,
        py::arg("pm"), py::arg("lst"),
        py::arg("np") = py::dict());
  m.def("does_self_intersect", &pmp::does_self_intersect_faces<Pm>,
        py::arg("face_range"), py::arg("tmesh"),
        py::arg("np") = py::dict());
  m.def("does_self_intersect", &pmp::does_self_intersect<Pm>,
        py::arg("pm"), py::arg("np") = py::dict());
  m.def("does_triangle_soup_self_intersect", &pmp::does_triangle_soup_self_intersect, // TODO: point_map
        py::arg("points"), py::arg("triangles"), py::arg("np") = py::dict());
  m.def("intersecting_meshes", &pmp::intersecting_meshes<Pm>, py::arg("range"),
        py::arg("np") = py::dict(), py::arg("nps") = std::vector<py::dict>());
  m.def("self_intersections", &pmp::self_intersections<Pm>,
        py::arg("pm"), py::arg("np") = py::dict());
  m.def("self_intersections", &pmp::self_intersections_faces<Pm>,
        py::arg("face_range"), py::arg("pm"),
        py::arg("np") = py::dict());
  m.def("triangle_soup_self_intersections",
        &pmp::triangle_soup_self_intersections, // TODO: point_map
        py::arg("points"), py::arg("triangles"), py::arg("np") = py::dict());
}
