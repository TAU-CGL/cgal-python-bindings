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

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/pair.h>

#include <CGAL/Polygon_mesh_processing/intersection.h>
#include <CGAL/Polygon_mesh_processing/self_intersections.h>

#include "cgalpy/Named_parameter_geom_traits.hpp"
#include "cgalpy/Named_parameter_maximum_number.hpp"
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

//! Apply simple intersection named parameters.
template <template <typename...> class Wrapper, typename... Args>
auto apply_intersection_geom_traits_named_parameters(const py::dict& params,
                                                     Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator(wrapper, np, params,
                                            geom_traits_op);
}

//! Apply simple self-intersection named parameters.
template <template <typename...> class Wrapper, typename... Args>
auto apply_self_intersections_named_parameters(const py::dict& params,
                                               Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_maximum_number maximum_number_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator(wrapper, np, params,
                                            geom_traits_op,
                                            maximum_number_op);
}

//! Wrap CGAL::Polygon_mesh_processing::does_self_intersect(tmesh, np).
template <typename NamedParameter, typename... Args>
struct Does_self_intersect_wrapper;

template <typename NamedParameter, typename PolygonMesh>
struct Does_self_intersect_wrapper<NamedParameter, PolygonMesh> {
  static auto call(NamedParameter& np, PolygonMesh&& pm)
  {
    return PMP::does_self_intersect(std::forward<PolygonMesh>(pm), np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::does_self_intersect(face_range, tmesh, np).
template <typename NamedParameter, typename... Args>
struct Does_self_intersect_faces_wrapper;

template <typename NamedParameter, typename FaceRange, typename PolygonMesh>
struct Does_self_intersect_faces_wrapper<NamedParameter, FaceRange, PolygonMesh> {
  static auto call(NamedParameter& np, FaceRange&& face_range, PolygonMesh&& pm)
  {
    return PMP::does_self_intersect(std::forward<FaceRange>(face_range),
                                    std::forward<PolygonMesh>(pm), np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::self_intersections(tmesh, out, np).
template <typename NamedParameter, typename... Args>
struct Self_intersections_wrapper;

template <typename NamedParameter, typename PolygonMesh, typename OutputIterator>
struct Self_intersections_wrapper<NamedParameter, PolygonMesh, OutputIterator> {
  static auto call(NamedParameter& np, PolygonMesh&& pm, OutputIterator&& out)
  {
    return PMP::self_intersections(std::forward<PolygonMesh>(pm),
                                   std::forward<OutputIterator>(out), np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::self_intersections(face_range, tmesh, out, np).
template <typename NamedParameter, typename... Args>
struct Self_intersections_faces_wrapper;

template <typename NamedParameter, typename FaceRange, typename PolygonMesh,
          typename OutputIterator>
struct Self_intersections_faces_wrapper<NamedParameter, FaceRange, PolygonMesh,
                                        OutputIterator> {
  static auto call(NamedParameter& np, FaceRange&& face_range, PolygonMesh&& pm,
                   OutputIterator&& out)
  {
    return PMP::self_intersections(std::forward<FaceRange>(face_range),
                                   std::forward<PolygonMesh>(pm),
                                   std::forward<OutputIterator>(out), np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::triangle_soup_self_intersections().
template <typename NamedParameter, typename... Args>
struct Triangle_soup_self_intersections_wrapper;

template <typename NamedParameter, typename PointRange, typename TriangleRange,
          typename OutputIterator>
struct Triangle_soup_self_intersections_wrapper<NamedParameter, PointRange,
                                                TriangleRange, OutputIterator> {
  static auto call(NamedParameter& np, PointRange&& points,
                   TriangleRange&& triangles, OutputIterator&& out)
  {
    return PMP::triangle_soup_self_intersections
      (std::forward<PointRange>(points), std::forward<TriangleRange>(triangles),
       std::forward<OutputIterator>(out), np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::does_triangle_soup_self_intersect().
template <typename NamedParameter, typename... Args>
struct Does_triangle_soup_self_intersect_wrapper;

template <typename NamedParameter, typename PointRange, typename TriangleRange>
struct Does_triangle_soup_self_intersect_wrapper<NamedParameter, PointRange,
                                                 TriangleRange> {
  static auto call(NamedParameter& np, PointRange&& points,
                   TriangleRange&& triangles)
  {
    return PMP::does_triangle_soup_self_intersect
      (std::forward<PointRange>(points), std::forward<TriangleRange>(triangles),
       np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::do_intersect(tmesh, polyline/range, np).
template <typename NamedParameter, typename... Args>
struct Do_intersect_mesh_polyline_wrapper;

template <typename NamedParameter, typename PolygonMesh, typename PolylineRange>
struct Do_intersect_mesh_polyline_wrapper<NamedParameter, PolygonMesh,
                                          PolylineRange> {
  static auto call(NamedParameter& np, PolygonMesh&& pm,
                   PolylineRange&& polyline_range)
  {
    return PMP::do_intersect(std::forward<PolygonMesh>(pm),
                             std::forward<PolylineRange>(polyline_range), np);
  }
};

//!
template <typename PolygonMesh>
bool does_self_intersect(const PolygonMesh& pm, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return apply_intersection_geom_traits_named_parameters
    <Does_self_intersect_wrapper>(np, pm);
}

//!
template <typename PolygonMesh>
bool does_self_intersect_faces(const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                               const PolygonMesh& pm, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  return apply_intersection_geom_traits_named_parameters
    <Does_self_intersect_faces_wrapper>(np, face_range, pm);
}

//!
template <typename PolygonMesh>
auto self_intersections(const PolygonMesh& pm, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  std::vector<std::pair<Fd, Fd>> result;
  apply_self_intersections_named_parameters
    <Self_intersections_wrapper>(np, pm, std::back_inserter(result));
  return result;
}

//!
template <typename PolygonMesh>
auto self_intersections_faces(const std::vector<
                              typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                              const PolygonMesh& pm, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  std::vector<std::pair<Fd, Fd>> result;
  apply_self_intersections_named_parameters
    <Self_intersections_faces_wrapper>
    (np, face_range, pm, std::back_inserter(result));
  return result;
}

//!
auto triangle_soup_self_intersections(const Point_3_vec& points,
                                      const std::vector<std::array<std::size_t, 3>>& triangles,
                                      const py::dict& np = py::dict()) {
  std::vector<std::pair<std::size_t, std::size_t>> result;
  apply_self_intersections_named_parameters
    <Triangle_soup_self_intersections_wrapper>
    (np, points, triangles, std::back_inserter(result));
  return result;
}

//!
auto does_triangle_soup_self_intersect(const Point_3_vec& points,
                                       const std::vector<std::array<std::size_t, 3>>& triangles,
                                       const py::dict& np = py::dict()) {
  return apply_intersection_geom_traits_named_parameters
    <Does_triangle_soup_self_intersect_wrapper>(np, points, triangles);
}


//!
auto triangle_soup_self_intersections_np(const py::ndarray<>& points_array,
                                         const std::vector<std::array<std::size_t, 3>>& triangles,
                                         const py::dict& np = py::dict()) {
  auto points = cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return triangle_soup_self_intersections(points, triangles, np);
}

//!
auto does_triangle_soup_self_intersect_np(const py::ndarray<>& points_array,
                                          const std::vector<std::array<std::size_t, 3>>& triangles,
                                          const py::dict& np = py::dict()) {
  auto points = cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return does_triangle_soup_self_intersect(points, triangles, np);
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
bool do_intersect_polyline_ranges(const std::vector<Point_3_vec>& range1, const std::vector<Point_3_vec>& range2) {
  return PMP::do_intersect(range1, range2);
}

//!
template <typename PolygonMesh>
bool do_intersect_meshes(const PolygonMesh& pm1, const PolygonMesh& pm2,
                         const py::dict& np1 = py::dict(), const py::dict& np2 = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::do_intersect(pm1, pm2);
}

//!
template <typename PolygonMesh>
bool do_intersect_mesh_polyline(const PolygonMesh& pm, const Point_3_vec& polyline, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return apply_intersection_geom_traits_named_parameters
    <Do_intersect_mesh_polyline_wrapper>(np, pm, polyline);
}

//!
template <typename PolygonMesh>
bool do_intersect_mesh_polyline_range(const PolygonMesh& pm, const std::vector<Point_3_vec>& range,
                                      const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return apply_intersection_geom_traits_named_parameters
    <Do_intersect_mesh_polyline_wrapper>(np, pm, range);
}

//!
template <typename PolygonMesh>
auto intersecting_meshes(const std::vector<PolygonMesh>& range, const py::dict& np = py::dict(),
                         const std::vector<py::dict>& nps = std::vector<py::dict>()) {
  using Pm = PolygonMesh;
  std::vector<std::pair<std::size_t, std::size_t>> result;
  PMP::intersecting_meshes(range, std::back_inserter(result));
  return result;
}

}
} // namespace cgalpy

/*!
 */
void export_pmp_intersection(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;

  m.def("do_intersect", &cgalpy::pmp::do_intersect_polylines,
        py::arg("polyline1"), py::arg("polyline2"),
        pmp_doc::Polygon_mesh_processing_do_intersect_1);
  m.def("do_intersect_polyline_ranges", &cgalpy::pmp::do_intersect_polyline_ranges,
        py::arg("polylines1"), py::arg("polylines2"),
        pmp_doc::Polygon_mesh_processing_do_intersect);
  m.def("do_intersect", &cgalpy::pmp::do_intersect_meshes<Pm>,
        py::arg("tm1"), py::arg("tm2"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        pmp_doc::Polygon_mesh_processing_do_intersect_2);
  m.def("do_intersect", &cgalpy::pmp::do_intersect_mesh_polyline<Pm>,
        py::arg("tm"), py::arg("polyline"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_do_intersect_4);
  m.def("do_intersect_polyline_range",
        &cgalpy::pmp::do_intersect_mesh_polyline_range<Pm>,
        py::arg("tm"), py::arg("polylines"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_do_intersect_3);
  m.def("does_self_intersect", &cgalpy::pmp::does_self_intersect_faces<Pm>,
        py::arg("face_range"), py::arg("tmesh"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_does_self_intersect);
  m.def("does_self_intersect", &cgalpy::pmp::does_self_intersect<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_does_self_intersect_1);
  m.def("does_triangle_soup_self_intersect", &cgalpy::pmp::does_triangle_soup_self_intersect,
        py::arg("points"), py::arg("triangles"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_does_triangle_soup_self_intersect);
  m.def("does_triangle_soup_self_intersect", &cgalpy::pmp::does_triangle_soup_self_intersect_np,
        py::arg("points"), py::arg("triangles"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_does_triangle_soup_self_intersect);
  m.def("intersecting_meshes", &cgalpy::pmp::intersecting_meshes<Pm>, py::arg("range"),
        py::arg("np") = py::dict(), py::arg("nps") = std::vector<py::dict>(),
        pmp_doc::Polygon_mesh_processing_intersecting_meshes_2);
  m.def("self_intersections", &cgalpy::pmp::self_intersections<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_self_intersections_1);
  m.def("self_intersections", &cgalpy::pmp::self_intersections_faces<Pm>,
        py::arg("face_range"), py::arg("tmesh"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_self_intersections);
  m.def("triangle_soup_self_intersections", &cgalpy::pmp::triangle_soup_self_intersections,
        py::arg("points"), py::arg("triangles"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_triangle_soup_self_intersections);
  m.def("triangle_soup_self_intersections", &cgalpy::pmp::triangle_soup_self_intersections_np,
        py::arg("points"), py::arg("triangles"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_triangle_soup_self_intersections);
}
