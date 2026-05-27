// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <stdexcept>
#include <iterator>

#include <boost/graph/graph_traits.hpp>
#include <boost/iterator/function_output_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/tuple.h>

#include <CGAL/Kernel/Dimension_utils.h>
#include <CGAL/boost/graph/border.h>
#include <CGAL/boost/graph/Face_filtered_graph.h>
#include <CGAL/Dynamic_property_map.h>
#include <CGAL/Mesh_constant_domain_field_3.h>
#include <CGAL/boost/graph/helpers.h>
#include <CGAL/iterator.h>
#include <CGAL/Named_function_parameters.h>
#include <CGAL/tags.h>
#include <CGAL/Mesh_facet_topology.h>
#include <CGAL/Polygon_mesh_processing/bbox.h>
#include <CGAL/Polygon_mesh_processing/refine_mesh_at_isolevel.h>
#include <CGAL/Polygon_mesh_processing/region_growing.h>
#include <CGAL/Polygon_mesh_processing/transform.h>
#include <CGAL/Polygon_mesh_processing/triangle.h>
#include <CGAL/Side_of_triangle_mesh.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/Internal_face_plane_3_map.hpp"
#include "CGALPY/Named_parameter_wrapper.hpp"
#include "CGALPY/named_parameter_applicator.hpp"
#include "CGALPY/Named_parameter_geom_traits.hpp"
#include "CGALPY/merge_coplanar_facets.hpp"
#include "CGALPY/pmp_helpers.hpp"
#include "CGALPY/Polyhedral_envelope.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

using Point_3_vec = std::vector<Point_3>;
using Size_t_vec = std::vector<std::size_t>;

//!
template <typename PolygonMesh>
auto edge_bbox(const typename boost::graph_traits<PolygonMesh>::edge_descriptor ed, const PolygonMesh& pm,
               const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;

  return PMP::edge_bbox(ed, pm);
}

//!
template <typename PolygonMesh>
auto face_bbox(const typename boost::graph_traits<PolygonMesh>::face_descriptor fd, const PolygonMesh& pm,
               const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::face_bbox(fd, pm);
}

//!
template <typename PolygonMesh, typename ValueMap>
auto refine_mesh_at_isolevel(PolygonMesh& pm, ValueMap value_map,
                             typename boost::property_traits<ValueMap>::value_type isovalue,
                             const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto eicm = get_edge_prop_map<Pm, bool>(pm, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());
  return PMP::refine_mesh_at_isolevel(pm, value_map, isovalue);
}

//!
template <typename PolygonMesh>
auto transform(const Aff_transformation_3& transformation, PolygonMesh& pmesh, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::transform(transformation, pmesh);
}

//!
template <typename TriangleMesh>
auto triangle(typename boost::graph_traits<TriangleMesh>::face_descriptor fd, const TriangleMesh& tmesh,
              const py::dict& np = py::dict()) {
  return PMP::triangle(fd, tmesh);
}

//!
template <typename PolygonMesh>
auto vertex_bbox(typename boost::graph_traits<PolygonMesh>::vertex_descriptor vd, const PolygonMesh& pm,
                 const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::vertex_bbox(vd, pm);
}

//
template <typename PolygonMesh>
auto extract_boundary_cycles(PolygonMesh& pm) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Hd = typename Gt::halfedge_descriptor;

  std::vector<Hd> result;
  auto it = std::back_inserter(result);
  CGAL::extract_boundary_cycles(pm, it);
  return result;
}

//!
template <typename PolygonMesh>
auto add_bbox(PolygonMesh& pmesh, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  PMP::add_bbox(pmesh);
}

//!
template <typename PolygonMesh>
auto bbox(PolygonMesh& pmesh, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::bbox(pmesh);
}

//!
template <typename PolygonMesh, typename FaceNormalMap>
void merge_coplanar_facets(PolygonMesh& mesh, FaceNormalMap face_normals, const py::dict& params = py::dict()) {
  using Pm = PolygonMesh;
  for (const auto& item : params) {
    const std::string& key = py::cast<std::string>(item.first);
    if (key == "geom_traits") {
      auto np = CGAL::parameters::geom_traits(py::cast<const Kernel&>(item.second));
      return PMP::merge_coplanar_facets(mesh, face_normals, np);
    }
  }
  PMP::merge_coplanar_facets(mesh, face_normals);
}

