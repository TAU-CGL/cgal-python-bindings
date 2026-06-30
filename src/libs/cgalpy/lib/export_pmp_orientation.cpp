// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#include <functional>
#include <vector>
#include <utility>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/function.h>

#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup_extension.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>

#include "cgalpy/Named_parameter_geom_traits.hpp"
#include "cgalpy/Named_parameter_outward_orientation.hpp"
#include "cgalpy/Named_parameter_wrapper.hpp"
#include "cgalpy/named_parameter_applicator.hpp"
#include "cgalpy/Default_orientation_visitor.hpp"
#include "cgalpy/pmp_helpers.hpp"
#include "cgalpy/ndarray_to_point_3_vector.hpp"
#include "cgalpy/polygon_mesh_processing_types.hpp"
#include "cgalpy/Pmp_docstrings.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace pmp_doc = cgalpy::pmp::docstrings;

namespace cgalpy {
namespace pmp {

//! Apply simple orientation named parameters.
template <template <typename...> class Wrapper, typename... Args>
auto apply_orientation_named_parameters(const py::dict& params, Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_outward_orientation outward_orientation_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator(wrapper, np, params,
                                            geom_traits_op,
                                            outward_orientation_op);
}

//! Apply only geom_traits for orientation predicates.
template <template <typename...> class Wrapper, typename... Args>
auto apply_orientation_geom_traits_named_parameters(const py::dict& params,
                                                    Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator(wrapper, np, params,
                                            geom_traits_op);
}

//! Wrap CGAL::Polygon_mesh_processing::orient(tm, np).
template <typename NamedParameter, typename... Args>
struct Orient_wrapper;

template <typename NamedParameter, typename TriangleMesh>
struct Orient_wrapper<NamedParameter, TriangleMesh> {
  static auto call(NamedParameter& np, TriangleMesh&& tm)
  {
    return PMP::orient(std::forward<TriangleMesh>(tm), np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::orient_to_bound_a_volume(tm, np).
template <typename NamedParameter, typename... Args>
struct Orient_to_bound_a_volume_wrapper;

template <typename NamedParameter, typename TriangleMesh>
struct Orient_to_bound_a_volume_wrapper<NamedParameter, TriangleMesh> {
  static auto call(NamedParameter& np, TriangleMesh&& tm)
  {
    return PMP::orient_to_bound_a_volume(std::forward<TriangleMesh>(tm), np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::is_outward_oriented(tm, np).
template <typename NamedParameter, typename... Args>
struct Is_outward_oriented_wrapper;

template <typename NamedParameter, typename TriangleMesh>
struct Is_outward_oriented_wrapper<NamedParameter, TriangleMesh> {
  static auto call(NamedParameter& np, TriangleMesh&& tm)
  {
    return PMP::is_outward_oriented(std::forward<TriangleMesh>(tm), np);
  }
};

//!
template <typename Point_3, typename Visitor>
auto orient_polygon_soup(std::vector<Point_3>& points, std::vector<std::vector<std::size_t>>& polygons,
                         const py::dict& np = py::dict()) {
  if (np.contains("visitor")) {
    auto visitor = py::cast<Visitor>(np["visitor"]);
    if (PMP::orient_polygon_soup
        (points, polygons, CGAL::parameters::visitor(visitor)))
      return std::make_pair(points, polygons);
  }
  else {
    if (PMP::orient_polygon_soup(points, polygons))
      return std::make_pair(points, polygons);
  }
  throw std::runtime_error("Could not orient the polygon soup");
}

//!
template <typename TriangleMesh>
auto orient(TriangleMesh& tm, const py::dict& np = py::dict()) {
  apply_orientation_named_parameters<Orient_wrapper>(np, tm);
}

//!
template <typename TriangleMesh>
py::object does_bound_a_volume(TriangleMesh& tm,
                               const py::dict& np = py::dict()) {
  const bool collect_cc_orientation =
    np.contains("is_cc_outward_oriented") &&
    py::cast<bool>(np["is_cc_outward_oriented"]);

  std::vector<bool> is_cc_outward_oriented;
  bool retv = false;

  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<TriangleMesh, std::size_t>
      (tm, "INTERNAL_MAP0", np["face_index_map"]);
    if (collect_cc_orientation) {
      retv = PMP::does_bound_a_volume
        (tm, CGAL::parameters::face_index_map(fim)
               .is_cc_outward_oriented(std::ref(is_cc_outward_oriented)));
    }
    else {
      retv = PMP::does_bound_a_volume
        (tm, CGAL::parameters::face_index_map(fim));
    }
  }
  else {
    if (collect_cc_orientation) {
      retv = PMP::does_bound_a_volume
        (tm, CGAL::parameters::is_cc_outward_oriented
             (std::ref(is_cc_outward_oriented)));
    }
    else {
      retv = PMP::does_bound_a_volume(tm);
    }
  }

  if (collect_cc_orientation) {
    py::list orientations;
    for (bool value : is_cc_outward_oriented) orientations.append(value);
    return py::make_tuple(retv, orientations);
  }
  return py::cast(retv);
}

//!
template <typename PolygonMesh>
void orient_to_bound_a_volume(PolygonMesh& tm,
                              const py::dict& np = py::dict()) {
  apply_orientation_named_parameters<Orient_to_bound_a_volume_wrapper>(np, tm);
}

//!
template <typename TriangleMesh, typename VolumeFaceIndexMap>
auto volume_connected_components(TriangleMesh& tm, VolumeFaceIndexMap volume_id_map, const py::dict& np = py::dict()) {
  std::vector<std::size_t> ccitvi, nl;
  std::vector<bool> icoo;
  std::vector<std::vector<std::size_t>> vi;
  std::vector<std::pair<std::size_t, std::size_t>> ivpot;
  std::size_t retv;
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<TriangleMesh, std::size_t>(tm, "INTERNAL_MAP0",
                                                            np.contains("face_index_map") ?
                                                            np["face_index_map"] : py::none());
    if (np.contains("face_connected_component_map")) {
      auto fccm = get_face_prop_map<TriangleMesh, std::size_t>(tm, "INTERNAL_MAP1",
                                                               np.contains("face_connected_component_map") ?
                                                               np["face_connected_component_map"] : py::none());
        retv = PMP::volume_connected_components(tm, volume_id_map);
    }
    else {
      retv = PMP::volume_connected_components(tm, volume_id_map);
    }
  }
  else {
    if (np.contains("face_connected_component_map")) {
      auto fccm = get_face_prop_map<TriangleMesh, std::size_t>(tm, "INTERNAL_MAP1",
                                                               np.contains("face_connected_component_map") ?
                                                               np["face_connected_component_map"] : py::none());
      retv = PMP::volume_connected_components(tm, volume_id_map);
    }
    else {
      retv = PMP::volume_connected_components(tm, volume_id_map);
    }
  }
  return std::make_tuple(retv, ccitvi, nl, icoo, vi, ivpot);
}

//!
template <typename TriangleMesh>
auto is_outward_oriented(TriangleMesh& tm, const py::dict& np = py::dict()) {
  return apply_orientation_geom_traits_named_parameters
    <Is_outward_oriented_wrapper>(np, tm);
}

//!
template <typename PolygonMesh>
auto reverse_face_orientations(const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                               PolygonMesh& pm) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  std::vector<Fd> out;
  PMP::reverse_face_orientations(face_range, pm);
}

//!
auto duplicate_non_manifold_edges_in_polygon_soup(std::vector<Point_3>& points,
                                                       std::vector<std::vector<std::size_t>>& polygons) {
  bool duplicated = PMP::duplicate_non_manifold_edges_in_polygon_soup(points, polygons);
  return std::make_tuple(points, polygons, duplicated);
}

//!
template <typename PolygonMesh>
auto orient_triangle_soup_with_reference_triangle_mesh(const PolygonMesh& tm_ref, std::vector<Point_3>& points,
                                                       std::vector<std::vector<std::size_t>>& triangles,
                                                       const py::dict& np1 = py::dict(),
                                                       const py::dict& np2 = py::dict()) {
  using TAG = CGAL::Sequential_tag;

  // The Python API exposes the soup as std::vector<Point_3>, so the CGAL
  // point_map named parameter in np2 is the default identity map.
  (void)np2;

  auto vpm = get_vertex_point_map(tm_ref, np1);
  auto orient_np = CGAL::parameters::vertex_point_map(vpm);

  if (np1.contains("geom_traits")) {
    auto gt = py::cast<Kernel>(np1["geom_traits"]);
    PMP::orient_triangle_soup_with_reference_triangle_mesh<TAG>
      (tm_ref, points, triangles, orient_np.geom_traits(gt));
  }
  else {
    PMP::orient_triangle_soup_with_reference_triangle_mesh<TAG>
      (tm_ref, points, triangles, orient_np);
  }

  return std::make_tuple(points, triangles);
}

//!
auto orient_triangle_soup_with_reference_triangle_soup(const std::vector<Point_3>& ref_points,
                                                       const std::vector<std::vector<std::size_t>>& ref_faces,
                                                       std::vector<Point_3>& points,
                                                       std::vector<std::vector<std::size_t>>& faces,
                                                       const py::dict& np1 = py::dict(),
                                                       const py::dict& np2 = py::dict()) {
  using TAG = CGAL::Sequential_tag;

  // The Python API exposes both soups as std::vector<Point_3>, so the default
  // identity point maps are the correct point maps for both named-parameter packs.
  (void) np2;

  if (np1.contains("geom_traits")) {
    auto gt = py::cast<Kernel>(np1["geom_traits"]);
    PMP::orient_triangle_soup_with_reference_triangle_soup<TAG>
      (ref_points, ref_faces, points, faces, CGAL::parameters::geom_traits(gt));
  }
  else {
    PMP::orient_triangle_soup_with_reference_triangle_soup<TAG>
      (ref_points, ref_faces, points, faces);
  }

  return std::make_tuple(points, faces);
}


//!
template <typename Point_3, typename Visitor>
auto orient_polygon_soup_np(const py::ndarray<>& points_array,
                            std::vector<std::vector<std::size_t>>& polygons,
                            const py::dict& np = py::dict()) {
  auto points = cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return orient_polygon_soup<Point_3, Visitor>(points, polygons, np);
}

//!
auto duplicate_non_manifold_edges_in_polygon_soup_np(const py::ndarray<>& points_array,
                                                     std::vector<std::vector<std::size_t>>& polygons) {
  auto points = cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return duplicate_non_manifold_edges_in_polygon_soup(points, polygons);
}

//!
template <typename PolygonMesh>
auto orient_triangle_soup_with_reference_triangle_mesh_np(const PolygonMesh& tm_ref,
                                                          const py::ndarray<>& points_array,
                                                          std::vector<std::vector<std::size_t>>& triangles,
                                                          const py::dict& np1 = py::dict(),
                                                          const py::dict& np2 = py::dict()) {
  auto points = cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return orient_triangle_soup_with_reference_triangle_mesh(tm_ref, points, triangles, np1, np2);
}

//!
auto orient_triangle_soup_with_reference_triangle_soup_np_np(const py::ndarray<>& ref_points_array,
                                                             const std::vector<std::vector<std::size_t>>& ref_faces,
                                                             const py::ndarray<>& points_array,
                                                             std::vector<std::vector<std::size_t>>& faces,
                                                             const py::dict& np1 = py::dict(),
                                                             const py::dict& np2 = py::dict()) {
  auto ref_points = cgalpy::ndarray_to_point_3_vector<Point_3>(ref_points_array, "ref_points");
  auto points = cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return orient_triangle_soup_with_reference_triangle_soup(ref_points, ref_faces, points, faces, np1, np2);
}

//!
auto orient_triangle_soup_with_reference_triangle_soup_ref_np(const py::ndarray<>& ref_points_array,
                                                              const std::vector<std::vector<std::size_t>>& ref_faces,
                                                              std::vector<Point_3>& points,
                                                              std::vector<std::vector<std::size_t>>& faces,
                                                              const py::dict& np1 = py::dict(),
                                                              const py::dict& np2 = py::dict()) {
  auto ref_points = cgalpy::ndarray_to_point_3_vector<Point_3>(ref_points_array, "ref_points");
  return orient_triangle_soup_with_reference_triangle_soup(ref_points, ref_faces, points, faces, np1, np2);
}

//!
auto orient_triangle_soup_with_reference_triangle_soup_points_np(const std::vector<Point_3>& ref_points,
                                                                 const std::vector<std::vector<std::size_t>>& ref_faces,
                                                                 const py::ndarray<>& points_array,
                                                                 std::vector<std::vector<std::size_t>>& faces,
                                                                 const py::dict& np1 = py::dict(),
                                                                 const py::dict& np2 = py::dict()) {
  auto points = cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return orient_triangle_soup_with_reference_triangle_soup(ref_points, ref_faces, points, faces, np1, np2);
}

//!
template <typename PolygonMesh>
void merge_reversible_connected_components(PolygonMesh& pm, const py::dict& np = py::dict()) {
  auto vpm = get_vertex_point_map(pm, np);
  auto merge_np = CGAL::parameters::vertex_point_map(vpm);

  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<PolygonMesh, std::size_t>(pm, "INTERNAL_MAP0",
                                                           np["face_index_map"]);
    PMP::merge_reversible_connected_components(pm, merge_np.face_index_map(fim));
  }
  else {
    PMP::merge_reversible_connected_components(pm, merge_np);
  }
}

//!
template <typename PolygonMesh, typename FaceBitMap>
auto compatible_orientations(PolygonMesh& pm, FaceBitMap fbm, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto fpim = get_face_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP0",
                                                 np.contains("face_patch_index_map") ?
                                                 np["face_patch_index_map"] : py::none());

  auto retv = PMP::compatible_orientations(pm, fbm);
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (!np.contains("face_patch_index_map")) {
    pm.remove_property_map(fpim);
  }
#endif
  return retv;
}

// visitor stuff
//!
void set_polygon_orientation_reversed(Default_orientation_visitor& v,
                                      const std::function<void(std::size_t)>& f) {
  v.set_polygon_orientation_reversed(f);
}

//!
void set_vertex_id_in_polygon_replaced(Default_orientation_visitor& v,
                                       const std::function<void(std::size_t, std::size_t, std::size_t)>& f) {
  v.set_vertex_id_in_polygon_replaced(f);
}

//!
void set_duplicated_vertex(Default_orientation_visitor& v,
                           const std::function<void(std::size_t, std::size_t)>& f) {
  v.set_duplicated_vertex(f);
}

//!
void set_non_manifold_edge(Default_orientation_visitor& v,
                           const std::function<void(std::size_t, std::size_t, std::size_t)>& f) {
  v.set_non_manifold_edge(f);
}

//!
void set_non_manifold_vertex(Default_orientation_visitor& v,
                             const std::function<void(std::size_t, std::size_t)>& f) {
  v.set_non_manifold_vertex(f);
}

//!
void set_link_connected_polygons(Default_orientation_visitor& v,
                                 const std::function<void(std::size_t, std::vector<std::size_t>)>& f) {
  v.set_link_connected_polygons(f);
}

}
} // namespace cgalpy // namespace pmp

//!
void export_pmp_orientation(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = boost::graph_traits<Pm>::face_descriptor;
  using faces_size_type = boost::graph_traits<Pm>::faces_size_type;
  using Dov = cgalpy::pmp::Default_orientation_visitor;

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  using Face_size_type_map = Pm::Property_map<Fd, faces_size_type>;
  using Face_bit_map = Pm::Property_map<Fd, bool>;
#endif

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  using Dfp_fst = CGAL::dynamic_face_property_t<faces_size_type>;
  using Dfp_bool = CGAL::dynamic_face_property_t<bool>;
  using Face_size_type_map = boost::property_map<Pm, Dfp_fst>;
  using Face_bit_map = boost::property_map<Pm, Dfp_bool>;
#endif

  m.def("orient_polygon_soup", &cgalpy::pmp::orient_polygon_soup<Point_3, Dov>,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict(),
        "Orients the polygons of a polygon soup.");
  m.def("orient_polygon_soup", &cgalpy::pmp::orient_polygon_soup_np<Point_3, Dov>,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict(),
        "Orients the polygons of a polygon soup from a NumPy point array.");
  m.def("orient", &cgalpy::pmp::orient<Pm>,
        py::arg("tm"), py::arg("np") = py::dict(),
        "Orients a triangle mesh.");
  m.def("does_bound_a_volume", &cgalpy::pmp::does_bound_a_volume<Pm>,
        py::arg("tm"), py::arg("np") = py::dict(),
        "Returns whether the triangle mesh bounds a volume.");
  m.def("orient_to_bound_a_volume", &cgalpy::pmp::orient_to_bound_a_volume<Pm>,
        py::arg("tm"), py::arg("np") = py::dict(),
        "Orients a polygon mesh to bound a volume.");
#if CGALPY_PMP_POLYGONAL_MESH == 1
  m.def("volume_connected_components", &cgalpy::pmp::volume_connected_components<Pm, Face_size_type_map>,
        py::arg("tm"), py::arg("volume_id_map"), py::arg("np") = py::dict(),
        "Labels connected components by volume.");
#endif
  m.def("is_outward_oriented", &cgalpy::pmp::is_outward_oriented<Pm>,
        py::arg("tm"), py::arg("np") = py::dict(),
        "Returns whether the polygon mesh is outward oriented.");
  m.def("reverse_face_orientations", &cgalpy::pmp::reverse_face_orientations<Pm>,
        py::arg("face_range"), py::arg("pmesh"),
        "Reverses the orientations of the selected faces.");
  m.def("reverse_face_orientations", &PMP::reverse_face_orientations<Pm>,
        py::arg("pmesh"),
        "Reverses the orientations of all faces of a polygon mesh.");
  m.def("duplicate_non_manifold_edges_in_polygon_soup", &cgalpy::pmp::duplicate_non_manifold_edges_in_polygon_soup,
        py::arg("points"), py::arg("polygons"),
        "Duplicates non-manifold edges in a polygon soup.");
  m.def("duplicate_non_manifold_edges_in_polygon_soup", &cgalpy::pmp::duplicate_non_manifold_edges_in_polygon_soup_np,
        py::arg("points"), py::arg("polygons"),
        "Duplicates non-manifold edges in a polygon soup from a NumPy point array.");
  m.def("orient_triangle_soup_with_reference_triangle_mesh",
        &cgalpy::pmp::orient_triangle_soup_with_reference_triangle_mesh<Pm>,
        py::arg("tm_ref"), py::arg("points"), py::arg("triangles"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        "Orients a triangle soup using a reference triangle mesh.");
  m.def("orient_triangle_soup_with_reference_triangle_mesh",
        &cgalpy::pmp::orient_triangle_soup_with_reference_triangle_mesh_np<Pm>,
        py::arg("tm_ref"), py::arg("points"), py::arg("triangles"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        "Orients a NumPy triangle soup using a reference triangle mesh.");
  m.def("orient_triangle_soup_with_reference_triangle_soup",
        &cgalpy::pmp::orient_triangle_soup_with_reference_triangle_soup,
        py::arg("ref_points"), py::arg("ref_faces"),
        py::arg("points"), py::arg("faces"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        "Orients a triangle soup using a reference triangle soup.");
  m.def("orient_triangle_soup_with_reference_triangle_soup",
        &cgalpy::pmp::orient_triangle_soup_with_reference_triangle_soup_np_np,
        py::arg("ref_points"), py::arg("ref_faces"),
        py::arg("points"), py::arg("faces"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        "Orients a NumPy triangle soup using a NumPy reference triangle soup.");
  m.def("orient_triangle_soup_with_reference_triangle_soup",
        &cgalpy::pmp::orient_triangle_soup_with_reference_triangle_soup_ref_np,
        py::arg("ref_points"), py::arg("ref_faces"),
        py::arg("points"), py::arg("faces"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        "Orients a triangle soup using a NumPy reference triangle soup.");
  m.def("orient_triangle_soup_with_reference_triangle_soup",
        &cgalpy::pmp::orient_triangle_soup_with_reference_triangle_soup_points_np,
        py::arg("ref_points"), py::arg("ref_faces"),
        py::arg("points"), py::arg("faces"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        "Orients a NumPy triangle soup using a reference triangle soup.");
  m.def("merge_reversible_connected_components", &cgalpy::pmp::merge_reversible_connected_components<Pm>,
         py::arg("pm"), py::arg("np") = py::dict(),
         "Merges connected components whose orientations can be made compatible.");
#if CGALPY_PMP_POLYGONAL_MESH == 1
  m.def("compatible_orientations", &cgalpy::pmp::compatible_orientations<Pm, Face_bit_map>,
        py::arg("pm"), py::arg("face_bit_map"), py::arg("np") = py::dict(),
        "Computes compatible orientations for connected components.");
#endif

  m.def("set_non_manifold_edge", &cgalpy::pmp::set_non_manifold_edge,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPPolygonSoupOrientationVisitor_non_manifold_edge);
  m.def("set_non_manifold_vertex", &cgalpy::pmp::set_non_manifold_vertex,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPPolygonSoupOrientationVisitor_non_manifold_vertex);
  m.def("set_duplicated_vertex", &cgalpy::pmp::set_duplicated_vertex,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPPolygonSoupOrientationVisitor_duplicated_vertex);
  m.def("set_vertex_id_in_polygon_replaced", &cgalpy::pmp::set_vertex_id_in_polygon_replaced,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPPolygonSoupOrientationVisitor_vertex_id_in_polygon_replaced);
  m.def("set_polygon_orientation_reversed", &cgalpy::pmp::set_polygon_orientation_reversed,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPPolygonSoupOrientationVisitor_polygon_orientation_reversed);
  m.def("set_link_connected_polygons", &cgalpy::pmp::set_link_connected_polygons,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPPolygonSoupOrientationVisitor_link_connected_polygons);

  py::class_<Dov>(m, "Default_orientation_visitor",
                  pmp_doc::PMPPolygonSoupOrientationVisitor_class)
    .def(py::init<>(),
         "Constructs a default orientation visitor.")
    ;
}
