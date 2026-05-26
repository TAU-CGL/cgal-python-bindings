// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <vector>
#include <array>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/array.h>

#include <CGAL/Polygon_mesh_processing/distance.h>

//! \todo remove
#include "CGALPY/pmp_np_parser.hpp"

#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "cgalpy/Pmp_docstrings.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace pmp_doc = cgalpy::docstrings::Polygon_mesh_processing;

namespace pmp {

using Point_3_vec = std::vector<Point_3>;

//!
template <typename PolygonMesh>
double approximate_Hausdorff_distance(const PolygonMesh& tm1,
                                      const PolygonMesh& tm2,
                                      const py::dict& np1 = py::dict(),
                                      const py::dict& np2 = py::dict()) {
  using Pm = PolygonMesh;
  using TAG = CGAL::Sequential_tag;
  return PMP::approximate_Hausdorff_distance<TAG>(tm1, tm2,
                                             internal::parse_pmp_np<Pm>(np1),
                                             internal::parse_pmp_np<Pm>(np2));
}

  //!
template <typename TriangleMesh>
auto approximate_max_distance_to_point_set(const TriangleMesh& tm,
                                           const Point_3_vec& points,
                                           const double precision,
                                           const py::dict& np = py::dict()) {
  return PMP::approximate_max_distance_to_point_set(tm, points, precision,
                                                    internal::parse_pmp_np<TriangleMesh>(np));
}

//!
template <typename PolygonMesh>
auto approximate_symmetric_Hausdorff_distance(const PolygonMesh& tm1,
                                              const PolygonMesh& tm2,
                                              const py::dict& np1 = py::dict(),
                                              const py::dict& np2 = py::dict()) {
  using Pm = PolygonMesh;
  using TAG = CGAL::Sequential_tag;
  return PMP::approximate_symmetric_Hausdorff_distance<TAG>(tm1, tm2,
                                                            internal::parse_pmp_np<Pm>(np1),
                                                            internal::parse_pmp_np<Pm>(np2));
}

//!
template <typename PolygonMesh>
auto bounded_error_Hausdorff_distance(const PolygonMesh& tm1,
                                      const PolygonMesh& tm2,
                                      const double error_bound = 0.0001,
                                      const py::dict& np1 = py::dict(),
                                      const py::dict& np2 = py::dict()) {
  using Pm = PolygonMesh;
  using TAG = CGAL::Sequential_tag;
  return PMP::bounded_error_Hausdorff_distance<TAG>(tm1, tm2, error_bound,
                                                 internal::parse_pmp_np<Pm>(np1),
                                                 internal::parse_pmp_np<Pm>(np2));
}

//!
template <typename PolygonMesh>
auto is_Hausdorff_distance_larger(const PolygonMesh& tm1, const PolygonMesh& tm2,
                                  const double distance_bound,
                                  const double error_bound,
                                  const py::dict& np1 = py::dict(),
                                  const py::dict& np2 = py::dict()) {
  using Pm = PolygonMesh;
  using TAG = CGAL::Sequential_tag;
  return PMP::is_Hausdorff_distance_larger<TAG>(tm1, tm2, distance_bound, error_bound,
                                       internal::parse_pmp_np<Pm>(np1),
                                       internal::parse_pmp_np<Pm>(np2));
}

//!
template <typename TriangeMesh>
auto max_distance_to_triangle_mesh(const Point_3_vec& points,
                                   const TriangeMesh& tm,
                                   const py::dict& np = py::dict()) {
  using Tm = TriangeMesh;
  using TAG = CGAL::Sequential_tag;
  return PMP::max_distance_to_triangle_mesh<TAG>(points, tm,
                                                 internal::parse_pmp_np<Tm>(np));
}

//!
template <typename PolygonMesh>
auto sample_triangle_mesh(const PolygonMesh& tm,
                          const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  PointRange pts;
  PMP::sample_triangle_mesh(tm, std::back_inserter(pts),
                            internal::parse_pmp_np<Pm>(np));
  return pts;
}

//!
auto sample_triangle_soup(const Point_3_vec& points,
                          const std::vector<std::array<std::size_t, 3>>& triangles,
                          const py::dict& np = py::dict()) {
  PointRange pts;
  PMP::sample_triangle_soup(points, triangles, std::back_inserter(pts), internal::parse_named_parameters(np));
  return pts;
}

}

/*!
 */
void export_pmp_distance(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;

  m.def("approximate_Hausdorff_distance",
        &pmp::approximate_Hausdorff_distance<Pm>,
        py::arg("tm1"), py::arg("tm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        pmp_doc::Polygon_mesh_processing_approximate_Hausdorff_distance);
  m.def("approximate_max_distance_to_point_set",
        &pmp::approximate_max_distance_to_point_set<Pm>,
        py::arg("tm"), py::arg("points"), py::arg("precision"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_approximate_max_distance_to_point_set);
  m.def("approximate_symmetric_Hausdorff_distance",
        &pmp::approximate_symmetric_Hausdorff_distance<Pm>,
        py::arg("tm1"), py::arg("tm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        pmp_doc::Polygon_mesh_processing_approximate_symmetric_Hausdorff_distance);
  m.def("bounded_error_Hausdorff_distance",
        &pmp::bounded_error_Hausdorff_distance<Pm>,
        py::arg("tm1"), py::arg("tm2"), py::arg("error_bound") = 0.0001,
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        pmp_doc::Polygon_mesh_processing_bounded_error_Hausdorff_distance);
  m.def("is_Hausdorff_distance_larger", &pmp::is_Hausdorff_distance_larger<Pm>,
        py::arg("tm1"), py::arg("tm2"), py::arg("distance_bound"),
        py::arg("error_bound"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        pmp_doc::Polygon_mesh_processing_is_Hausdorff_distance_larger);
  m.def("max_distance_to_triangle_mesh", &pmp::max_distance_to_triangle_mesh<Pm>,
        py::arg("points"), py::arg("tm"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_max_distance_to_triangle_mesh);
  m.def("sample_triangle_mesh", &pmp::sample_triangle_mesh<Pm>,
        py::arg("tm"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_sample_triangle_mesh);
  m.def("sample_triangle_soup", &pmp::sample_triangle_soup,
        py::arg("points"), py::arg("triangles"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_sample_triangle_soup);
}
