// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#include <vector>
#include <tuple>
#include <functional>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/function.h>

#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/angle_and_area_smoothing.h>
#include <CGAL/Polygon_mesh_processing/extrude.h>
#include <CGAL/Polygon_mesh_processing/fair.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/random_perturbation.h>
#include <CGAL/Polygon_mesh_processing/refine.h>
#include <CGAL/Polygon_mesh_processing/remesh_planar_patches.h>
#include <CGAL/Polygon_mesh_processing/smooth_shape.h>
#include <CGAL/Polygon_mesh_processing/surface_Delaunay_remeshing.h>
#include <CGAL/Polygon_mesh_processing/tangential_relaxation.h>

#include "cgalpy/Named_parameter_density_control_factor.hpp"
#include "cgalpy/Named_parameter_do_project.hpp"
#include "cgalpy/Named_parameter_edge_is_constrained_map.hpp"
#include "cgalpy/Named_parameter_geom_traits.hpp"
#include "cgalpy/Named_parameter_random_seed.hpp"
#include "cgalpy/Named_parameter_vertex_is_constrained_map.hpp"
#include "cgalpy/Named_parameter_vertex_point_map.hpp"
#include "cgalpy/Named_parameter_wrapper.hpp"
#include "cgalpy/named_parameter_applicator.hpp"
#include "cgalpy/Adaptive_sizing_field.hpp"
#include "cgalpy/Custom_sizing_field.hpp"
#include "cgalpy/pmp_helpers.hpp"
#include "cgalpy/ndarray_to_point_3_vector.hpp"
#include "cgalpy/polygon_mesh_processing_types.hpp"
#include "cgalpy/Uniform_sizing_field.hpp"
#include "cgalpy/Triangulate_faces_visitor.hpp"
#include "cgalpy/Pmp_docstrings.hpp"

#if CGAL_VERSION_NR <= 1060100900
#include "cgalpy/pmp_np_parser.hpp"
#endif

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;
namespace pmp_doc = cgalpy::pmp::docstrings;

namespace cgalpy {
namespace pmp {

//! Apply only geom_traits to meshing wrappers.
template <template <typename...> class Wrapper, typename... Args>
auto apply_meshing_geom_traits_named_parameters(const py::dict& params,
                                                Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator(wrapper, np, params,
                                            geom_traits_op);
}

//! Apply triangulate_faces named parameters.
template <typename PolygonMesh>
struct Named_parameter_triangulate_faces_visitor {
  const std::string m_name = "visitor";

  template <typename NamedParameter>
  auto operator()(NamedParameter& np, const py::handle& value) const
  {
    using Visitor = Triangulate_faces_visitor<PolygonMesh>;
    return np.visitor(py::cast<Visitor>(value));
  }
};

template <typename PolygonMesh, template <typename...> class Wrapper,
          typename... Args>
auto apply_triangulate_faces_named_parameters(const py::dict& params,
                                              Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  Named_parameter_triangulate_faces_visitor<PolygonMesh> visitor_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator(wrapper, np, params,
                                            geom_traits_op,
                                            visitor_op);
}

//! Apply refine named parameters.
template <template <typename...> class Wrapper, typename... Args>
auto apply_refine_named_parameters(const py::dict& params, Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_density_control_factor density_control_factor_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator(wrapper, np, params,
                                            density_control_factor_op);
}

//! Apply random perturbation named parameters.
template <typename PolygonMesh, template <typename...> class Wrapper,
          typename... Args>
auto apply_random_perturbation_named_parameters(const py::dict& params,
                                                Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_vertex_point_map<PolygonMesh> vertex_point_map_op;
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_vertex_is_constrained_map<PolygonMesh>
    vertex_is_constrained_map_op;
  cgalpy::Named_parameter_do_project do_project_op;
  cgalpy::Named_parameter_random_seed random_seed_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator(wrapper, np, params,
                                            vertex_point_map_op,
                                            geom_traits_op,
                                            vertex_is_constrained_map_op,
                                            do_project_op,
                                            random_seed_op);
}

//! Apply tangential relaxation number-of-iterations parameter.
struct Named_parameter_number_of_iterations {
  const std::string m_name = "number_of_iterations";

