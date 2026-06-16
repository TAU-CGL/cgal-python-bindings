// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com>
//            Utkarsh Khajuria <utkarshkhajuria7@gmail.com>

#include <tuple>
#include <vector>
#include <utility>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/tuple.h>

#include <CGAL/Polygon_mesh_processing/measure.h>

#include "cgalpy/Named_parameter_geom_traits.hpp"
#include "cgalpy/Named_parameter_vertex_point_map.hpp"
#include "cgalpy/Named_parameter_wrapper.hpp"
#include "cgalpy/named_parameter_applicator.hpp"
#include "cgalpy/polygon_mesh_processing_types.hpp"
#include "cgalpy/Pmp_docstrings.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace pmp_doc = cgalpy::pmp::docstrings;

namespace cgalpy {
namespace pmp {

/*! Apply the vertex_point_map and geom_traits named parameters to a PMP
 * measure wrapper.
 */
template <typename PolygonMesh, template <typename...> class Wrapper,
          typename... Args>
auto apply_measure_named_parameters(const py::dict& params, Args&&... args)
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

/*! A class template that wraps the full-mesh overload of
 * CGAL::Polygon_mesh_processing::area()
 */
template <typename NamedParameter, typename... Args>
struct Area_mesh_wrapper;

template <typename NamedParameter, typename TriangleMesh>
struct Area_mesh_wrapper<NamedParameter, TriangleMesh> {
  static auto call(NamedParameter& np, TriangleMesh&& tm)
  {
    return PMP::area(faces(tm), std::forward<TriangleMesh>(tm), np);
  }
};

/*! A class template that wraps the face-range overload of
 * CGAL::Polygon_mesh_processing::area()
 */
template <typename NamedParameter, typename... Args>
struct Area_range_wrapper;

template <typename NamedParameter, typename FaceRange, typename TriangleMesh>
struct Area_range_wrapper<NamedParameter, FaceRange, TriangleMesh> {
  static auto call(NamedParameter& np, FaceRange&& face_range,
                   TriangleMesh&& tm)
  {
    return PMP::area(std::forward<FaceRange>(face_range),
                     std::forward<TriangleMesh>(tm), np);
  }
};

/*! A class template that wraps CGAL::Polygon_mesh_processing::centroid(). */
template <typename NamedParameter, typename... Args>
struct Centroid_wrapper;

template <typename NamedParameter, typename TriangleMesh>
struct Centroid_wrapper<NamedParameter, TriangleMesh> {
  static auto call(NamedParameter& np, TriangleMesh&& tm)
  { return PMP::centroid(std::forward<TriangleMesh>(tm), np); }
};

/*! A class template that wraps CGAL::Polygon_mesh_processing::edge_length(). */
template <typename NamedParameter, typename... Args>
struct Edge_length_wrapper;

template <typename NamedParameter, typename HalfedgeDescriptor,
          typename TriangleMesh>
struct Edge_length_wrapper<NamedParameter, HalfedgeDescriptor, TriangleMesh> {
  static auto call(NamedParameter& np, HalfedgeDescriptor&& h,
                   TriangleMesh&& tm)
  {
    return PMP::edge_length(std::forward<HalfedgeDescriptor>(h),
                            std::forward<TriangleMesh>(tm), np);
  }
};

/*! A class template that wraps CGAL::Polygon_mesh_processing::face_area(). */
template <typename NamedParameter, typename... Args>
struct Face_area_wrapper;

template <typename NamedParameter, typename FaceDescriptor,
          typename TriangleMesh>
struct Face_area_wrapper<NamedParameter, FaceDescriptor, TriangleMesh> {
  static auto call(NamedParameter& np, FaceDescriptor&& f, TriangleMesh&& tm)
  {
    return PMP::face_area(std::forward<FaceDescriptor>(f),
                          std::forward<TriangleMesh>(tm), np);
  }
};

/*! A class template that wraps
 * CGAL::Polygon_mesh_processing::face_aspect_ratio().
 */
template <typename NamedParameter, typename... Args>
struct Face_aspect_ratio_wrapper;

template <typename NamedParameter, typename FaceDescriptor,
          typename TriangleMesh>
struct Face_aspect_ratio_wrapper<NamedParameter, FaceDescriptor,
                                 TriangleMesh> {
  static auto call(NamedParameter& np, FaceDescriptor&& f, TriangleMesh&& tm)
  {
    return PMP::face_aspect_ratio(std::forward<FaceDescriptor>(f),
                                  std::forward<TriangleMesh>(tm), np);
  }
};

/*! A class template that wraps
 * CGAL::Polygon_mesh_processing::face_border_length().
 */
template <typename NamedParameter, typename... Args>
struct Face_border_length_wrapper;

template <typename NamedParameter, typename HalfedgeDescriptor,
          typename TriangleMesh>
struct Face_border_length_wrapper<NamedParameter, HalfedgeDescriptor,
                                  TriangleMesh> {
  static auto call(NamedParameter& np, HalfedgeDescriptor&& h,
                   TriangleMesh&& tm)
  {
    return PMP::face_border_length(std::forward<HalfedgeDescriptor>(h),
                                   std::forward<TriangleMesh>(tm), np);
  }
};

/*! A class template that wraps CGAL::Polygon_mesh_processing::longest_border(). */
template <typename NamedParameter, typename... Args>
struct Longest_border_wrapper;

template <typename NamedParameter, typename TriangleMesh>
struct Longest_border_wrapper<NamedParameter, TriangleMesh> {
  static auto call(NamedParameter& np, TriangleMesh&& tm)
  { return PMP::longest_border(std::forward<TriangleMesh>(tm), np); }
};

/*! A class template that wraps
 * CGAL::Polygon_mesh_processing::squared_edge_length().
 */
template <typename NamedParameter, typename... Args>
struct Squared_edge_length_wrapper;

template <typename NamedParameter, typename EdgeDescriptor,
          typename TriangleMesh>
struct Squared_edge_length_wrapper<NamedParameter, EdgeDescriptor,
                                   TriangleMesh> {
  static auto call(NamedParameter& np, EdgeDescriptor&& e, TriangleMesh&& tm)
  {
    return PMP::squared_edge_length(std::forward<EdgeDescriptor>(e),
                                    std::forward<TriangleMesh>(tm), np);
  }
};

/*! A class template that wraps
 * CGAL::Polygon_mesh_processing::squared_face_area().
 */
template <typename NamedParameter, typename... Args>
struct Squared_face_area_wrapper;

template <typename NamedParameter, typename FaceDescriptor,
          typename TriangleMesh>
struct Squared_face_area_wrapper<NamedParameter, FaceDescriptor,
                                 TriangleMesh> {
  static auto call(NamedParameter& np, FaceDescriptor&& f, TriangleMesh&& tm)
  {
    return PMP::squared_face_area(std::forward<FaceDescriptor>(f),
                                  std::forward<TriangleMesh>(tm), np);
  }
};

/*! A class template that wraps CGAL::Polygon_mesh_processing::volume(). */
template <typename NamedParameter, typename... Args>
struct Volume_wrapper;

template <typename NamedParameter, typename TriangleMesh>
struct Volume_wrapper<NamedParameter, TriangleMesh> {
  static auto call(NamedParameter& np, TriangleMesh&& tm)
  { return PMP::volume(std::forward<TriangleMesh>(tm), np); }
};

//!
template <typename TriangleMesh>
auto area(const TriangleMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return apply_measure_named_parameters<Tm, Area_mesh_wrapper>(np, tm);
}

//!
template <typename TriangleMesh>
auto area_f(const std::vector<typename boost::graph_traits<TriangleMesh>::face_descriptor>& face_range,
            const TriangleMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<TriangleMesh>;
  using Fd = typename Gt::face_descriptor;
  return apply_measure_named_parameters<Tm, Area_range_wrapper>(np, face_range, tm);
}

//!
template <typename TriangleMesh>
auto centroid(const TriangleMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return apply_measure_named_parameters<Tm, Centroid_wrapper>(np, tm);
}

//!
template <typename TriangleMesh>
auto edge_length(typename boost::graph_traits<TriangleMesh>::halfedge_descriptor& e, const TriangleMesh& tm,
                 const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return apply_measure_named_parameters<Tm, Edge_length_wrapper>(np, e, tm);
}

//!
template <typename TriangleMesh>
auto face_area(typename boost::graph_traits<TriangleMesh>::face_descriptor& f, const TriangleMesh& tm,
               const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return apply_measure_named_parameters<Tm, Face_area_wrapper>(np, f, tm);
}

template <typename TriangleMesh>
auto face_aspect_ratio(typename boost::graph_traits<TriangleMesh>::face_descriptor& f, const TriangleMesh& tm,
                       const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return apply_measure_named_parameters<Tm, Face_aspect_ratio_wrapper>(np, f, tm);
}

//!
template <typename TriangleMesh>
auto face_border_length(typename boost::graph_traits<TriangleMesh>::halfedge_descriptor& h, const TriangleMesh& tm,
                        const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return apply_measure_named_parameters<Tm, Face_border_length_wrapper>(np, h, tm);
}

//!
template <typename TriangleMesh>
auto longest_border(const TriangleMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return apply_measure_named_parameters<Tm, Longest_border_wrapper>(np, tm);
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
  return apply_measure_named_parameters<Tm, Squared_edge_length_wrapper>(np, e, tm);
}

//!
template <typename TriangleMesh>
auto squared_face_area(typename boost::graph_traits<TriangleMesh>::face_descriptor& f,
                       const TriangleMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return apply_measure_named_parameters<Tm, Squared_face_area_wrapper>(np, f, tm);
}

template <typename TriangleMesh>
auto volume(const TriangleMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return apply_measure_named_parameters<Tm, Volume_wrapper>(np, tm);
}

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH

template <typename TriangleMesh>
auto polyhedron_face_iterable_to_descriptors(py::iterable faces) {
  using Tm = TriangleMesh;
  using Face = typename Tm::Face;
  using Fd = typename boost::graph_traits<Tm>::face_descriptor;

  std::vector<Fd> descriptors;
  for (py::handle item : faces) {
    Face& face = py::cast<Face&>(item);
    descriptors.emplace_back(&face);
  }
  return descriptors;
}

template <typename TriangleMesh>
auto area_f_faces(py::iterable face_range, const TriangleMesh& tm,
                  const py::dict& np = py::dict()) {
  auto descriptors = polyhedron_face_iterable_to_descriptors<TriangleMesh>(face_range);
  return area_f<TriangleMesh>(descriptors, tm, np);
}

template <typename TriangleMesh>
auto edge_length_halfedge(typename TriangleMesh::Halfedge& h,
                          const TriangleMesh& tm,
                          const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Hd = typename boost::graph_traits<Tm>::halfedge_descriptor;
  auto descriptor = Hd(&h);
  return edge_length<TriangleMesh>(descriptor, tm, np);
}

template <typename TriangleMesh>
auto face_area_face(typename TriangleMesh::Face& f, const TriangleMesh& tm,
                    const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Fd = typename boost::graph_traits<Tm>::face_descriptor;
  auto descriptor = Fd(&f);
  return face_area<TriangleMesh>(descriptor, tm, np);
}

template <typename TriangleMesh>
auto face_aspect_ratio_face(typename TriangleMesh::Face& f,
                            const TriangleMesh& tm,
                            const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Fd = typename boost::graph_traits<Tm>::face_descriptor;
  auto descriptor = Fd(&f);
  return face_aspect_ratio<TriangleMesh>(descriptor, tm, np);
}

template <typename TriangleMesh>
auto face_border_length_halfedge(typename TriangleMesh::Halfedge& h,
                                 const TriangleMesh& tm,
                                 const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Hd = typename boost::graph_traits<Tm>::halfedge_descriptor;
  auto descriptor = Hd(&h);
  return face_border_length<TriangleMesh>(descriptor, tm, np);
}

template <typename TriangleMesh>
auto squared_edge_length_halfedge(typename TriangleMesh::Halfedge& h,
                                  const TriangleMesh& tm,
                                  const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Hd = typename boost::graph_traits<Tm>::halfedge_descriptor;
  auto descriptor = Hd(&h);
  return apply_measure_named_parameters<Tm, Squared_edge_length_wrapper>(np,
                                                                         descriptor,
                                                                         tm);
}

template <typename TriangleMesh>
auto squared_face_area_face(typename TriangleMesh::Face& f,
                            const TriangleMesh& tm,
                            const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Fd = typename boost::graph_traits<Tm>::face_descriptor;
  auto descriptor = Fd(&f);
  return squared_face_area<TriangleMesh>(descriptor, tm, np);
}

#endif

}
} // namespace cgalpy

