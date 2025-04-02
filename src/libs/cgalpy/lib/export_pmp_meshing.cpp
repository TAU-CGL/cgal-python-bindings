// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <vector>
#include <tuple>

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

#include "CGALPY/pmp_np_parser.hpp"
#include "CGALPY/pmp_helpers.hpp"
#include "CGALPY/Adaptive_sizing_field.hpp"
#include "CGALPY/Uniform_sizing_field.hpp"
#include "CGALPY/Custom_sizing_field.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

//!
template <typename PolygonMesh>
auto remesh_planar_patches(PolygonMesh& pmesh,
                           const py::dict& np_in = py::dict(),
                           const py::dict& np_out = py::dict()) {
  using Pm = PolygonMesh;

  auto eicm = get_edge_prop_map<Pm, bool>
    (pmesh, "edge_is_constrained_map",
     np_in.contains("edge_is_constrained_map") ?
     np_in["edge_is_constrained_map"] : py::none());
  auto fpm = get_face_prop_map<Pm, int>(pmesh, "face_patch_map",
    np_in.contains("face_patch_map") ? np_in["face_patch_map"] : py::none());
  auto vcm = get_vertex_prop_map<Pm, std::size_t>
    (pmesh, "vertex_corner_map",
     np_in.contains("vertex_corner_map") ?
     np_in["vertex_corner_map"] : py::none());

  auto fpm2 = get_face_prop_map<Pm, int>(pmesh, "face_patch_map",
    np_out.contains("face_patch_map") ? np_out["face_patch_map"] : py::none());
  auto vcm2 = get_vertex_prop_map<Pm, std::size_t>
    (pmesh, "vertex_corner_map",
    np_out.contains("vertex_corner_map") ?
     np_out["vertex_corner_map"] : py::none());

  Pm out;

  PMP::remesh_planar_patches(pmesh,
                             out,
                             internal::parse_pmp_np<Pm>(np_in)
                             .edge_is_constrained_map(eicm)
                             .face_patch_map(fpm)
                             .vertex_corner_map(vcm),
                             internal::parse_pmp_np<Pm>(np_out)
                             .face_patch_map(fpm2)
                             .vertex_corner_map(vcm2)
                             );

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np_in.contains("edge_is_constrained_map"))
    pmesh.remove_property_map(eicm);
  if (! np_in.contains("face_patch_map")) pmesh.remove_property_map(fpm);
  if (! np_in.contains("vertex_corner_map")) pmesh.remove_property_map(vcm);
#endif

  return out;
}

//!
template <typename PolygonMesh, typename FacePatchMap, typename VertexCornerMap,
          typename EdgeIsConstrainedMap>