//!
template <typename PolygonMesh>
auto border_halfedges(const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                      PolygonMesh& pmesh, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  using Hd = typename Gt::halfedge_descriptor;

  std::vector<Hd> out;
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP0",
                                                  np.contains("face_index_map") ?
                                                  np["face_patch_index_map"] : py::none());
    CGAL::border_halfedges(face_range, pmesh, std::back_inserter(out));
  }
  else {
    CGAL::border_halfedges(face_range, pmesh, std::back_inserter(out));
  }
  return out;
}

//!
py::list ptvec2ptlist(const Point_3_vec& ptvec) {
  py::list ptlist;
  for (auto pt : ptvec) ptlist.append(pt);
  return ptlist;
}

//!
Point_3_vec ptlist2ptvec(const py::list& ptlist) {
  Point_3_vec ptvec;
  ptvec.reserve(py::len(ptlist));
  for (auto pt : ptlist) {
    try {
      ptvec.push_back(py::cast<Point_3>(pt));
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Failed to cast to a Point_3");
    }
  }
  return ptvec;
}

//!
auto polylist2polyvec(const py::list& polylist) {
  std::vector<Size_t_vec> polyvec;
  polyvec.reserve(py::len(polylist));
  for (auto poly : polylist) {
    std::vector<std::size_t> poly_ids;
    // poyl_ids.reserve(py::len(poly));
    for (auto polyid : poly) {
      std::size_t id;
      try {
        id = py::cast<size_t>(polyid);
        poly_ids.push_back(id);
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Failed to cast to a size_t");
      }
      poly_ids.push_back(id);
    }
    polyvec.push_back(poly_ids);
  }
  return polyvec;
}

//!
auto polyvec2polylist(const std::vector<std::vector<std::size_t>> polyvec) {
  py::list polylist;
  for (auto poly : polyvec) {
    py::list p;
    for (auto pid : poly) p.append(pid);
    polylist.append(p);
  }
  return polylist;
}

using Pm = Polygonal_mesh;
using Gt = boost::graph_traits<Polygonal_mesh>;
using Vd = typename Gt::vertex_descriptor;
using Hd = typename Gt::halfedge_descriptor;
using Fd = typename Gt::face_descriptor;

//!
template <typename PolygonMesh, typename RegionMap>
auto region_growing_of_planes_on_faces(PolygonMesh& pmesh, RegionMap region_map, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  std::size_t num_regions;

  if (np.contains("region_primitive_map")) {
    boost::vector_property_map<Vector_3> rpm;
    try {
      rpm = py::cast<boost::vector_property_map<Vector_3>>(np["region_primitive_map"]);
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Failed to cast to a vector property map");
    }
    num_regions = PMP::region_growing_of_planes_on_faces(pmesh, region_map);
  }
  else {
    num_regions = PMP::region_growing_of_planes_on_faces(pmesh, region_map);
  }
  return num_regions;
}

//!
template <typename PolygonMesh, typename RegionMap, typename CornerIdMap>
auto detect_corners_of_regions(PolygonMesh& pmesh, RegionMap region_map, std::size_t nb_regions,
                               CornerIdMap corner_id_map, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  std::size_t num_corners;
  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ?
                                          np["edge_is_constrained_map"] : py::none());
  std::size_t r = PMP::detect_corners_of_regions(pmesh, region_map, nb_regions, corner_id_map);

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) {
    pmesh.remove_property_map(eicm);
  }
#endif
  return r;
}

//!
template <typename TriangleMesh>
auto degenerate_edges_r(const std::vector<typename boost::graph_traits<TriangleMesh>::edge_descriptor>& edges,
                        const TriangleMesh& tmesh, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Ed = typename Gt::edge_descriptor;
  std::vector<Ed> out;
  PMP::degenerate_edges(edges, tmesh, std::back_inserter(out));
  return out;
}

//!
template <typename TriangleMesh>
auto degenerate_edges(TriangleMesh& tmesh, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Ed = typename Gt::edge_descriptor;
  std::vector<Ed> out;
  PMP::degenerate_edges(tmesh, std::back_inserter(out));
  return out;
}

