// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#include <vector>
#include <array>
#include <utility>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/array.h>

#include <CGAL/Polygon_mesh_processing/distance.h>

#include "cgalpy/Named_parameter_geom_traits.hpp"
#include "cgalpy/Named_parameter_vertex_point_map.hpp"
#include "cgalpy/Named_parameter_wrapper.hpp"
#include "cgalpy/named_parameter_applicator.hpp"
#include "cgalpy/polygon_mesh_processing_types.hpp"
#include "cgalpy/ndarray_to_point_3_vector.hpp"
#include "cgalpy/Pmp_docstrings.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace pmp_doc = cgalpy::pmp::docstrings;

namespace cgalpy {
namespace pmp {

using Point_3_vec = std::vector<Point_3>;

/*! Apply vertex_point_map and geom_traits to a single-mesh PMP
 * distance wrapper.
 */
template <typename PolygonMesh, template <typename...> class Wrapper,
          typename... Args>
auto apply_distance_named_parameters(const py::dict& params, Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_vertex_point_map<PolygonMesh> vertex_point_map_op;
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator(wrapper, np, params,
                                            vertex_point_map_op,
                                            geom_traits_op);
}

/*! Apply geom_traits to a PMP distance wrapper that has no mesh vertex point
 * map, such as triangle soup sampling.
 */
template <template <typename...> class Wrapper, typename... Args>
auto apply_distance_geom_traits(const py::dict& params, Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator(wrapper, np, params,
                                            geom_traits_op);
}

/*! A class template that wraps
 * CGAL::Polygon_mesh_processing::approximate_max_distance_to_point_set().
 */
template <typename NamedParameter, typename... Args>
struct Approximate_max_distance_to_point_set_wrapper;

template <typename NamedParameter, typename TriangleMesh, typename PointRange,
          typename Precision>
struct Approximate_max_distance_to_point_set_wrapper<NamedParameter,
                                                     TriangleMesh,
                                                     PointRange,
                                                     Precision> {
  static auto call(NamedParameter& np, TriangleMesh&& tm,
                   PointRange&& points, Precision&& precision)
  {
    return PMP::approximate_max_distance_to_point_set
      (std::forward<TriangleMesh>(tm), std::forward<PointRange>(points),
       std::forward<Precision>(precision), np);
  }
};

/*! A class template that wraps
 * CGAL::Polygon_mesh_processing::max_distance_to_triangle_mesh().
 */
template <typename NamedParameter, typename... Args>
struct Max_distance_to_triangle_mesh_wrapper;

template <typename NamedParameter, typename PointRange, typename TriangleMesh>
struct Max_distance_to_triangle_mesh_wrapper<NamedParameter, PointRange,
                                             TriangleMesh> {
  static auto call(NamedParameter& np, PointRange&& points, TriangleMesh&& tm)
  {
    using TAG = CGAL::Sequential_tag;
    return PMP::max_distance_to_triangle_mesh<TAG>
      (std::forward<PointRange>(points), std::forward<TriangleMesh>(tm), np);
  }
};

/*! A class template that wraps
 * CGAL::Polygon_mesh_processing::sample_triangle_mesh().
 */
template <typename NamedParameter, typename... Args>
struct Sample_triangle_mesh_wrapper;

template <typename NamedParameter, typename PolygonMesh>
struct Sample_triangle_mesh_wrapper<NamedParameter, PolygonMesh, PointRange&> {
  static auto call(NamedParameter& np, PolygonMesh&& tm, PointRange& pts)
  {
    PMP::sample_triangle_mesh(std::forward<PolygonMesh>(tm),
                              std::back_inserter(pts), np);
  }
};

/*! A class template that wraps
 * CGAL::Polygon_mesh_processing::sample_triangle_soup().
 */
template <typename NamedParameter, typename... Args>
struct Sample_triangle_soup_wrapper;

template <typename NamedParameter, typename PointRangeInput,
          typename TriangleRange>
struct Sample_triangle_soup_wrapper<NamedParameter, PointRangeInput,
                                    TriangleRange, PointRange&> {
  static auto call(NamedParameter& np, PointRangeInput&& points,
                   TriangleRange&& triangles, PointRange& pts)
  {
    PMP::sample_triangle_soup(std::forward<PointRangeInput>(points),
                              std::forward<TriangleRange>(triangles),
                              std::back_inserter(pts), np);
  }
};

//!
template <typename PolygonMesh>
double approximate_Hausdorff_distance(const PolygonMesh& tm1, const PolygonMesh& tm2,
                                      const py::dict& np1 = py::dict(), const py::dict& np2 = py::dict()) {
  using Pm = PolygonMesh;
  using TAG = CGAL::Sequential_tag;
  return PMP::approximate_Hausdorff_distance<TAG>(tm1, tm2);
}