  template <typename NamedParameter>
  auto operator()(NamedParameter& np, const py::handle& value) const
  { return np.number_of_iterations(py::cast<unsigned int>(value)); }
};

//! Apply tangential relaxation constraint-relaxation flag.
struct Named_parameter_relax_constraints {
  const std::string m_name = "relax_constraints";

  template <typename NamedParameter>
  auto operator()(NamedParameter& np, const py::handle& value) const
  { return np.relax_constraints(py::cast<bool>(value)); }
};

//! Apply tangential relaxation move filter.
template <typename PolygonMesh>
struct Named_parameter_allow_move_functor {
  const std::string m_name = "allow_move_functor";

  template <typename NamedParameter>
  auto operator()(NamedParameter& np, const py::handle& value) const {
    using Vd = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;
    using Point = Kernel::Point_3;
    using Move_functor = std::function<bool(Vd, Point, Point)>;
    return np.allow_move_functor(py::cast<Move_functor>(value));
  }
};

//! Wrap CGAL::Polygon_mesh_processing::tangential_relaxation(tm, np).
template <typename NamedParameter, typename TriangleMesh>
struct Tangential_relaxation_wrapper {
  static auto call(NamedParameter& np, TriangleMesh&& tmesh)
  {
    return PMP::tangential_relaxation(vertices(tmesh), tmesh, np);
  }
};

//! Apply tangential relaxation named parameters.
template <typename PolygonMesh, template <typename...> class Wrapper,
          typename... Args>
auto apply_tangential_relaxation_named_parameters(const py::dict& params,
                                                  Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_vertex_point_map<PolygonMesh> vertex_point_map_op;
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_edge_is_constrained_map<PolygonMesh>
    edge_is_constrained_map_op;
  cgalpy::Named_parameter_vertex_is_constrained_map<PolygonMesh>
    vertex_is_constrained_map_op;
  Named_parameter_number_of_iterations number_of_iterations_op;
  Named_parameter_relax_constraints relax_constraints_op;
  Named_parameter_allow_move_functor<PolygonMesh> allow_move_functor_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator(wrapper, np, params,
                                            vertex_point_map_op,
                                            geom_traits_op,
                                            edge_is_constrained_map_op,
                                            vertex_is_constrained_map_op,
                                            number_of_iterations_op,
                                            relax_constraints_op,
                                            allow_move_functor_op);
}

//! Wrap CGAL::Polygon_mesh_processing::refine(..., np).
template <typename NamedParameter, typename... Args>
struct Refine_wrapper;

template <typename NamedParameter, typename TriangleMesh, typename FaceRange,
          typename FaceOutputIterator, typename VertexOutputIterator>
struct Refine_wrapper<NamedParameter, TriangleMesh, FaceRange,
                      FaceOutputIterator, VertexOutputIterator> {
  static auto call(NamedParameter& np, TriangleMesh&& tmesh,
                   FaceRange&& face_range,
                   FaceOutputIterator&& faces_out,
                   VertexOutputIterator&& vertices_out)
  {
    return PMP::refine(std::forward<TriangleMesh>(tmesh),
                       std::forward<FaceRange>(face_range),
                       std::forward<FaceOutputIterator>(faces_out),
                       std::forward<VertexOutputIterator>(vertices_out),
                       np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::triangulate_faces(pm, np).
template <typename NamedParameter, typename... Args>
struct Triangulate_faces_wrapper;

template <typename NamedParameter, typename PolygonMesh>
struct Triangulate_faces_wrapper<NamedParameter, PolygonMesh> {
  static auto call(NamedParameter& np, PolygonMesh&& pm)
  {
    return PMP::triangulate_faces(faces(pm), pm, np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::triangulate_faces(face_range, pm, np).
template <typename NamedParameter, typename... Args>
struct Triangulate_faces_range_wrapper;

template <typename NamedParameter, typename FaceRange, typename PolygonMesh>
struct Triangulate_faces_range_wrapper<NamedParameter, FaceRange, PolygonMesh> {
  static auto call(NamedParameter& np, FaceRange&& face_range,
                   PolygonMesh&& pm)
  {
    return PMP::triangulate_faces(std::forward<FaceRange>(face_range),
                                  std::forward<PolygonMesh>(pm),
                                  np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::random_perturbation(tmesh, size, np).
template <typename NamedParameter, typename... Args>
struct Random_perturbation_wrapper;

template <typename NamedParameter, typename TriangleMesh, typename Size>
struct Random_perturbation_wrapper<NamedParameter, TriangleMesh, Size> {
  static auto call(NamedParameter& np, TriangleMesh&& tmesh, Size&& size)
  {
    return PMP::random_perturbation(std::forward<TriangleMesh>(tmesh),
                                    std::forward<Size>(size),
                                    np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::random_perturbation(vertices, tmesh, size, np).
template <typename NamedParameter, typename... Args>
struct Random_perturbation_vertices_wrapper;

template <typename NamedParameter, typename VertexRange,
          typename TriangleMesh, typename Size>
struct Random_perturbation_vertices_wrapper<NamedParameter, VertexRange,
                                            TriangleMesh, Size> {
  static auto call(NamedParameter& np, VertexRange&& vertices,
                   TriangleMesh&& tmesh, Size&& size)
  {
    return PMP::random_perturbation(std::forward<VertexRange>(vertices),
                                    std::forward<TriangleMesh>(tmesh),
                                    std::forward<Size>(size),
                                    np);
  }
};

//!
template <typename PolygonMesh>
auto remesh_planar_patches(PolygonMesh& pmesh, const py::dict& np_in = py::dict(),
                           const py::dict& np_out = py::dict()) {
  using Pm = PolygonMesh;

  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "edge_is_constrained_map",
                                          np_in.contains("edge_is_constrained_map") ?
                                          np_in["edge_is_constrained_map"] : py::none());
  auto fpm = get_face_prop_map<Pm, int>(pmesh, "face_patch_map",
    np_in.contains("face_patch_map") ? np_in["face_patch_map"] : py::none());
  auto vcm = get_vertex_prop_map<Pm, std::size_t>(pmesh, "vertex_corner_map",
                                                  np_in.contains("vertex_corner_map") ?
                                                  np_in["vertex_corner_map"] : py::none());

  auto fpm2 = get_face_prop_map<Pm, int>(pmesh, "face_patch_map",
    np_out.contains("face_patch_map") ? np_out["face_patch_map"] : py::none());
  auto vcm2 = get_vertex_prop_map<Pm, std::size_t>(pmesh, "vertex_corner_map",
                                                   np_out.contains("vertex_corner_map") ?
                                                   np_out["vertex_corner_map"] : py::none());

  Pm out;

  PMP::remesh_planar_patches(pmesh, out);

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np_in.contains("edge_is_constrained_map"))
    pmesh.remove_property_map(eicm);
  if (! np_in.contains("face_patch_map")) pmesh.remove_property_map(fpm);
  if (! np_in.contains("vertex_corner_map")) pmesh.remove_property_map(vcm);
#endif

  return out;
}

//!
template <typename PolygonMesh, typename FacePatchMap, typename VertexCornerMap, typename EdgeIsConstrainedMap>
auto remesh_almost_planar_patches(PolygonMesh tm_in, std::size_t nb_patches, std::size_t nb_corners,
                                  FacePatchMap face_patch_map, VertexCornerMap vertex_corner_map,
                                  EdgeIsConstrainedMap ecm,
                                  const py::dict& np_in = py::dict(), const py::dict& np_out = py::dict()) {
  using Pm = PolygonMesh;

  Pm pm_out;

  auto fpm = get_face_prop_map<Pm, int>(pm_out, "INTERNAL_MAP1",
                                        np_out.contains("face_patch_map") ? np_out["face_patch_map"] : py::none());
  auto vcm = get_vertex_prop_map<Pm, int>(pm_out, "INTERNAL_MAP2",
                                          np_out.contains("vertex_corner_map") ?
                                          np_out["vertex_corner_map"] : py::none());

  bool retv;
  if (np_in.contains("patch_normal_map")) {
    // auto rpm = py::cast<boost::vector_property_map<Vector_3>>(np_in["patch_normal_map"]);
    boost::vector_property_map<Vector_3> rpm;
    try {
      using vpm = boost::vector_property_map<Vector_3>;
      rpm = py::cast<vpm>(np_in["patch_normal_map"]);
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Failed to cast to a vector property map");
    }
    retv = PMP::remesh_almost_planar_patches(tm_in, pm_out, nb_patches, nb_corners, face_patch_map,
                                             vertex_corner_map, ecm);
  }
  else {
    retv = PMP::remesh_almost_planar_patches(tm_in, pm_out, nb_patches, nb_corners, face_patch_map,
                                             vertex_corner_map, ecm);
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np_out.contains("face_patch_map")) pm_out.remove_property_map(fpm);
  if (! np_out.contains("vertex_corner_map")) pm_out.remove_property_map(vcm);
#endif

  if (! retv)
    throw std::runtime_error("Remeshing almost planar patches failed!");
  return pm_out;
}

//!
template <typename PolygonMesh>
auto refine(PolygonMesh& tmesh, const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& faces,
            const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  using Vd = typename Gt::vertex_descriptor;
  std::vector<Fd> faces_out;
  std::vector<Vd> vertices_out;
  apply_refine_named_parameters<Refine_wrapper>
    (np, tmesh, faces, std::back_inserter(faces_out),
     std::back_inserter(vertices_out));
  return std::make_tuple(faces_out, vertices_out);
}

// only for polyhedron!
template <typename TriangleMesh>
auto fair(TriangleMesh& tmesh, const std::vector<Point_3>& vertices, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Vh = typename TriangleMesh::Vertex_handle;
  return PMP::fair(tmesh, vertices);
}

//!
template <typename PolygonMesh>
auto triangulate_faces(PolygonMesh& pm, const py::dict& np) {
  using Pm = PolygonMesh;
  // auto vpm = get_vertex_point_map(pm, np);
  return apply_triangulate_faces_named_parameters
    <Pm, Triangulate_faces_wrapper>(np, pm);
}

//!
template <typename PolygonMesh>
auto triangulate_faces_r(const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                         PolygonMesh& pm, const py::dict& np) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  return apply_triangulate_faces_named_parameters
    <Pm, Triangulate_faces_range_wrapper>(np, face_range, pm);
}

//!
auto triangulate_polygons(std::vector<Point_3>& points, std::vector<std::vector<std::size_t>>& polygons,
                          const py::dict& np = py::dict()) {
  PMP::triangulate_polygons(points, polygons);
  return std::make_tuple(points, polygons);
}


//!
auto triangulate_polygons_np(const py::ndarray<>& points_array,
                             std::vector<std::vector<std::size_t>>& polygons,
                             const py::dict& np = py::dict()) {
  auto points = cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return triangulate_polygons(points, polygons, np);
}

//!
template <typename PolygonMesh, typename SizingFunction>
auto isotropic_remeshing_sf(const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                            SizingFunction& sizing, PolygonMesh& pmesh, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  PMP::isotropic_remeshing(face_range, sizing, pmesh);
}

//!
template <typename PolygonMesh>
auto surface_Delaunay_remeshing(PolygonMesh& tmesh, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto eicm = get_edge_prop_map<Pm, bool>(tmesh, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ?
                                          np["edge_is_constrained_map"] : py::none());
  auto fpm = get_face_prop_map<Pm, int>(tmesh, "INTERNAL_MAP1",
                                        np.contains("face_patch_map") ? np["face_patch_map"] : py::none());
  // auto vpm = get_vertex_point_map(tmesh, np);

  FT mfs, mfa, mfd;
  try {
    mfs = np.contains("mesh_facet_size") ? py::cast<FT>(np["mesh_facet_size"]) : 0;
    mfa = np.contains("mesh_facet_angle") ? py::cast<FT>(np["mesh_facet_angle"]) : 0;
    mfd = np.contains("mesh_facet_distance") ? py::cast<FT>(np["mesh_facet_distance"]) : 0;
  }
  catch (const py::cast_error& e) {
    throw std::runtime_error("Failed to cast mesh facet size, angle, or distance");
  }
  Pm retv;

  if (np.contains("mesh_edge_size")) {
    using Mcdf = CGAL::Mesh_constant_domain_field_3<Kernel, int>;
    auto mes = py::cast<Mcdf>(np["mesh_edge_size"]);
    retv = PMP::surface_Delaunay_remeshing(tmesh);
  }
  else {
    retv = PMP::surface_Delaunay_remeshing(tmesh);
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) tmesh.remove_property_map(eicm);
  if (! np.contains("face_patch_map")) tmesh.remove_property_map(fpm);
#endif

  return retv;
}

//!
template <typename PolygonMesh>
auto split_long_edges(const std::vector<typename boost::graph_traits<PolygonMesh>::edge_descriptor>& edge_range,
                      double max_length, PolygonMesh& pmesh, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Ed = typename Gt::edge_descriptor;
  // auto vpm = get_vertex_point_map(pmesh, np);
  auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP0",
                                                np.contains("face_index_map") ?
                                                np["face_patch_index_map"] : py::none());
  auto fpm = get_face_prop_map<Pm, int>(pmesh, "INTERNAL_MAP1",
                                        np.contains("face_patch_map") ? np["face_patch_map"] : py::none());
  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP2",
                                          np.contains("edge_index_copy_map") ?
                                          np["edge_index_copy_map"] : py::none());
  // turn edge_range to a vector
  std::vector<Ed> edge_vec;
  PMP::split_long_edges(edge_vec, max_length, pmesh);
  return edge_vec;
}

//!
template <typename InputMesh, typename OutputMesh>
auto extrude_mesh_v(const InputMesh& imesh, OutputMesh& omesh, Vector_3 v, const py::dict& np_in = py::dict(),
                    const py::dict& np_out = py::dict()) {
  PMP::extrude_mesh(imesh, omesh, v);
}

//!
template <typename PolygonMesh>
auto angle_and_area_smoothing(const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& faces,
                              PolygonMesh& pmesh, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ?
                                          np["edge_is_constrained_map"] : py::none());
  auto vicm = get_vertex_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP1",
                                            np.contains("vertex_is_constrained_map") ?
                                            np["vertex_is_constrained_map"] : py::none());
  PMP::angle_and_area_smoothing(faces, pmesh);

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) {
    pmesh.remove_property_map(eicm);
  }
  if (! np.contains("vertex_is_constrained_map")) {
    pmesh.remove_property_map(vicm);
  }
#endif
}

//!
template <typename PolygonMesh>
auto angle_and_area_smoothing_m(PolygonMesh& pmesh, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ?
                                          np["edge_is_constrained_map"] : py::none());
  auto vicm = get_vertex_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP1",
                                            np.contains("vertex_is_constrained_map") ?
                                            np["vertex_is_constrained_map"] : py::none());
  PMP::angle_and_area_smoothing(pmesh);

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) pmesh.remove_property_map(eicm);
  if (! np.contains("vertex_is_constrained_map")) pmesh.remove_property_map(vicm);
#endif
}

//! Wrap CGAL::Polygon_mesh_processing::tangential_relaxation(tm, np).
template <typename PolygonMesh>
auto tangential_relaxation(PolygonMesh& pmesh, const py::dict& np = py::dict()) {
#if CGAL_VERSION_NR > 1060100900
  return apply_tangential_relaxation_named_parameters
    <PolygonMesh, Tangential_relaxation_wrapper>(np, pmesh);
#else
  auto cgal_parameters = internal::parse_pmp_np<PolygonMesh>(np);
  return PMP::tangential_relaxation(vertices(pmesh), pmesh, cgal_parameters);
#endif
}

//
template <typename PolygonMesh>
auto smooth_shape(PolygonMesh& pmesh, const double time, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto vpm = get_vertex_point_map(pmesh, np);
  auto vicm = get_vertex_prop_map<Pm, bool>
    (pmesh, "INTERNAL_MAP0",
     np.contains("vertex_is_constrained_map") ?
     np["vertex_is_constrained_map"] : py::none());
  auto smooth_np = CGAL::parameters::vertex_point_map(vpm).
    vertex_is_constrained_map(vicm);

  if (np.contains("geom_traits")) {
    auto gt = py::cast<Kernel>(np["geom_traits"]);
    PMP::smooth_shape(pmesh, time, smooth_np.geom_traits(gt));
  }
  else {
    PMP::smooth_shape(pmesh, time, smooth_np);
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("vertex_is_constrained_map")) pmesh.remove_property_map(vicm);
#endif
}

//!
template <typename TriangleMesh>
auto random_perturbation(TriangleMesh& tmesh, const double& perturbation_max_size, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  apply_random_perturbation_named_parameters
    <Tm, Random_perturbation_wrapper>(np, tmesh, perturbation_max_size);
}

//!
template <typename PolygonMesh>
auto random_perturbation_v(const std::vector<typename boost::graph_traits<PolygonMesh>::vertex_descriptor>& vertices,
                           PolygonMesh& pmesh, const double& perturbation_max_size, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  apply_random_perturbation_named_parameters
    <Pm, Random_perturbation_vertices_wrapper>(np, vertices, pmesh,
                                               perturbation_max_size);
}

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
//!
template <typename PolygonMesh>
auto random_perturbation_v_pol3(const py::sequence& vertices,
                                PolygonMesh& pmesh,
                                const double& perturbation_max_size,
                                const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Vd = typename boost::graph_traits<Pm>::vertex_descriptor;

  std::vector<Vd> vertex_descriptors;
  vertex_descriptors.reserve(py::len(vertices));

  for (size_t i = 0; i < py::len(vertices); ++i) {
    auto& vertex = py::cast<typename Pm::Vertex&>(vertices[i]);
    vertex_descriptors.emplace_back(Vd(&vertex));
  }

  apply_random_perturbation_named_parameters
    <Pm, Random_perturbation_vertices_wrapper>(np, vertex_descriptors, pmesh,
                                               perturbation_max_size);
}
#endif

}
} // namespace cgalpy // namespace pmp

//!
void export_pmp_meshing(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  using Vd = typename Gt::vertex_descriptor;
  using Ed = typename Gt::edge_descriptor;

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  using Fpm = Pm::Property_map<Fd, std::size_t>;
  using Vcm = Pm::Property_map<Vd, std::size_t>;
  using Eicm = Pm::Property_map<Ed, bool>;
#endif

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  using Dfpt = CGAL::dynamic_face_property_t<std::size_t>;
  using Fpm = boost::property_map<Pm, Dfpt>::type;
  using Dvpt = CGAL::dynamic_vertex_property_t<std::size_t>;
  using Vcm = boost::property_map<Pm, Dvpt>::type;
  using Dept = CGAL::dynamic_edge_property_t<bool>;
  using Eicm = boost::property_map<Pm, Dept>::type;
#endif

  m.def("remesh_planar_patches", &cgalpy::pmp::remesh_planar_patches<Pm>,
        py::arg("pm"), py::arg("np_in") = py::dict(),
        py::arg("np_out") = py::dict(),
        "Remeshes planar patches of a polygon mesh.");

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  m.def("remesh_almost_planar_patches",
        &cgalpy::pmp::remesh_almost_planar_patches<Pm, Fpm, Vcm, Eicm>,
        py::arg("tm_in"), py::arg("nb_patches"), py::arg("nb_corners"),
        py::arg("face_patch_map"), py::arg("vertex_corner_map"), py::arg("ecm"),
        py::arg("np_in") = py::dict(), py::arg("np_out") = py::dict(),
        "Remeshes almost planar patches of a triangle mesh.");
#endif

  m.def("refine", &cgalpy::pmp::refine<Pm>,
        py::arg("tmesh"), py::arg("faces"), py::arg("np") = py::dict(),
        "Refines selected faces of a triangle mesh.");

#if 0 // broken for now because of CGAL
  m.def("fair", &cgalpy::pmp::fair<Pm>,
        py::arg("tmesh"), py::arg("vertices"), py::arg("np") = py::dict(),
        "Fairs selected vertices of a triangle mesh.");
#endif

  m.def("triangulate_faces", &cgalpy::pmp::triangulate_faces<Pm>,
        py::arg("pm"), py::arg("np") = py::dict(),
        "Triangulates all faces of a polygon mesh.");
  m.def("triangulate_faces", &cgalpy::pmp::triangulate_faces_r<Pm>,
        py::arg("face_range"), py::arg("pm"), py::arg("np") = py::dict(),
        "Triangulates selected faces of a polygon mesh.");
  m.def("triangulate_polygons", &cgalpy::pmp::triangulate_polygons,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict(),
        "Triangulates polygon soup faces.");
  m.def("triangulate_polygons", &cgalpy::pmp::triangulate_polygons_np,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict(),
        "Triangulates polygon soup faces from a NumPy point array.");

  using Tfv = cgalpy::pmp::Triangulate_faces_visitor<Pm>;
  static PyType_Slot triangulate_faces_visitor_slots[] = {
    {Py_tp_traverse, (void*) Tfv::tp_traverse},
    {Py_tp_clear, (void*) Tfv::tp_clear},
    {0, nullptr}
  };
  py::class_<Tfv>(m, "Triangulate_faces_visitor",
                  "Visitor for triangulate_faces().",
                  py::type_slots(triangulate_faces_visitor_slots))
    .def(py::init<>(), "Constructs a triangulate faces visitor.")
    .def("set_before_subface_creations",
         &Tfv::set_before_subface_creations,
         py::arg("callback"),
         "Sets a callback called before a face is split into subfaces.")
    .def("set_after_subface_creations",
         &Tfv::set_after_subface_creations,
         py::arg("callback"),
         "Sets a callback called after a face has been split into subfaces.")
    .def("set_after_subface_created",
         &Tfv::set_after_subface_created,
         py::arg("callback"),
         "Sets a callback called after each new subface is created.")
    .def("set_accept_face",
         &Tfv::set_accept_face,
         py::arg("callback"),
         "Sets a callback deciding whether a candidate triangle is accepted.");

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  //! \todo Fix interpolated_corrected_curvatures to use epeck
  m.def("isotropic_remeshing",
        &cgalpy::pmp::isotropic_remeshing_sf<Pm, cgalpy::pmp::Adaptive_sizing_field<Pm>>,
        py::arg("faces"), py::arg("sizing"), py::arg("pmesh"), py::arg("np") = py::dict(),
        "Performs isotropic remeshing using an adaptive sizing field.");

  m.def("isotropic_remeshing",
        &cgalpy::pmp::isotropic_remeshing_sf<Pm, cgalpy::pmp::Uniform_sizing_field<Pm>>,
        py::arg("faces"), py::arg("target_edge_length"), py::arg("pmesh"), py::arg("np") = py::dict(),
        "Performs isotropic remeshing using a uniform target edge length.");
  // m.def("isotropic_remeshing",
  //       &cgalpy::pmp::isotropic_remeshing_sf<Pm, cgalpy::pmp::Custom_sizing_field<Pm>>,
  //       py::arg("faces"), py::arg("target_edge_length"), py::arg("pmesh"),
  //       py::arg("np") = py::dict());

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  // The CGAL code is faulty and cannot coop with Polyhedron_3
  m.def("surface_Delaunay_remeshing", &cgalpy::pmp::surface_Delaunay_remeshing<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict(),
        "Performs surface Delaunay remeshing.");
#endif

  //! \todo Fix interpolated_corrected_curvatures to use epeck
  m.def("split_long_edges", &cgalpy::pmp::split_long_edges<Pm>,
        py::arg("edge_range"), py::arg("max_length"), py::arg("pmesh"),
        py::arg("np") = py::dict(),
        "Splits edges longer than the given maximum length.");
#endif

  m.def("extrude_mesh", &cgalpy::pmp::extrude_mesh_v<Pm, Pm>,
        py::arg("imesh"), py::arg("omesh"), py::arg("v"),
        py::arg("np_in") = py::dict(), py::arg("np_out") = py::dict(),
        "Extrudes an input mesh into an output mesh.");
  m.def("angle_and_area_smoothing", &cgalpy::pmp::angle_and_area_smoothing<Pm>,
        py::arg("faces"), py::arg("tmesh"), py::arg("np") = py::dict(),
        "Smooths selected faces by angle and area optimization.");
  m.def("angle_and_area_smoothing", &cgalpy::pmp::angle_and_area_smoothing_m<Pm>,
        py::arg("pmesh"), py::arg("np") = py::dict(),
        "Smooths a polygon mesh by angle and area optimization.");
  m.def("tangential_relaxation", &cgalpy::pmp::tangential_relaxation<Pm>,
        py::arg("pm"), py::arg("np") = py::dict(),
        "Applies tangential relaxation to a polygon mesh.");

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  //! \todo Fix interpolated_corrected_curvatures to use epeck
  m.def("smooth_shape", &cgalpy::pmp::smooth_shape<Pm>,
        py::arg("pm"), py::arg("time"), py::arg("np") = py::dict(),
        "Smooths the shape of a polygon mesh.");
#endif

  m.def("random_perturbation", &cgalpy::pmp::random_perturbation<Pm>,
        py::arg("tmesh"), py::arg("perturbation_max_size"), py::arg("np") = py::dict(),
        "Randomly perturbs vertices of a triangle mesh.");
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("random_perturbation", &cgalpy::pmp::random_perturbation_v_pol3<Pm>,
        py::arg("vertices"), py::arg("tmesh"), py::arg("perturbation_max_size"), py::arg("np") = py::dict(),
        "Randomly perturbs selected vertices of a triangle mesh.");
#else
  m.def("random_perturbation", &cgalpy::pmp::random_perturbation_v<Pm>,
        py::arg("vertices"), py::arg("tmesh"), py::arg("perturbation_max_size"), py::arg("np") = py::dict(),
        "Randomly perturbs selected vertices of a triangle mesh.");
#endif

  using Asf = cgalpy::pmp::Adaptive_sizing_field<Pm>;
  using Gt = boost::graph_traits<Pm>;
  using face_descriptor = Gt::face_descriptor;

#if CGAL_VERSION_NR > 1060200900
  py::class_<Asf>(m, "Adaptive_sizing_field",
                  pmp_doc::PMPSizingField_class)

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
    //! \todo Fix interpolated_corrected_curvatures to use epeck
    .def(py::init<FT, const std::tuple<FT, FT>&,
         const std::vector<face_descriptor>&, Pm&, const py::dict&>(),
         py::arg("tol"), py::arg("edge_len_min_max"),
         py::arg("face_range"), py::arg("pmesh"), py::arg("np") = py::dict(),
         "Constructs an adaptive sizing field.")
#endif

    .def("at", &Asf::at,
         py::arg("vertex"), py::arg("pmesh"),
         pmp_doc::PMPSizingField_at)
    // .def("is_too_long", &Asf::is_too_long)
    // .def("is_too_short", &Asf::is_too_short)
    .def("split_placement", &Asf::split_placement,
         py::arg("halfedge"), py::arg("pmesh"),
         pmp_doc::PMPSizingField_split_placement)

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
    //! \todo Fix interpolated_corrected_curvatures to use epeck
    .def("register_split_vertex", &Asf::register_split_vertex,
         py::arg("vertex"), py::arg("pmesh"),
         pmp_doc::PMPSizingField_register_split_vertex)
#endif
    ;
#endif

  using Usf = cgalpy::pmp::Uniform_sizing_field<Pm>; // Work in progress
  py::class_<Usf>(m, "Uniform_sizing_field",
                  pmp_doc::PMPSizingField_class)
    .def(py::init<const FT, const Pm&>(),
         py::arg("size"), py::arg("pmesh"),
         "Constructs a uniform sizing field.")
    .def("at", &Usf::at,
         py::arg("vertex"), py::arg("pmesh"),
         pmp_doc::PMPSizingField_at)
    // .def("is_too_long", &Usf::is_too_long)
    // .def("is_too_short", &Usf::is_too_short)
    .def("split_placement", &Usf::split_placement,
         py::arg("halfedge"), py::arg("pmesh"),
         pmp_doc::PMPSizingField_split_placement)
    .def("register_split_vertex", &Usf::register_split_vertex,
         py::arg("vertex"), py::arg("pmesh"),
         pmp_doc::PMPSizingField_register_split_vertex)
    ;
}