//!
template <typename TriangleMesh>
auto degenerate_faces_r(const std::vector<typename boost::graph_traits<TriangleMesh>::face_descriptor>& faces,
                        const TriangleMesh& tmesh, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Fd = typename Gt::face_descriptor;
  std::vector<Fd> out;
  PMP::degenerate_faces(faces, tmesh, std::back_inserter(out));
  return out;
}

//!
template <typename TriangleMesh>
auto degenerate_faces(TriangleMesh& tmesh, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Fd = typename Gt::face_descriptor;
  std::vector<Fd> out;
  PMP::degenerate_faces(tmesh, std::back_inserter(out));
  return out;
}

//!
template <typename TriangleMesh>
auto is_cap_triangle_face(typename boost::graph_traits<TriangleMesh>::face_descriptor f, const TriangleMesh& tm,
                          const double threshold, const py::dict& np = py::dict()) {
  auto retv = PMP::is_cap_triangle_face(f, tm, threshold);
  // return retv != boost::graph_traits<TriangleMesh>::null_halfedge() ? py::cast(retv) : py::none();
  if (retv == boost::graph_traits<TriangleMesh>::null_halfedge()) {
    throw std::runtime_error("is_cap_triangle_face failed");
  }
  return retv;
}

//!
template <typename TriangleMesh>
auto is_degenerate_edge(typename boost::graph_traits<TriangleMesh>::edge_descriptor e,
                        const TriangleMesh& tm, const py::dict& np = py::dict()) {
  return PMP::is_degenerate_edge(e, tm);
}

//!
template <typename TriangleMesh>
auto is_degenerate_triangle_face(typename boost::graph_traits<TriangleMesh>::face_descriptor f,
                                 const TriangleMesh& tm, const py::dict& np = py::dict()) {
  return PMP::is_degenerate_triangle_face(f, tm);
}

//!
template <typename TriangleMesh>
auto is_needle_triangle_face(typename boost::graph_traits<TriangleMesh>::face_descriptor f,
                             const TriangleMesh& tm, const double threshold, const py::dict& np = py::dict()) {
  return PMP::is_needle_triangle_face(f, tm, threshold);
}

// using Boolean_operation_type = COREFINEMENT::Boolean_operation_type;

} // namespace pmp