auto remesh_almost_planar_patches(PolygonMesh tm_in,
                                  std::size_t nb_patches,
                                  std::size_t nb_corners,
                                  FacePatchMap face_patch_map,
                                  VertexCornerMap vertex_corner_map,
                                  EdgeIsConstrainedMap ecm,
                                  const py::dict& np_in = py::dict(),
                                  const py::dict& np_out = py::dict()) {
  using Pm = PolygonMesh;

  Pm pm_out;

  auto fpm = get_face_prop_map<Pm, int>
    (pm_out, "INTERNAL_MAP1",
     np_out.contains("face_patch_map") ? np_out["face_patch_map"] : py::none());
  auto vcm = get_vertex_prop_map<Pm, int>
    (pm_out, "INTERNAL_MAP2",
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
    retv = PMP::remesh_almost_planar_patches(tm_in, pm_out, nb_patches,
                                             nb_corners, face_patch_map,
                                             vertex_corner_map, ecm,
                                             internal::parse_pmp_np<Pm>(np_in)
                                             .patch_normal_map(rpm),
                                             internal::parse_pmp_np<Pm>(np_out)
                                             .face_patch_map(fpm)
                                             .vertex_corner_map(vcm));
  }
  else {
    retv = PMP::remesh_almost_planar_patches(tm_in, pm_out, nb_patches,
                                             nb_corners, face_patch_map,
                                             vertex_corner_map, ecm,
                                             internal::parse_pmp_np<Pm>(np_in),
                                             internal::parse_pmp_np<Pm>(np_out)
                                             .face_patch_map(fpm)
                                             .vertex_corner_map(vcm));
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
auto refine(PolygonMesh& tmesh,
            const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& faces,
            const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  using Vd = typename Gt::vertex_descriptor;
  std::vector<Fd> faces_out;
  std::vector<Vd> vertices_out;
  PMP::refine(tmesh, faces, std::back_inserter(faces_out),
              std::back_inserter(vertices_out),
              internal::parse_pmp_np<Pm>(np));
  return std::make_tuple(faces_out, vertices_out);
}

// only for polyhedron!
template <typename TriangleMesh>
auto fair(TriangleMesh& tmesh,
          const std::vector<Point_3>& vertices,
          const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Vh = typename TriangleMesh::Vertex_handle;
  return PMP::fair(tmesh, vertices, internal::parse_pmp_np<Tm>(np));
}

//!
template <typename PolygonMesh>
auto triangulate_faces(PolygonMesh& pm, const py::dict& np) {
  using Pm = PolygonMesh;
  // TODO: visitor?
  // auto vpm = get_vertex_point_map(pm, np);
  return PMP::triangulate_faces(pm, internal::parse_pmp_np<Pm>(np)
                                // .vertex_point_map(vpm)
                                );
}

//!
template <typename PolygonMesh>
auto triangulate_faces_r(const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                         PolygonMesh& pm,
                         const py::dict& np) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  return PMP::triangulate_faces(face_range, pm, internal::parse_pmp_np<Pm>(np));
}

//!
auto triangulate_polygons(std::vector<Point_3>& points,
                          std::vector<std::vector<std::size_t>>& polygons,
                          const py::dict& np = py::dict()) {
  PMP::triangulate_polygons(points, polygons,
                            internal::parse_named_parameters(np));
  return std::make_tuple(points, polygons);
}

//!
template <typename PolygonMesh, typename SizingFunction>
auto isotropic_remeshing_sf(const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                            SizingFunction& sizing,
                            PolygonMesh& pmesh,
                            const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  PMP::isotropic_remeshing(face_range, sizing, pmesh,
                           internal::parse_pmp_np<Pm>(np));

}

//!
template <typename PolygonMesh>
auto surface_Delaunay_remeshing(PolygonMesh& tmesh,
                                const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto eicm = get_edge_prop_map<Pm, bool>
    (tmesh, "INTERNAL_MAP0",
     np.contains("edge_is_constrained_map") ?
     np["edge_is_constrained_map"] : py::none());
  auto fpm = get_face_prop_map<Pm, int>
    (tmesh, "INTERNAL_MAP1",
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
    retv = PMP::surface_Delaunay_remeshing(tmesh,
                                           internal::parse_pmp_np<Pm>(np)
                                           .edge_is_constrained_map(eicm)
                                           .face_patch_map(fpm)
                                           .facet_size(mfs)
                                           .facet_angle(mfa)
                                           .facet_distance(mfd)
                                           .mesh_edge_size(mes));
  }
  else {
    retv = PMP::surface_Delaunay_remeshing(tmesh,
                                           internal::parse_pmp_np<Pm>(np)
                                           .edge_is_constrained_map(eicm)
                                           .face_patch_map(fpm)
                                           .facet_size(mfs)
                                           .facet_angle(mfa)
                                           .facet_distance(mfd));
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
                      double max_length,
                      PolygonMesh& pmesh,
                      const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Ed = typename Gt::edge_descriptor;
  // auto vpm = get_vertex_point_map(pmesh, np);
  auto fim = get_face_prop_map<Pm, std::size_t>
    (pmesh, "INTERNAL_MAP0",
     np.contains("face_index_map") ? np["face_patch_index_map"] : py::none());
  auto fpm = get_face_prop_map<Pm, int>
    (pmesh, "INTERNAL_MAP1",
     np.contains("face_patch_map") ? np["face_patch_map"] : py::none());
  auto eicm = get_edge_prop_map<Pm, bool>
    (pmesh, "INTERNAL_MAP2",
     np.contains("edge_index_copy_map") ?
     np["edge_index_copy_map"] : py::none());
  // turn edge_range to a vector
  std::vector<Ed> edge_vec;
  PMP::split_long_edges(edge_vec, max_length, pmesh,
                        internal::parse_pmp_np<Pm>(np)
                        // .vertex_point_map(vpm)
                        .face_index_map(fim)
                        .face_patch_map(fpm)
                        .edge_is_constrained_map(eicm));
  return edge_vec;
}

//!
template <typename InputMesh, typename OutputMesh>
auto extrude_mesh_v(const InputMesh& imesh,
                    OutputMesh& omesh,
                    Vector_3 v,
                    const py::dict& np_in = py::dict(),
                    const py::dict& np_out = py::dict()) {
  PMP::extrude_mesh(imesh, omesh, v,
                    internal::parse_pmp_np<OutputMesh>(np_in),
                    internal::parse_pmp_np<OutputMesh>(np_out));
}

//!
template <typename PolygonMesh>
auto angle_and_area_smoothing(const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& faces,
                              PolygonMesh& pmesh,
                              const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  auto eicm = get_edge_prop_map<Pm, bool>
    (pmesh, "INTERNAL_MAP0",
     np.contains("edge_is_constrained_map") ?
     np["edge_is_constrained_map"] : py::none());
  auto vicm = get_vertex_prop_map<Pm, bool>
    (pmesh, "INTERNAL_MAP1",
    np.contains("vertex_is_constrained_map") ?
     np["vertex_is_constrained_map"] : py::none());
  PMP::angle_and_area_smoothing(faces, pmesh,
                                internal::parse_pmp_np<Pm>(np)
                                .edge_is_constrained_map(eicm)
                                .vertex_is_constrained_map(vicm));

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
auto angle_and_area_smoothing_m(PolygonMesh& pmesh,
                              const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto eicm = get_edge_prop_map<Pm, bool>
    (pmesh, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ?
     np["edge_is_constrained_map"] : py::none());
  auto vicm = get_vertex_prop_map<Pm, bool>
    (pmesh, "INTERNAL_MAP1",
    np.contains("vertex_is_constrained_map") ?
     np["vertex_is_constrained_map"] : py::none());
  PMP::angle_and_area_smoothing(pmesh,
                                internal::parse_pmp_np<Pm>(np)
                                .edge_is_constrained_map(eicm)
                                .vertex_is_constrained_map(vicm));

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map"))
    pmesh.remove_property_map(eicm);
  if (! np.contains("vertex_is_constrained_map"))
    pmesh.remove_property_map(vicm);
#endif
}

//
template <typename PolygonMesh>
auto smooth_shape(PolygonMesh& pmesh,
                  const double time,
                  const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Vd = typename boost::graph_traits<Pm>::vertex_descriptor;
  auto vpm = get_vertex_point_map(pmesh, np);
  auto propmap = get_vertex_prop_map<Pm, bool>
    (pmesh, "vertex_is_constrained_map",
     np.contains("vertex_is_constrained_map") ?
     np["vertex_is_constrained_map"] : py::none());
  return PMP::smooth_shape(pmesh, time,
                    internal::parse_named_parameters(np)
                    .vertex_is_constrained_map(propmap)
                    .vertex_point_map(vpm));
}

//!
template <typename TriangleMesh>
auto random_perturbation(TriangleMesh& tmesh,
                         const double& perturbation_max_size,
                         const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  auto vicm = get_vertex_prop_map<Tm, Point_3>
    (tmesh, "INTERNAL_MAP0", np.contains("vertex_point_map") ?
     np["vertex_point_map"] : py::none());
  PMP::random_perturbation(tmesh, perturbation_max_size,
                           internal::parse_pmp_np<Tm>(np)
                           .vertex_point_map(vicm));

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("vertex_point_map")) tmesh.remove_property_map(vicm);
#endif
}

//!
template <typename PolygonMesh>
auto random_perturbation_v(const std::vector<typename boost::graph_traits<PolygonMesh>::vertex_descriptor>& vertices,
                           PolygonMesh& pmesh,
                           const double& perturbation_max_size,
                           const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Vd = typename boost::graph_traits<Pm>::vertex_descriptor;
  auto vicm = get_vertex_prop_map<Pm, Point_3>
    (pmesh, "INTERNAL_MAP0",
     np.contains("vertex_point_map") ? np["vertex_point_map"] : py::none());
  PMP::random_perturbation(vertices, pmesh, perturbation_max_size,
                           internal::parse_pmp_np<Pm>(np)
                           .vertex_point_map(vicm));
}

} // namespace pmp

//!
void export_pmp_meshing(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;
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

  m.def("remesh_planar_patches", &pmp::remesh_planar_patches<Pm>,
        py::arg("pm"), py::arg("np_in") = py::dict(),
        py::arg("np_out") = py::dict());

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  m.def("remesh_almost_planar_patches",
        &pmp::remesh_almost_planar_patches<Pm, Fpm, Vcm, Eicm>,
        py::arg("tm_in"), py::arg("nb_patches"), py::arg("nb_corners"),
        py::arg("face_patch_map"), py::arg("vertex_corner_map"), py::arg("ecm"),
        py::arg("np_in") = py::dict(), py::arg("np_out") = py::dict());
#endif

  m.def("refine", &pmp::refine<Pm>,
        py::arg("tmesh"), py::arg("faces"), py::arg("np") = py::dict());

#if 0 // broken for now because of CGAL
  m.def("fair", &pmp::fair<Pm>,
        py::arg("tmesh"), py::arg("vertices"),
        py::arg("np") = py::dict());
#endif

  m.def("triangulate_faces", &pmp::triangulate_faces<Pm>,
        py::arg("pm"), py::arg("np") = py::dict());
  m.def("triangulate_faces", &pmp::triangulate_faces_r<Pm>,
        py::arg("face_range"), py::arg("pm"),
        py::arg("np") = py::dict());
  m.def("triangulate_polygons", &pmp::triangulate_polygons,
      py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  //! \todo Fix interpolated_corrected_curvatures to use epeck
  m.def("isotropic_remeshing",
        &pmp::isotropic_remeshing_sf<Pm, pmp::Adaptive_sizing_field<Pm>>,
        py::arg("faces"), py::arg("sizing"), py::arg("pmesh"),
        py::arg("np") = py::dict());

  m.def("isotropic_remeshing",
        &pmp::isotropic_remeshing_sf<Pm, pmp::Uniform_sizing_field<Pm>>,
        py::arg("faces"), py::arg("target_edge_length"), py::arg("pmesh"),
        py::arg("np") = py::dict());
  // m.def("isotropic_remeshing",
  //       &pmp::isotropic_remeshing_sf<Pm, pmp::Custom_sizing_field<Pm>>,
  //       py::arg("faces"), py::arg("target_edge_length"), py::arg("pmesh"),
  //       py::arg("np") = py::dict());

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  // The CGAL code is faulty and cannot coop with Polyhedron_3
  m.def("surface_Delaunay_remeshing", &pmp::surface_Delaunay_remeshing<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict());
#endif

  //! \todo Fix interpolated_corrected_curvatures to use epeck
  m.def("split_long_edges", &pmp::split_long_edges<Pm>,
        py::arg("edge_range"), py::arg("max_length"), py::arg("pmesh"),
        py::arg("np") = py::dict());
#endif

m.def("extrude_mesh", &pmp::extrude_mesh_v<Pm, Pm>,
        py::arg("imesh"), py::arg("omesh"), py::arg("v"),
        py::arg("np_in") = py::dict(), py::arg("np_out") = py::dict());
  m.def("angle_and_area_smoothing", &pmp::angle_and_area_smoothing<Pm>,
        py::arg("faces"), py::arg("tmesh"),
        py::arg("np") = py::dict());
  m.def("angle_and_area_smoothing", &pmp::angle_and_area_smoothing_m<Pm>,
        py::arg("pmesh"), py::arg("np") = py::dict());
  // m.def("tangential_relaxation", &pmp::tangential_relaxation<Pm>, // changed in master
  //       py::arg("pm"), py::arg("np") = py::dict());

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  //! \todo Fix interpolated_corrected_curvatures to use epeck
  m.def("smooth_shape", &pmp::smooth_shape<Pm>,
        py::arg("pm"), py::arg("time"), py::arg("np") = py::dict());
#endif

  m.def("random_perturbation", &pmp::random_perturbation<Pm>,
        py::arg("tmesh"), py::arg("perturbation_max_size"),
        py::arg("np") = py::dict());
  m.def("random_perturbation", &pmp::random_perturbation_v<Pm>,
        py::arg("vertices"), py::arg("tmesh"), py::arg("perturbation_max_size"),
        py::arg("np") = py::dict());

  using Asf = pmp::Adaptive_sizing_field<Pm>;
  using Gt = boost::graph_traits<Pm>;
  using face_descriptor = Gt::face_descriptor;

#if CGAL_VERSION_NR > 1060100900
  py::class_<Asf>(m, "Adaptive_sizing_field")

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
    //! \todo Fix interpolated_corrected_curvatures to use epeck
    .def(py::init<FT, const std::tuple<FT, FT>&,
         const std::vector<face_descriptor>&, Pm&, const py::dict&>(),
         py::arg("tol"), py::arg("edge_len_min_max"),
         py::arg("face_range"), py::arg("pmesh"), py::arg("np") = py::dict())
#endif

    .def("at", &Asf::at)
    // .def("is_too_long", &Asf::is_too_long)
    // .def("is_too_short", &Asf::is_too_short)
    .def("split_placement", &Asf::split_placement)

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
    //! \todo Fix interpolated_corrected_curvatures to use epeck
    .def("register_split_vertex", &Asf::register_split_vertex)
#endif
    ;
#endif

  using Usf = pmp::Uniform_sizing_field<Pm>; // Work in progress
  py::class_<Usf>(m, "Uniform_sizing_field")
    .def(py::init<const FT, const Pm&>())
    .def("at", &Usf::at)
    // .def("is_too_long", &Usf::is_too_long)
    // .def("is_too_short", &Usf::is_too_short)
    .def("split_placement", &Usf::split_placement)
    .def("register_split_vertex", &Usf::register_split_vertex)
    ;
}