//!
void export_pmp_geometric_measure(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;

  m.def("area", &cgalpy::pmp::area<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_area_1);
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("area", &cgalpy::pmp::area_f_faces<Pm>,
        py::arg("face_range"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_area);
#else
  m.def("area", &cgalpy::pmp::area_f<Pm>,
        py::arg("face_range"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_area);
#endif
  m.def("centroid", &cgalpy::pmp::centroid<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_centroid);
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("edge_length", &cgalpy::pmp::edge_length_halfedge<Pm>,
        py::arg("h"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_edge_length);
#else
  m.def("edge_length", &cgalpy::pmp::edge_length<Pm>,
        py::arg("h"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_edge_length);
#endif
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("face_area", &cgalpy::pmp::face_area_face<Pm>,
        py::arg("f"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_face_area);
#else
  m.def("face_area", &cgalpy::pmp::face_area<Pm>,
        py::arg("f"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_face_area);
#endif
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("face_aspect_ratio", &cgalpy::pmp::face_aspect_ratio_face<Pm>,
        py::arg("f"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_face_aspect_ratio);
#else
  m.def("face_aspect_ratio", &cgalpy::pmp::face_aspect_ratio<Pm>,
        py::arg("f"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_face_aspect_ratio);
#endif
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("face_border_length", &cgalpy::pmp::face_border_length_halfedge<Pm>,
        py::arg("h"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_face_border_length);
#else
  m.def("face_border_length", &cgalpy::pmp::face_border_length<Pm>,
        py::arg("h"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_face_border_length);
#endif
  m.def("longest_border", &cgalpy::pmp::longest_border<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_longest_border);
  m.def("match_faces", &cgalpy::pmp::match_faces<Pm>,
        py::arg("m1"), py::arg("m2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        pmp_doc::Polygon_mesh_processing_match_faces);
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("squared_edge_length", &cgalpy::pmp::squared_edge_length_halfedge<Pm>,
        py::arg("e"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_squared_edge_length);
#else
  m.def("squared_edge_length", &cgalpy::pmp::squared_edge_length<Pm>,
        py::arg("e"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_squared_edge_length);
#endif
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("squared_face_area", &cgalpy::pmp::squared_face_area_face<Pm>,
        py::arg("f"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_squared_face_area);
#else
  m.def("squared_face_area", &cgalpy::pmp::squared_face_area<Pm>,
        py::arg("f"), py::arg("tmesh"),
        py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_squared_face_area);
#endif
  m.def("volume", &cgalpy::pmp::volume<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_volume);
}