// Export Polygon_mesh_processing
void export_polygon_mesh_processing(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;
  // using Vid = pmp::Vertex_identifier;
  // using Hid = pmp::Halfedge_identifier;
  // using Fid = pmp::Face_identifier;
  using Polyline = std::vector<Kernel::Point_3>;
  using Np = CGAL::parameters::Default_named_parameters;
  namespace PMP = CGAL::Polygon_mesh_processing;
  using Fd = boost::graph_traits<Pm>::face_descriptor;
  using Hd = boost::graph_traits<Pm>::halfedge_descriptor;
  using Vd = boost::graph_traits<Pm>::vertex_descriptor;
  using Ed = boost::graph_traits<Pm>::edge_descriptor;
  using faces_size_type = boost::graph_traits<Pm>::faces_size_type;

  using Np_t = bool;
  using Np_tag = CGAL::internal_np::all_default_t;
  using Np_base = CGAL::internal_np::No_property;
  using Np_class = CGAL::Named_function_parameters<Np_t, Np_tag, Np_base>;
  using Concurrency_tag = CGAL::Sequential_tag;

  // FacePatchMap a class model of ReadablePropertyMap with
  // boost::graph_traits<TriangleMeshIn>::face_descriptor as key type and
  // std::size_t as value type EdgeIsConstrainedMap a class model of
  // ReadablePropertyMap with
  // boost::graph_traits<TriangleMeshIn>::edge_descriptor as key type and bool
  // as value type VertexCornerMap a class model of ReadablePropertyMap with
  // boost::graph_traits<TriangleMeshIn>::vertex_descriptor as key type and
  // std::size_t as value type

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  using Vertex_size_map = Pm::Property_map<Vd, std::size_t>;
  using Vertex_double_map = Pm::Property_map<Vd, double>;
  using Face_bool_map = Pm::Property_map<Fd, bool>;
  using Face_size_type_map = Pm::Property_map<Fd, faces_size_type>;
  using Face_size_map = Pm::Property_map<Fd, std::size_t>;
  using Face_plane_map = Pm::Property_map<Fd, Plane_3>;
  using Face_vector_map = Pm::Property_map<Fd, Vector_3>;
#endif

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  using Vertex_size_tag = CGAL::dynamic_vertex_property_t<std::size_t>;
  using Vertex_size_map = boost::property_map<Pm, Vertex_size_tag>::type;

  using Vector_double_tag = CGAL::dynamic_vertex_property_t<double>;
  using Vertex_double_map = boost::property_map<Pm, Vector_double_tag>::type;

  using Face_size_tag = CGAL::dynamic_face_property_t<std::size_t>;
  using Face_size_map = boost::property_map<Pm, Face_size_tag>::type;

  using Face_size_type_tag = CGAL::dynamic_face_property_t<faces_size_type>;
  using Face_size_type_map = boost::property_map<Pm, Face_size_type_tag>::type;

  using Face_bool_tag = CGAL::dynamic_face_property_t<bool>;
  using Face_bool_map = boost::property_map<Pm, Face_bool_tag>::type;

  using Face_plane_tag = CGAL::dynamic_face_property_t<Plane_3>;
  using Face_plane_map = boost::property_map<Pm, Face_plane_tag>::type;

  using Face_vector_tag = CGAL::dynamic_face_property_t<Vector_3>;
  using Face_vector_map = boost::property_map<Pm, Face_vector_tag>::type;
#endif

  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Np_class>(m, "Named_function_parameters")
    .def(py::init<>());

  // Predicates
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYGONAL_MESH
  using Sotm = CGAL::Side_of_triangle_mesh<Pm, Kernel>;
  using Sotm_op = CGAL::Bounded_side(Sotm::*)(const Point_3&)const;
  if (! add_attr<Sotm>(m, "Side_of_triangle_mesh")) {
    py::class_<Sotm>(m, "Side_of_triangle_mesh")
      .def(py::init<const Pm&>())
      .def(py::init<const Pm&, const Kernel&>())
      .def("__call__", static_cast<Sotm_op>(&Sotm::operator()))
      ;
  }
#endif

  m.def("degenerate_edges", &pmp::degenerate_edges_r<Pm>, py::arg("edges"), py::arg("tm"), py::arg("np") = py::dict());
  m.def("degenerate_edges", &pmp::degenerate_edges<Pm>, py::arg("tm"), py::arg("np") = py::dict());
  m.def("degenerate_faces", &pmp::degenerate_faces_r<Pm>, py::arg("faces"), py::arg("tm"), py::arg("np") = py::dict());
  m.def("degenerate_faces", &pmp::degenerate_faces<Pm>, py::arg("tm"), py::arg("np") = py::dict());
  m.def("is_cap_triangle_face", &pmp::is_cap_triangle_face<Pm>,
        py::arg("f"), py::arg("tm"), py::arg("threshold"), py::arg("np") = py::dict());
  m.def("is_degenerate_edge", &pmp::is_degenerate_edge<Pm>, py::arg("e"), py::arg("pm"), py::arg("np") = py::dict());
  m.def("is_degenerate_triangle_face", &pmp::is_degenerate_triangle_face<Pm>,
        py::arg("f"), py::arg("tm"), py::arg("np") = py::dict());
  m.def("is_needle_triangle_face", &pmp::is_needle_triangle_face<Pm>,
        py::arg("f"), py::arg("tm"), py::arg("threshold"), py::arg("np") = py::dict());

  // other
  m.def("add_bbox", &pmp::add_bbox<Pm>, py::arg("pmesh"), py::arg("np") = py::dict());
  m.def("bbox", &pmp::bbox<Pm>, py::arg("pmesh"), py::arg("np") = py::dict());
  m.def("border_halfedges", &pmp::border_halfedges<Pm>,
        py::arg("face_range"), py::arg("pm"), py::arg("np") = py::dict());

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  m.def("detect_corners_of_regions",
        &pmp::detect_corners_of_regions<Pm, Face_size_map, Vertex_size_map>,
        py::arg("pmesh"), py::arg("region_map"), py::arg("nb_regions"), py::arg("corner_id_map"),
        py::arg("np") = py::dict());
#endif

  m.def("edge_bbox", &pmp::edge_bbox<Pm>,
        py::arg("ed"), py::arg("pmesh"), py::arg("np") = py::dict());
  m.def("extract_boundary_cycles", &pmp::extract_boundary_cycles<Pm>,
        py::arg("pmesh"));
  m.def("face_bbox", &pmp::face_bbox<Pm>,
        py::arg("f"), py::arg("pmesh"), py::arg("np") = py::dict());

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  m.def("refine_mesh_at_isolevel",
        &pmp::refine_mesh_at_isolevel<Pm, Vertex_double_map>,
        py::arg("pm"), py::arg("value_map"), py::arg("isovalue"), py::arg("np") = py::dict());
  m.def("region_growing_of_planes_on_faces",
        &pmp::region_growing_of_planes_on_faces<Pm, Face_size_map>,
        py::arg("pmesh"), py::arg("region_map"), py::arg("np") = py::dict());
#endif

  // region growing only for sm

  m.def("transform", &pmp::transform<Pm>, py::arg("transformation"), py::arg("pmesh"), py::arg("np") = py::dict());
  m.def("triangle", &pmp::triangle<Pm>, py::arg("f"), py::arg("tmesh"), py::arg("np") = py::dict());
  m.def("vertex_bbox", &pmp::vertex_bbox<Pm>, py::arg("vd"), py::arg("pmesh"), py::arg("np") = py::dict());

  // Custom sizing field
  // m.def("set_at", &pmp::set_at);
  // m.def("set_is_too_long", &pmp::set_is_too_long);
  // m.def("set_is_too_short", &pmp::set_is_too_short);
  // m.def("set_split_placement", &pmp::set_split_placement);
  // m.def("set_register_split_vertex", &pmp::set_register_split_vertex);

  using Pe = pmp::Polyhedral_envelope<Pm, Kernel>;
  py::class_<Pe>(m, "Polyhedral_envelope")
    .def(py::init<Pm&, double, const py::dict&>(),
         py::arg("tmesh"), py::arg("epsilon"), py::arg("np") = py::dict())
    .def(py::init<const std::vector<Fd>&, Pm&, double, const py::dict&>(),
         py::arg("face_range"), py::arg("tmesh"), py::arg("epsilon"), py::arg("np") = py::dict())
    .def(py::init<const pmp::Point_3_vec&, const std::vector<pmp::Size_t_vec>&, double, const py::dict&>(),
         py::arg("points"), py::arg("polygons"), py::arg("epsilon"),
         py::arg("np") = py::dict()) // TODO: handle face_epsilon_map
    .def("is_empty", &Pe::is_empty)
    .def("inside", [](const Pe& i, const Point_3& query) { return i(query); }, py::arg("query"))
    .def("inside", [](const Pe& i, const Point_3& source, const Point_3& target) { return i(source, target); },
         py::arg("source"), py::arg("target"))
    .def("inside", [](const Pe& i, const Point_3& t0, const Point_3& t1, const Point_3& t2) { return i(t0, t1, t2); },
         py::arg("t0"), py::arg("t1"), py::arg("t2"))
    .def("inside",
         [](const Pe& i, const Pm& tmesh, const py::dict& np) { return i(tmesh); },
         py::arg("tmesh"), py::arg("np") = py::dict())
    // TODO: inside triangle range
    ;

  // using Csf = pmp::Custom_sizing_field<Pm>;
  // py::class_<Csf>(m, "Custom_sizing_field")
  //   .def(py::init<>())
  //   ;

#if ((CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH) && \
     (CGALPY_POL3_GEOMETRY_TRAITS == CGALPY_POL3_WITH_NORMALS_GEOMETRY_TRAITS))
  using Face_normal_map = pol3::Internal_face_plane_3_map<Pm>;
#else
  using Face_normal_map = Face_vector_map;
#endif

  m.def("merge_coplanar_facets", &pmp::merge_coplanar_facets<Pm, Face_normal_map>,
        py::arg("pmesh"), py::arg("face_normals"), py::arg("np") = py::dict());
}