  //!
template <typename TriangleMesh>
auto approximate_max_distance_to_point_set(const TriangleMesh& tm, const Point_3_vec& points,
                                           const double precision, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return apply_distance_named_parameters<Tm, Approximate_max_distance_to_point_set_wrapper>(np, tm, points, precision);
}

//!
template <typename TriangleMesh>
auto approximate_max_distance_to_point_set_np(const TriangleMesh& tm,
                                              const py::ndarray<>& points_array,
                                              const double precision,
                                              const py::dict& np = py::dict()) {
  auto points = cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return approximate_max_distance_to_point_set(tm, points, precision, np);
}

//!
template <typename PolygonMesh>
auto approximate_symmetric_Hausdorff_distance(const PolygonMesh& tm1, const PolygonMesh& tm2,
                                              const py::dict& np1 = py::dict(), const py::dict& np2 = py::dict()) {
  using Pm = PolygonMesh;
  using TAG = CGAL::Sequential_tag;
  return PMP::approximate_symmetric_Hausdorff_distance<TAG>(tm1, tm2);
}

//!
template <typename PolygonMesh>
auto bounded_error_Hausdorff_distance(const PolygonMesh& tm1, const PolygonMesh& tm2, const double error_bound = 0.0001,
                                      const py::dict& np1 = py::dict(), const py::dict& np2 = py::dict()) {
  using Pm = PolygonMesh;
  using TAG = CGAL::Sequential_tag;
  return PMP::bounded_error_Hausdorff_distance<TAG>(tm1, tm2, error_bound);
}

//!
template <typename PolygonMesh>
auto is_Hausdorff_distance_larger(const PolygonMesh& tm1, const PolygonMesh& tm2, const double distance_bound,
                                  const double error_bound,
                                  const py::dict& np1 = py::dict(), const py::dict& np2 = py::dict()) {
  using Pm = PolygonMesh;
  using TAG = CGAL::Sequential_tag;
  return PMP::is_Hausdorff_distance_larger<TAG>(tm1, tm2, distance_bound, error_bound);
}

//!
template <typename TriangeMesh>
auto max_distance_to_triangle_mesh(const Point_3_vec& points, const TriangeMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangeMesh;
  return apply_distance_named_parameters<Tm, Max_distance_to_triangle_mesh_wrapper>(np, points, tm);
}

//!
template <typename TriangeMesh>
auto max_distance_to_triangle_mesh_np(const py::ndarray<>& points_array,
                                      const TriangeMesh& tm,
                                      const py::dict& np = py::dict()) {
  auto points = cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return max_distance_to_triangle_mesh(points, tm, np);
}

//!
template <typename PolygonMesh>
auto sample_triangle_mesh(const PolygonMesh& tm, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  PointRange pts;
  apply_distance_named_parameters<Pm, Sample_triangle_mesh_wrapper>(np, tm, pts);
  return pts;
}

//!
auto sample_triangle_soup(const Point_3_vec& points,
                          const std::vector<std::array<std::size_t, 3>>& triangles, const py::dict& np = py::dict()) {
  PointRange pts;
  apply_distance_geom_traits<Sample_triangle_soup_wrapper>(np, points, triangles, pts);
  return pts;
}


//!
auto sample_triangle_soup_np(const py::ndarray<>& points_array,
                             const std::vector<std::array<std::size_t, 3>>& triangles,
                             const py::dict& np = py::dict()) {
  auto points = cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return sample_triangle_soup(points, triangles, np);
}

}
} // namespace cgalpy

/*!
 */
void export_pmp_distance(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;

  m.def("approximate_Hausdorff_distance", &cgalpy::pmp::approximate_Hausdorff_distance<Pm>,
        py::arg("tm1"), py::arg("tm2"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        pmp_doc::Polygon_mesh_processing_approximate_Hausdorff_distance);
  m.def("approximate_max_distance_to_point_set", &cgalpy::pmp::approximate_max_distance_to_point_set<Pm>,
        py::arg("tm"), py::arg("points"), py::arg("precision"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_approximate_max_distance_to_point_set);
  m.def("approximate_max_distance_to_point_set", &cgalpy::pmp::approximate_max_distance_to_point_set_np<Pm>,
        py::arg("tm"), py::arg("points"), py::arg("precision"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_approximate_max_distance_to_point_set);
  m.def("approximate_symmetric_Hausdorff_distance", &cgalpy::pmp::approximate_symmetric_Hausdorff_distance<Pm>,
        py::arg("tm1"), py::arg("tm2"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        pmp_doc::Polygon_mesh_processing_approximate_symmetric_Hausdorff_distance);
  m.def("bounded_error_Hausdorff_distance", &cgalpy::pmp::bounded_error_Hausdorff_distance<Pm>,
        py::arg("tm1"), py::arg("tm2"), py::arg("error_bound") = 0.0001,
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        pmp_doc::Polygon_mesh_processing_bounded_error_Hausdorff_distance);
  m.def("is_Hausdorff_distance_larger", &cgalpy::pmp::is_Hausdorff_distance_larger<Pm>,
        py::arg("tm1"), py::arg("tm2"), py::arg("distance_bound"), py::arg("error_bound"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        pmp_doc::Polygon_mesh_processing_is_Hausdorff_distance_larger);
  m.def("max_distance_to_triangle_mesh", &cgalpy::pmp::max_distance_to_triangle_mesh<Pm>,
        py::arg("points"), py::arg("tm"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_max_distance_to_triangle_mesh);
  m.def("max_distance_to_triangle_mesh", &cgalpy::pmp::max_distance_to_triangle_mesh_np<Pm>,
        py::arg("points"), py::arg("tm"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_max_distance_to_triangle_mesh);
  m.def("sample_triangle_mesh", &cgalpy::pmp::sample_triangle_mesh<Pm>,
        py::arg("tm"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_sample_triangle_mesh);
  m.def("sample_triangle_soup", &cgalpy::pmp::sample_triangle_soup,
        py::arg("points"), py::arg("triangles"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_sample_triangle_soup);
  m.def("sample_triangle_soup", &cgalpy::pmp::sample_triangle_soup_np,
        py::arg("points"), py::arg("triangles"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_sample_triangle_soup);
}
