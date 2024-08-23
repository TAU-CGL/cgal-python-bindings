#include <nanobind/stl/vector.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/tuple.h>

#include <CGAL/Polygon_mesh_processing/autorefinement.h>
#include <CGAL/Polygon_mesh_processing/clip.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_mesh_processing/intersection.h>

#include "CGALPY/pmp_np_parser.hpp"
#include "CGALPY/pmp_helpers.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/Autorefinement_visitor.hpp"
#include "CGALPY/Corefine_visitor.hpp"
#include "CGALPY/Default_visitor.hpp"
#include "CGALPY/Non_manifold_output_visitor.hpp"

namespace py = nanobind;

namespace pmp {

//
template <typename PolygonMesh>
auto autorefine(PolygonMesh& tm,
                const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  PMP::autorefine(tm, internal::parse_pmp_np<PolygonMesh>(np)
                  // .vertex_point_map(vpm)
                  );
}

auto autorefine_triangle_soup(
                         std::vector<Point_3>& points,
                         std::vector<std::vector<std::size_t>>& polygons,
                         const py::dict& np = py::dict()) {

  bool visitor = np.contains("visitor");
  if (visitor) {
    try {
      auto v = py::cast<pmp::Autorefinement_visitor>(np["visitor"]);
      PMP::autorefine_triangle_soup(points, polygons, internal::parse_named_parameters(np).visitor(v));
    }
    catch (const py::cast_error& e) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    PMP::autorefine_triangle_soup(points, polygons, internal::parse_named_parameters(np));
  }

  return std::make_tuple(points, polygons);
}

template <typename PolygonMesh>
bool clip(PolygonMesh& tm, PolygonMesh& clipper,
          const py::dict& np_tm = py::dict(),
          const py::dict& np_c = py::dict()) {
  using Pm = PolygonMesh;

  // auto vpm1 = get_vertex_point_map(tm, np_tm);
  // auto vpm2 = get_vertex_point_map(clipper, np_c);
  auto eicm1 = get_edge_prop_map<Pm, bool>(tm, "INTERNAL_MAP0",
    np_tm.contains("edge_is_constrained_map") ? np_tm["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Pm, bool>(clipper, "INTERNAL_MAP1",
    np_c.contains("edge_is_constrained_map") ? np_c["edge_internal_map"] : py::none());


  bool visitor1 = np_tm.contains("visitor");
  bool visitor2 = np_c.contains("visitor");
  bool res;
  if (visitor1 && visitor2) {
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).visitor(v2).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).visitor(v2).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).visitor(v2).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).visitor(v2).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
      std::cerr << "Visitor type not recognized\n";
    }
  } else if (visitor1) {
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np_tm["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_tm["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
      std::cerr << "Visitor type not recognized\n";
    }
  } else if (visitor2) {
    try {
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).visitor(v2).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).visitor(v2).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
      std::cerr << "Visitor type not recognized\n";
    }
  } else {
    res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).edge_is_constrained_map(eicm1),
                    internal::parse_pmp_np<PolygonMesh>(np_c).edge_is_constrained_map(eicm2));
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np_tm.contains("edge_is_constrained_map")) {
    tm.remove_property_map(eicm1);
  }
  if (!np_c.contains("edge_is_constrained_map")) {
    clipper.remove_property_map(eicm2);
  }
#endif
  return res;
}

template <typename TriangleMesh>
auto clip_c(TriangleMesh& tm,
            const Iso_cuboid_3& box,
            const py::dict& np = py::dict()) {
  using Pm = TriangleMesh;
  bool visitor = np.contains("visitor");
  if (visitor) {
    try {
      auto v = py::cast<pmp::Corefine_visitor<Pm>>(np["visitor"]);
      return PMP::clip(tm, box, internal::parse_pmp_np<TriangleMesh>(np).visitor(v));
    } catch (const py::cast_error&) {
    }
    try {
      auto v = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np["visitor"]);
      return PMP::clip(tm, box, internal::parse_pmp_np<TriangleMesh>(np).visitor(v));
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  } else {
    return PMP::clip(tm, box, internal::parse_pmp_np<TriangleMesh>(np));
  }
}

template <typename TriangleMesh>
auto clip_p(TriangleMesh& tm,
            const Plane_3& plane,
            const py::dict& np = py::dict()) {
  using Pm = TriangleMesh;
  bool visitor = np.contains("visitor");
  if (visitor) {
    try {
      auto v = py::cast<pmp::Corefine_visitor<Pm>>(np["visitor"]);
      return PMP::clip(tm, plane, internal::parse_pmp_np<TriangleMesh>(np).visitor(v));
    } catch (const py::cast_error&) {
    }
    try {
      auto v = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np["visitor"]);
      return PMP::clip(tm, plane, internal::parse_pmp_np<TriangleMesh>(np).visitor(v));
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  } else {
    return PMP::clip(tm, plane, internal::parse_pmp_np<TriangleMesh>(np));
  }
}

template <typename PolygonMesh>
void corefine(PolygonMesh& tm1, PolygonMesh& tm2,
              const py::dict& np1 = py::dict(),
              const py::dict& np2 = py::dict()) {
  // auto vpm1 = get_vertex_point_map(tm1, np1);
  // auto vpm2 = get_vertex_point_map(tm2, np2);
  auto eicm1 = get_edge_prop_map<PolygonMesh, bool>(tm1, "INTERNAL_MAP0",
    np1.contains("edge_is_constrained_map") ? np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<PolygonMesh, bool>(tm2, "INTERNAL_MAP1",
    np2.contains("edge_is_constrained_map") ? np2["edge_internal_map"] : py::none());
  // np1 can have a corefinement visitor
  bool visitor = np1.contains("visitor");
  if (visitor) {
    // try to cast to Non_manifold_output_visitor or Default_visitor
    try {
      auto visitor = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np1["visitor"]);
      PMP::corefine(tm1, tm2, internal::parse_pmp_np<PolygonMesh>(np1)
                    // .vertex_point_map(vpm1)
                    .edge_is_constrained_map(eicm1)
                    .visitor(visitor),
                    internal::parse_pmp_np<PolygonMesh>(np2)
                    // .vertex_point_map(vpm2)
                    .edge_is_constrained_map(eicm2)
                    );
    } catch (const py::cast_error&) {
    }
    try {
      auto visitor = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np1["visitor"]);
      PMP::corefine(tm1, tm2, internal::parse_pmp_np<PolygonMesh>(np1)
                    // .vertex_point_map(vpm1)
                    .edge_is_constrained_map(eicm1)
                    .visitor(visitor),
                    internal::parse_pmp_np<PolygonMesh>(np2)
                    // .vertex_point_map(vpm2)
                    .edge_is_constrained_map(eicm2)
                    );
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    PMP::corefine(tm1, tm2, internal::parse_pmp_np<PolygonMesh>(np1)
                  // .vertex_point_map(vpm1)
                  .edge_is_constrained_map(eicm1)
                  ,
                  internal::parse_pmp_np<PolygonMesh>(np2)
                  // .vertex_point_map(vpm2)
                  .edge_is_constrained_map(eicm2)
                  );
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np1.contains("edge_is_constrained_map")) tm1.remove_property_map(eicm1);
  if (!np2.contains("edge_is_constrained_map")) tm2.remove_property_map(eicm2);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
}

template <typename PolygonMesh>
auto corefine_and_compute_boolean_operations(PolygonMesh& pm1, PolygonMesh& pm2,
                                                 const py::dict& np1 = py::dict(),
                                                 const py::dict& np2 = py::dict(),
                                                 // const py::tuple& np_out = py::tuple()) {
                                                 const std::array<py::dict, 4>& np_out = std::array<py::dict, 4>()) {
                                                 
  using Pm = PolygonMesh;
  // auto vpm1 = get_vertex_point_map(pm1, np1);
  // auto vpm2 = get_vertex_point_map(pm2, np2);
  auto eicm1 = get_edge_prop_map<Pm, bool>(pm1, "INTERNAL_MAP0",
    np1.contains("edge_is_constrained_map") ? np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Pm, bool>(pm2, "INTERNAL_MAP1",
    np2.contains("edge_is_constrained_map") ? np2["edge_internal_map"] : py::none());

  // auto vpm_out1 = get_vertex_point_map(pm_out1, np_out[0]);
  // auto vpm_out2 = get_vertex_point_map(pm_out2, np_out[1]);
  // auto vpm_out3 = get_vertex_point_map(pm_out3, np_out[2]);
  // auto vpm_out4 = get_vertex_point_map(pm_out4, np_out[3]);
  auto eicm_out1 = get_edge_prop_map<Pm, bool>(pm1, "INTERNAL_MAP2",
    np_out[0].contains("edge_is_constrained_map") ? np_out[0]["edge_internal_map"] : py::none());
  auto eicm_out2 = get_edge_prop_map<Pm, bool>(pm2, "INTERNAL_MAP3",
    np_out[1].contains("edge_is_constrained_map") ? np_out[1]["edge_internal_map"] : py::none());
  auto eicm_out3 = get_edge_prop_map<Pm, bool>(pm1, "INTERNAL_MAP4",
    np_out[2].contains("edge_is_constrained_map") ? np_out[2]["edge_internal_map"] : py::none());
  auto eicm_out4 = get_edge_prop_map<Pm, bool>(pm2, "INTERNAL_MAP5",
    np_out[3].contains("edge_is_constrained_map") ? np_out[3]["edge_internal_map"] : py::none());

  Pm out_union, out_intersection, tm1_minus_tm2, tm2_minus_tm1;

  std::array<bool, 4> res;
  std::array<std::optional<Pm*>, 4> outputs = {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1};

  bool fimb1 = np1.contains("face_index_map");
  bool fimb2 = np2.contains("face_index_map");
  bool visitor_flag = np1.contains("visitor");
  if (fimb1 && fimb2) {
    if (visitor_flag) {
      try {
        auto visit = py::cast<pmp::Corefine_visitor<Pm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP7",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                      internal::parse_pmp_np<PolygonMesh>(np1)
                                      .face_index_map(fim1)
                                      // .vertex_point_map(vpm1)
                                      .edge_is_constrained_map(eicm1)
                                      .visitor(visit),
                                      internal::parse_pmp_np<PolygonMesh>(np2)
                                      .face_index_map(fim2)
                                      // .vertex_point_map(vpm2)
                                      .edge_is_constrained_map(eicm2),
                                      std::make_tuple(
                                        internal::parse_pmp_np<PolygonMesh>(np_out[0])
                                        // .vertex_point_map(vpm_out1)
                                        .edge_is_constrained_map(eicm_out1),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[1])
                                        // .vertex_point_map(vpm_out2)
                                        .edge_is_constrained_map(eicm_out2),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[2])
                                        // .vertex_point_map(vpm_out3)
                                        .edge_is_constrained_map(eicm_out3),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[3])
                                        // .vertex_point_map(vpm_out4)
                                        .edge_is_constrained_map(eicm_out4)
                                      )
                                      );
      } catch (const py::cast_error&) {
      }
      try {
        auto visit = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP7",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                      internal::parse_pmp_np<PolygonMesh>(np1)
                                      .face_index_map(fim1)
                                      // .vertex_point_map(vpm1)
                                      .edge_is_constrained_map(eicm1)
                                      .visitor(visit),
                                      internal::parse_pmp_np<PolygonMesh>(np2)
                                      .face_index_map(fim2)
                                      // .vertex_point_map(vpm2)
                                      .edge_is_constrained_map(eicm2),
                                      std::make_tuple(
                                        internal::parse_pmp_np<PolygonMesh>(np_out[0])
                                        // .vertex_point_map(vpm_out1)
                                        .edge_is_constrained_map(eicm_out1),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[1])
                                        // .vertex_point_map(vpm_out2)
                                        .edge_is_constrained_map(eicm_out2),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[2])
                                        // .vertex_point_map(vpm_out3)
                                        .edge_is_constrained_map(eicm_out3),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[3])
                                        // .vertex_point_map(vpm_out4)
                                        .edge_is_constrained_map(eicm_out4)
                                      )
                                      );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP7",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                    internal::parse_pmp_np<PolygonMesh>(np1)
                                    .face_index_map(fim1)
                                    // .vertex_point_map(vpm1)
                                    .edge_is_constrained_map(eicm1),
                                    internal::parse_pmp_np<PolygonMesh>(np2)
                                    .face_index_map(fim2)
                                    // .vertex_point_map(vpm2)
                                    .edge_is_constrained_map(eicm2),
                                    std::make_tuple(
                                      internal::parse_pmp_np<PolygonMesh>(np_out[0])
                                      // .vertex_point_map(vpm_out1)
                                      .edge_is_constrained_map(eicm_out1),
                                      internal::parse_pmp_np<PolygonMesh>(np_out[1])
                                      // .vertex_point_map(vpm_out2)
                                      .edge_is_constrained_map(eicm_out2),
                                      internal::parse_pmp_np<PolygonMesh>(np_out[2])
                                      // .vertex_point_map(vpm_out3)
                                      .edge_is_constrained_map(eicm_out3),
                                      internal::parse_pmp_np<PolygonMesh>(np_out[3])
                                      // .vertex_point_map(vpm_out4)
                                      .edge_is_constrained_map(eicm_out4)
                                    )
                                    );
    }
    
  }
  else if (fimb1) {
    if (visitor_flag) {
      try {
        auto visit = py::cast<pmp::Corefine_visitor<Pm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                      internal::parse_pmp_np<PolygonMesh>(np1)
                                      // .vertex_point_map(vpm1)
                                      .edge_is_constrained_map(eicm1)
                                      .visitor(visit)
                                      .face_index_map(fim1),
                                      internal::parse_pmp_np<PolygonMesh>(np2)
                                      // .vertex_point_map(vpm2)
                                      .edge_is_constrained_map(eicm2),
                                      std::make_tuple(
                                        internal::parse_pmp_np<PolygonMesh>(np_out[0])
                                        // .vertex_point_map(vpm_out1)
                                        .edge_is_constrained_map(eicm_out1),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[1])
                                        // .vertex_point_map(vpm_out2)
                                        .edge_is_constrained_map(eicm_out2),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[2])
                                        // .vertex_point_map(vpm_out3)
                                        .edge_is_constrained_map(eicm_out3),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[3])
                                        // .vertex_point_map(vpm_out4)
                                        .edge_is_constrained_map(eicm_out4)
                                      )
                                      );
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visit = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                      internal::parse_pmp_np<PolygonMesh>(np1)
                                      // .vertex_point_map(vpm1)
                                      .edge_is_constrained_map(eicm1)
                                      .visitor(visit)
                                      .face_index_map(fim1),
                                      internal::parse_pmp_np<PolygonMesh>(np2)
                                      // .vertex_point_map(vpm2)
                                      .edge_is_constrained_map(eicm2),
                                      std::make_tuple(
                                        internal::parse_pmp_np<PolygonMesh>(np_out[0])
                                        // .vertex_point_map(vpm_out1)
                                        .edge_is_constrained_map(eicm_out1),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[1])
                                        // .vertex_point_map(vpm_out2)
                                        .edge_is_constrained_map(eicm_out2),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[2])
                                        // .vertex_point_map(vpm_out3)
                                        .edge_is_constrained_map(eicm_out3),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[3])
                                        // .vertex_point_map(vpm_out4)
                                        .edge_is_constrained_map(eicm_out4)
                                      )
                                      );
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                    internal::parse_pmp_np<PolygonMesh>(np1)
                                    // .vertex_point_map(vpm1)
                                    .edge_is_constrained_map(eicm1)
                                    .face_index_map(fim1),
                                    internal::parse_pmp_np<PolygonMesh>(np2)
                                    // .vertex_point_map(vpm2)
                                    .edge_is_constrained_map(eicm2),
                                    std::make_tuple(
                                      internal::parse_pmp_np<PolygonMesh>(np_out[0])
                                      // .vertex_point_map(vpm_out1)
                                      .edge_is_constrained_map(eicm_out1),
                                      internal::parse_pmp_np<PolygonMesh>(np_out[1])
                                      // .vertex_point_map(vpm_out2)
                                      .edge_is_constrained_map(eicm_out2),
                                      internal::parse_pmp_np<PolygonMesh>(np_out[2])
                                      // .vertex_point_map(vpm_out3)
                                      .edge_is_constrained_map(eicm_out3),
                                      internal::parse_pmp_np<PolygonMesh>(np_out[3])
                                      // .vertex_point_map(vpm_out4)
                                      .edge_is_constrained_map(eicm_out4)
                                    )
                                    );
    }
  }
  else if (fimb2) {
    if (visitor_flag) {
      try {
        auto visit = py::cast<pmp::Corefine_visitor<Pm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP6",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                      internal::parse_pmp_np<PolygonMesh>(np1)
                                      // .vertex_point_map(vpm1)
                                      .edge_is_constrained_map(eicm1)
                                      .visitor(visit),
                                      internal::parse_pmp_np<PolygonMesh>(np2)
                                      // .vertex_point_map(vpm2)
                                      .edge_is_constrained_map(eicm2)
                                      .face_index_map(fim2),
                                      std::make_tuple(
                                        internal::parse_pmp_np<PolygonMesh>(np_out[0])
                                        // .vertex_point_map(vpm_out1)
                                        .edge_is_constrained_map(eicm_out1),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[1])
                                        // .vertex_point_map(vpm_out2)
                                        .edge_is_constrained_map(eicm_out2),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[2])
                                        // .vertex_point_map(vpm_out3)
                                        .edge_is_constrained_map(eicm_out3),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[3])
                                        // .vertex_point_map(vpm_out4)
                                        .edge_is_constrained_map(eicm_out4)
                                      )
                                      );
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visit = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP6",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                      internal::parse_pmp_np<PolygonMesh>(np1)
                                      // .vertex_point_map(vpm1)
                                      .edge_is_constrained_map(eicm1)
                                      .visitor(visit),
                                      internal::parse_pmp_np<PolygonMesh>(np2)
                                      // .vertex_point_map(vpm2)
                                      .edge_is_constrained_map(eicm2)
                                      .face_index_map(fim2),
                                      std::make_tuple(
                                        internal::parse_pmp_np<PolygonMesh>(np_out[0])
                                        // .vertex_point_map(vpm_out1)
                                        .edge_is_constrained_map(eicm_out1),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[1])
                                        // .vertex_point_map(vpm_out2)
                                        .edge_is_constrained_map(eicm_out2),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[2])
                                        // .vertex_point_map(vpm_out3)
                                        .edge_is_constrained_map(eicm_out3),
                                        internal::parse_pmp_np<PolygonMesh>(np_out[3])
                                        // .vertex_point_map(vpm_out4)
                                        .edge_is_constrained_map(eicm_out4)
                                      )
                                      );
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP6",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                    internal::parse_pmp_np<PolygonMesh>(np1)
                                    // .vertex_point_map(vpm1)
                                    .edge_is_constrained_map(eicm1),
                                    internal::parse_pmp_np<PolygonMesh>(np2)
                                    // .vertex_point_map(vpm2)
                                    .edge_is_constrained_map(eicm2)
                                    .face_index_map(fim2),
                                    std::make_tuple(
                                      internal::parse_pmp_np<PolygonMesh>(np_out[0])
                                      // .vertex_point_map(vpm_out1)
                                      .edge_is_constrained_map(eicm_out1),
                                      internal::parse_pmp_np<PolygonMesh>(np_out[1])
                                      // .vertex_point_map(vpm_out2)
                                      .edge_is_constrained_map(eicm_out2),
                                      internal::parse_pmp_np<PolygonMesh>(np_out[2])
                                      // .vertex_point_map(vpm_out3)
                                      .edge_is_constrained_map(eicm_out3),
                                      internal::parse_pmp_np<PolygonMesh>(np_out[3])
                                      // .vertex_point_map(vpm_out4)
                                      .edge_is_constrained_map(eicm_out4)
                                    )
                                    );
    }
  }
  else {
    res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                  internal::parse_pmp_np<PolygonMesh>(np1)
                                  // .vertex_point_map(vpm1)
                                  .edge_is_constrained_map(eicm1),
                                  internal::parse_pmp_np<PolygonMesh>(np2)
                                  // .vertex_point_map(vpm2)
                                  .edge_is_constrained_map(eicm2),
                                  std::make_tuple(
                                    internal::parse_pmp_np<PolygonMesh>(np_out[0])
                                    // .vertex_point_map(vpm_out1)
                                    .edge_is_constrained_map(eicm_out1),
                                    internal::parse_pmp_np<PolygonMesh>(np_out[1])
                                    // .vertex_point_map(vpm_out2)
                                    .edge_is_constrained_map(eicm_out2),
                                    internal::parse_pmp_np<PolygonMesh>(np_out[2])
                                    // .vertex_point_map(vpm_out3)
                                    .edge_is_constrained_map(eicm_out3),
                                    internal::parse_pmp_np<PolygonMesh>(np_out[3])
                                    // .vertex_point_map(vpm_out4)
                                    .edge_is_constrained_map(eicm_out4)
                                   )
                                   );
  }


  auto retv = std::make_tuple(res[0] ? py::cast(out_union) : py::none(),
                        res[1] ? py::cast(out_intersection) : py::none(),
                        res[2] ? py::cast(tm1_minus_tm2) : py::none(),
                        res[3] ? py::cast(tm2_minus_tm1) : py::none());
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np1.contains("edge_is_constrained_map")) pm1.remove_property_map(eicm1);
  if (!np2.contains("edge_is_constrained_map")) pm2.remove_property_map(eicm2);
  if (!np_out[0].contains("edge_is_constrained_map")) out_union.remove_property_map(eicm_out1);
  if (!np_out[1].contains("edge_is_constrained_map")) out_intersection.remove_property_map(eicm_out2);
  if (!np_out[2].contains("edge_is_constrained_map")) tm1_minus_tm2.remove_property_map(eicm_out3);
  if (!np_out[3].contains("edge_is_constrained_map")) tm2_minus_tm1.remove_property_map(eicm_out4);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
  return retv;
}

//
template <typename TriangleMesh>
TriangleMesh corefine_and_compute_difference(TriangleMesh& pm1, TriangleMesh& pm2,
                                          const py::dict& np1 = py::dict(),
                                          const py::dict& np2 = py::dict(),
                                          const py::dict& np_out = py::dict()) {
  using Tm = TriangleMesh;

  Tm out;
  bool valid;

  // auto vpm1 = get_vertex_point_map(pm1, np1);
  // auto vpm2 = get_vertex_point_map(pm2, np2);
  // auto vpm3 = get_vertex_point_map(out, np_out);
  auto eicm1 = get_edge_prop_map<Tm, bool>(pm1, "INTERNAL_MAP0",
    np1.contains("edge_is_constrained_map") ? np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Tm, bool>(pm2, "INTERNAL_MAP1",
    np2.contains("edge_is_constrained_map") ? np2["edge_internal_map"] : py::none());
  auto eicm_out = get_edge_prop_map<Tm, bool>(out, "INTERNAL_MAP2",
    np_out.contains("edge_is_constrained_map") ? np_out["edge_internal_map"] : py::none());

  bool fimb1 = np1.contains("face_index_map");
  bool fimb2 = np2.contains("face_index_map");
  bool visitor_flag = np1.contains("visitor");
  if (fimb1 && fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                      internal::parse_pmp_np<TriangleMesh>(np1)
                                                      // .vertex_point_map(vpm1)
                                                      .edge_is_constrained_map(eicm1)
                                                      .face_index_map(fim1)
                                                      .visitor(visitor),
                                                      internal::parse_pmp_np<TriangleMesh>(np2)
                                                      // .vertex_point_map(vpm2)
                                                      .face_index_map(fim2)
                                                      .edge_is_constrained_map(eicm2),
                                                      internal::parse_pmp_np<TriangleMesh>(np_out)
                                                      // .vertex_point_map(vpm3)
                                                      .edge_is_constrained_map(eicm_out)
                                                      );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                      internal::parse_pmp_np<TriangleMesh>(np1)
                                                      // .vertex_point_map(vpm1)
                                                      .edge_is_constrained_map(eicm1)
                                                      .face_index_map(fim1)
                                                      .visitor(visitor),
                                                      internal::parse_pmp_np<TriangleMesh>(np2)
                                                      // .vertex_point_map(vpm2)
                                                      .face_index_map(fim2)
                                                      .edge_is_constrained_map(eicm2),
                                                      internal::parse_pmp_np<TriangleMesh>(np_out)
                                                      // .vertex_point_map(vpm3)
                                                      .edge_is_constrained_map(eicm_out)
                                                      );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1)
                                                  .face_index_map(fim1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .face_index_map(fim2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else if (fimb1) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .face_index_map(fim1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .face_index_map(fim1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1)
                                                  .face_index_map(fim1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else if (fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .face_index_map(fim2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .face_index_map(fim2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .face_index_map(fim2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else {
    valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                internal::parse_pmp_np<TriangleMesh>(np1)
                                                // .vertex_point_map(vpm1)
                                                .edge_is_constrained_map(eicm1),
                                                internal::parse_pmp_np<TriangleMesh>(np2)
                                                // .vertex_point_map(vpm2)
                                                .edge_is_constrained_map(eicm2),
                                                internal::parse_pmp_np<TriangleMesh>(np_out)
                                                // .vertex_point_map(vpm3)
                                                .edge_is_constrained_map(eicm_out)
                                                );
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np1.contains("edge_is_constrained_map")) pm1.remove_property_map(eicm1);
  if (!np2.contains("edge_is_constrained_map")) pm2.remove_property_map(eicm2);
  if (!np_out.contains("edge_is_constrained_map")) out.remove_property_map(eicm_out);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
  if (! valid) throw std::runtime_error("Cannot compute difference!");
  return out;
}


//
template <typename TriangleMesh>
TriangleMesh corefine_and_compute_intersection(TriangleMesh& pm1, TriangleMesh& pm2,
                                          const py::dict& np1 = py::dict(),
                                          const py::dict& np2 = py::dict(),
                                          const py::dict& np_out = py::dict()) {
  using Tm = TriangleMesh;

  Tm out;
  bool valid;

  // auto vpm1 = get_vertex_point_map(pm1, np1);
  // auto vpm2 = get_vertex_point_map(pm2, np2);
  // auto vpm3 = get_vertex_point_map(out, np_out);
  auto eicm1 = get_edge_prop_map<Tm, bool>(pm1, "INTERNAL_MAP0",
    np1.contains("edge_is_constrained_map") ? np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Tm, bool>(pm2, "INTERNAL_MAP1",
    np2.contains("edge_is_constrained_map") ? np2["edge_internal_map"] : py::none());
  auto eicm_out = get_edge_prop_map<Tm, bool>(out, "INTERNAL_MAP2",
    np_out.contains("edge_is_constrained_map") ? np_out["edge_internal_map"] : py::none());

  bool fimb1 = np1.contains("face_index_map");
  bool fimb2 = np2.contains("face_index_map");
  bool visitor_flag = np1.contains("visitor");
  if (fimb1 && fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                      internal::parse_pmp_np<TriangleMesh>(np1)
                                                      // .vertex_point_map(vpm1)
                                                      .edge_is_constrained_map(eicm1)
                                                      .face_index_map(fim1)
                                                      .visitor(visitor),
                                                      internal::parse_pmp_np<TriangleMesh>(np2)
                                                      // .vertex_point_map(vpm2)
                                                      .face_index_map(fim2)
                                                      .edge_is_constrained_map(eicm2),
                                                      internal::parse_pmp_np<TriangleMesh>(np_out)
                                                      // .vertex_point_map(vpm3)
                                                      .edge_is_constrained_map(eicm_out)
                                                      );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                      internal::parse_pmp_np<TriangleMesh>(np1)
                                                      // .vertex_point_map(vpm1)
                                                      .edge_is_constrained_map(eicm1)
                                                      .face_index_map(fim1)
                                                      .visitor(visitor),
                                                      internal::parse_pmp_np<TriangleMesh>(np2)
                                                      // .vertex_point_map(vpm2)
                                                      .face_index_map(fim2)
                                                      .edge_is_constrained_map(eicm2),
                                                      internal::parse_pmp_np<TriangleMesh>(np_out)
                                                      // .vertex_point_map(vpm3)
                                                      .edge_is_constrained_map(eicm_out)
                                                      );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1)
                                                  .face_index_map(fim1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .face_index_map(fim2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else if (fimb1) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .face_index_map(fim1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .face_index_map(fim1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1)
                                                  .face_index_map(fim1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else if (fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .face_index_map(fim2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .face_index_map(fim2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .face_index_map(fim2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else {
    valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                internal::parse_pmp_np<TriangleMesh>(np1)
                                                // .vertex_point_map(vpm1)
                                                .edge_is_constrained_map(eicm1),
                                                internal::parse_pmp_np<TriangleMesh>(np2)
                                                // .vertex_point_map(vpm2)
                                                .edge_is_constrained_map(eicm2),
                                                internal::parse_pmp_np<TriangleMesh>(np_out)
                                                // .vertex_point_map(vpm3)
                                                .edge_is_constrained_map(eicm_out)
                                                );
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np1.contains("edge_is_constrained_map")) pm1.remove_property_map(eicm1);
  if (!np2.contains("edge_is_constrained_map")) pm2.remove_property_map(eicm2);
  if (!np_out.contains("edge_is_constrained_map")) out.remove_property_map(eicm_out);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
  if (! valid) throw std::runtime_error("Cannot compute difference!");
  return out;
}

//
template <typename TriangleMesh>
TriangleMesh corefine_and_compute_union(TriangleMesh& pm1, TriangleMesh& pm2,
                                          const py::dict& np1 = py::dict(),
                                          const py::dict& np2 = py::dict(),
                                          const py::dict& np_out = py::dict()) {
  using Tm = TriangleMesh;

  Tm out;
  bool valid;

  // auto vpm1 = get_vertex_point_map(pm1, np1);
  // auto vpm2 = get_vertex_point_map(pm2, np2);
  // auto vpm3 = get_vertex_point_map(out, np_out);
  auto eicm1 = get_edge_prop_map<Tm, bool>(pm1, "INTERNAL_MAP0",
    np1.contains("edge_is_constrained_map") ? np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Tm, bool>(pm2, "INTERNAL_MAP1",
    np2.contains("edge_is_constrained_map") ? np2["edge_internal_map"] : py::none());
  auto eicm_out = get_edge_prop_map<Tm, bool>(out, "INTERNAL_MAP2",
    np_out.contains("edge_is_constrained_map") ? np_out["edge_internal_map"] : py::none());

  bool fimb1 = np1.contains("face_index_map");
  bool fimb2 = np2.contains("face_index_map");
  bool visitor_flag = np1.contains("visitor");
  if (fimb1 && fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                      internal::parse_pmp_np<TriangleMesh>(np1)
                                                      // .vertex_point_map(vpm1)
                                                      .edge_is_constrained_map(eicm1)
                                                      .face_index_map(fim1)
                                                      .visitor(visitor),
                                                      internal::parse_pmp_np<TriangleMesh>(np2)
                                                      // .vertex_point_map(vpm2)
                                                      .face_index_map(fim2)
                                                      .edge_is_constrained_map(eicm2),
                                                      internal::parse_pmp_np<TriangleMesh>(np_out)
                                                      // .vertex_point_map(vpm3)
                                                      .edge_is_constrained_map(eicm_out)
                                                      );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                      internal::parse_pmp_np<TriangleMesh>(np1)
                                                      // .vertex_point_map(vpm1)
                                                      .edge_is_constrained_map(eicm1)
                                                      .face_index_map(fim1)
                                                      .visitor(visitor),
                                                      internal::parse_pmp_np<TriangleMesh>(np2)
                                                      // .vertex_point_map(vpm2)
                                                      .face_index_map(fim2)
                                                      .edge_is_constrained_map(eicm2),
                                                      internal::parse_pmp_np<TriangleMesh>(np_out)
                                                      // .vertex_point_map(vpm3)
                                                      .edge_is_constrained_map(eicm_out)
                                                      );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1)
                                                  .face_index_map(fim1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .face_index_map(fim2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else if (fimb1) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .face_index_map(fim1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .face_index_map(fim1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1)
                                                  .face_index_map(fim1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else if (fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .face_index_map(fim2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .face_index_map(fim2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .face_index_map(fim2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else {
    valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                internal::parse_pmp_np<TriangleMesh>(np1)
                                                // .vertex_point_map(vpm1)
                                                .edge_is_constrained_map(eicm1),
                                                internal::parse_pmp_np<TriangleMesh>(np2)
                                                // .vertex_point_map(vpm2)
                                                .edge_is_constrained_map(eicm2),
                                                internal::parse_pmp_np<TriangleMesh>(np_out)
                                                // .vertex_point_map(vpm3)
                                                .edge_is_constrained_map(eicm_out)
                                                );
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np1.contains("edge_is_constrained_map")) pm1.remove_property_map(eicm1);
  if (!np2.contains("edge_is_constrained_map")) pm2.remove_property_map(eicm2);
  if (!np_out.contains("edge_is_constrained_map")) out.remove_property_map(eicm_out);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
  if (! valid) throw std::runtime_error("Cannot compute difference!");
  return out;
}

template <typename PolygonMesh>
void split(PolygonMesh& pm,
           PolygonMesh& splitter,
           const py::dict& np_tm = py::dict(),
           const py::dict& np_s = py::dict()) {
  // auto vpm1 = get_vertex_point_map(pm, np_tm);
  // auto vpm2 = get_vertex_point_map(splitter, np_s);
  // this can also have 2 visitors
  bool visitor1 = np_tm.contains("visitor");
  bool visitor2 = np_s.contains("visitor");
  if (visitor1 && visitor2) {
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
  } else if (visitor1) {
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_tm["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
  } else if (visitor2) {
    try {
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm)
                 // .vertex_point_map(vpm1)
                 ,
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2)
                 // .vertex_point_map(vpm2)
                 );
    }
    catch (const py::cast_error& e) {
    }
  } else {
    PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm)
               // .vertex_point_map(vpm1)
               ,
               internal::parse_pmp_np<PolygonMesh>(np_s)
               // .vertex_point_map(vpm2)
               );
  }
}

template <typename TriangleMesh>
auto split_c(TriangleMesh& tm,
             const Iso_cuboid_3& bbox,
             const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::split(tm, bbox, internal::parse_pmp_np<TriangleMesh>(np)
                    // .vertex_point_map(vpm)
                    );
}

template <typename TriangleMesh>
auto split_p(TriangleMesh& tm,
             const Plane_3& plane,
             const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::split(tm, plane, internal::parse_pmp_np<TriangleMesh>(np)
                    // .vertex_point_map(vpm)
                    );
}

template <typename PolygonMesh>
auto surface_intersection(const PolygonMesh& tm1,
                          const PolygonMesh& tm2,
                          const py::dict& np1 = py::dict(),
                          const py::dict& np2 = py::dict()) {
  std::vector< std::vector<Point_3> > polylines;
  // auto vpm1 = get_vertex_point_map(tm1, np1);
  // auto vpm2 = get_vertex_point_map(tm2, np2);
  PMP::surface_intersection(tm1, tm2, std::back_inserter(polylines),
                            internal::parse_pmp_np<PolygonMesh>(np1)
                            // .vertex_point_map(vpm1)
                            ,
                            internal::parse_pmp_np<PolygonMesh>(np2)
                            // .vertex_point_map(vpm2)
                            );
  return polylines;
}
using Boolean_operation_type = std::size_t;
using Pm = Polygonal_mesh;
using Gt = boost::graph_traits<Pm>;
using Fd = boost::graph_traits<Pm>::face_descriptor;
using Hd = boost::graph_traits<Pm>::halfedge_descriptor;
using Ed = boost::graph_traits<Pm>::edge_descriptor;
using Vd = boost::graph_traits<Pm>::vertex_descriptor;
using Cv = Corefine_visitor<Pm>;
void set_before_subface_creations_fn(Cv& v,
                                     const std::function<void(Fd, Pm&)>& f) {
  v.set_before_subface_creations(f);
}
void set_after_subface_creations_fn(Cv& v,
                                    const std::function<void(Pm&)>& f) {
  v.set_after_subface_creations(f);
}
void set_before_subface_created_fn(Cv& v,
                                   const std::function<void(Pm&)>& f) {
  v.set_before_subface_created(f);
}
void set_after_subface_created_fn(Cv& v,
                                  const std::function<void(Fd, Pm&)>& f) {
  v.set_after_subface_created(f);
}
void set_before_face_copy_fn(Cv& v,
                             const std::function<void(Fd, const Pm&, Pm&)>& f) {
  v.set_before_face_copy(f);
}
void set_after_face_copy_fn(Cv& v,
                            const std::function<void(Fd, const Pm&, Fd, Pm&)>& f) {
  v.set_after_face_copy(f);
}
void set_before_edge_split_fn(Cv& v,
                              const std::function<void(Hd, Pm&)>& f) {
  v.set_before_edge_split(f);
}
void set_edge_split_fn(Cv& v,
                       const std::function<void(Hd, Pm&)>& f) {
  v.set_edge_split(f);
}
void set_after_edge_split_fn(Cv& v,
                             const std::function<void()>& f) {
  v.set_after_edge_split(f);
}
void set_add_retriangulation_edge_fn(Cv& v,
                                     const std::function<void(Hd, Pm&)>& f) {
  v.set_add_retriangulation_edge(f);
}
void set_before_edge_copy_fn(Cv& v,
                             const std::function<void(Hd, const Pm&, Pm&)>& f) {
  v.set_before_edge_copy(f);
}
void set_after_edge_copy_fn(Cv& v,
                            const std::function<void(Hd, const Pm&, Hd, Pm&)>& f) {
  v.set_after_edge_copy(f);
}
void set_before_edge_duplicated_fn(Cv& v,
                                   const std::function<void(Hd, Pm&)>& f) {
  v.set_before_edge_duplicated(f);
}
void set_after_edge_duplicated_fn(Cv& v,
                                  const std::function<void(Hd, Hd, Pm&)>& f) {
  v.set_after_edge_duplicated(f);
}
void set_intersection_edge_copy_fn(Cv& v,
                                   const std::function<void(Hd, const Pm&, Hd, const Pm&, Hd, Pm&)>& f) {
  v.set_intersection_edge_copy(f);
}
void set_new_vertex_added_fn(Cv& v,
                             const std::function<void(std::size_t, Vd, const Pm&)>& f) {
  v.set_new_vertex_added(f);
}
void set_intersection_point_detected_fn(Cv& v,
                                        const std::function<void(std::size_t, int, Hd, Hd, const Pm&, const Pm&, bool, bool)>& f) {
  v.set_intersection_point_detected(f);
}
void set_before_vertex_copy_fn(Cv& v,
                               const std::function<void(Vd, const Pm&, Pm&)>& f) {
  v.set_before_vertex_copy(f);
}
void set_after_vertex_copy_fn(Cv& v,
                              const std::function<void(Vd, const Pm&, Vd, Pm&)>& f) {
  v.set_after_vertex_copy(f);
}
void set_start_filtering_intersections_fn(Cv& v,
                                          const std::function<void()>& f) {
  v.set_start_filtering_intersections(f);
}
void set_progress_filtering_intersections_fn(Cv& v,
                                             const std::function<void(double)>& f) {
  v.set_progress_filtering_intersections(f);
}
void set_end_filtering_intersections_fn(Cv& v,
                                        const std::function<void()>& f) {
  v.set_end_filtering_intersections(f);
}
void set_start_triangulating_faces_fn(Cv& v,
                                      const std::function<void(std::size_t)>& f) {
  v.set_start_triangulating_faces(f);
}
void set_triangulating_faces_step_fn(Cv& v,
                                     const std::function<void()>& f) {
  v.set_triangulating_faces_step(f);
}
void set_end_triangulating_faces_fn(Cv& v,
                                    const std::function<void()>& f) {
  v.set_end_triangulating_faces(f);
}
void set_start_handling_intersection_of_coplanar_faces_fn(Cv& v,
                                                          const std::function<void(std::size_t)>& f) {
  v.set_start_handling_intersection_of_coplanar_faces(f);
}
void set_intersection_of_coplanar_faces_step_fn(Cv& v,
                                                const std::function<void()>& f) {
  v.set_intersection_of_coplanar_faces_step(f);
}
void set_end_handling_intersection_of_coplanar_faces_fn(Cv& v,
                                                        const std::function<void()>& f) {
  v.set_end_handling_intersection_of_coplanar_faces(f);
}
void set_start_handling_edge_face_intersections_fn(Cv& v,
                                                   const std::function<void(std::size_t)>& f) {
  v.set_start_handling_edge_face_intersections(f);
}
void set_edge_face_intersections_step_fn(Cv& v,
                                         const std::function<void()>& f) {
  v.set_edge_face_intersections_step(f);
}
void set_end_handling_edge_face_intersections_fn(Cv& v,
                                                const std::function<void()>& f) {
  v.set_end_handling_edge_face_intersections(f);
}
void set_start_building_output_fn(Cv& v,
                                  const std::function<void()>& f) {
  v.set_start_building_output(f);
}
void set_end_building_output_fn(Cv& v,
                                const std::function<void()>& f) {
  v.set_end_building_output(f);
}
void set_filter_coplanar_edges_fn(Cv& v,
                                  const std::function<void()>& f) {
  v.set_filter_coplanar_edges(f);
}
void set_detect_patches_fn(Cv& v,
                           const std::function<void()>& f) {
  v.set_detect_patches(f);
}
void set_classify_patches_fn(Cv& v,
                             const std::function<void()>& f) {
  v.set_classify_patches(f);
}
void set_classify_intersection_free_patches_fn(Cv& v,
                                               const std::function<void(const Pm&)>& f) {
  v.set_classify_intersection_free_patches(f);
}
void set_out_of_place_operation_fn(Cv& v,
                                   const std::function<void(Boolean_operation_type)>& f) {
  v.set_out_of_place_operation(f);
}
void set_in_place_operation_fn(Cv& v,
                               const std::function<void(Boolean_operation_type)>& f) {
  v.set_in_place_operation(f);
}
void set_in_place_operations_fn(Cv& v,
                               const std::function<void(Boolean_operation_type, Boolean_operation_type)>& f) {
  v.set_in_place_operations(f);
}


} // namespace pmp

void export_corefinement(py::module_& m) {

  using Pm = pmp::Polygonal_mesh;

  // Corefinement and Boolean Operations
  m.def("autorefine", &pmp::autorefine<Pm>,
        py::arg("tm"), py::arg("np") = py::dict());
  m.def("autorefine_triangle_soup", &pmp::autorefine_triangle_soup,
        py::arg("soup_points"), py::arg("soup_triangles"), py::arg("np") = py::dict());
  m.def("clip", &pmp::clip_c<Pm>,
        py::arg("tm"), py::arg("iso_cuboid"),
        py::arg("np") = py::dict());
  m.def("clip", &pmp::clip_p<Pm>,
        py::arg("tm"), py::arg("plane"),
        py::arg("np") = py::dict());
  m.def("clip", &pmp::clip<Pm>,
        py::arg("tm"), py::arg("clipper"),
        py::arg("np_tm") = py::dict(), py::arg("np_c") = py::dict());
  m.def("corefine", &pmp::corefine<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());
  m.def("corefine_and_compute_boolean_operations", &pmp::corefine_and_compute_boolean_operations<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        py::arg("np_out") = py::tuple());
  m.def("corefine_and_compute_difference", &pmp::corefine_and_compute_difference<Pm>,
        py::arg("tm1"), py::arg("tm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(), py::arg("np_out") = py::dict());
  m.def("corefine_and_compute_intersection", &pmp::corefine_and_compute_intersection<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        py::arg("np_out") = py::dict());
  m.def("corefine_and_compute_union", &pmp::corefine_and_compute_union<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        py::arg("np_out") = py::dict());
  m.def("split", &pmp::split_c<Pm>,
        py::arg("tm"), py::arg("iso_cuboid"),
        py::arg("np") = py::dict());
  m.def("split", &pmp::split_p<Pm>,
        py::arg("tm"), py::arg("plane"),
        py::arg("np") = py::dict());
  m.def("split", &pmp::split<Pm>,
        py::arg("tm"), py::arg("splitter"),
        py::arg("np_tm") = py::dict(), py::arg("np_s") = py::dict());
  m.def("surface_intersection", &pmp::surface_intersection<Pm>,
        py::arg("tm1"), py::arg("tm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());


  // Corefine_visitor
  m.def("set_before_subface_creations", &pmp::set_before_subface_creations_fn);
  m.def("set_after_subface_creations", &pmp::set_after_subface_creations_fn);
  m.def("set_before_subface_created", &pmp::set_before_subface_created_fn);
  m.def("set_after_subface_created", &pmp::set_after_subface_created_fn);
  m.def("set_before_face_copy", &pmp::set_before_face_copy_fn);
  m.def("set_after_face_copy", &pmp::set_after_face_copy_fn);
  m.def("set_before_edge_split", &pmp::set_before_edge_split_fn);
  m.def("set_edge_split", &pmp::set_edge_split_fn);
  m.def("set_after_edge_split", &pmp::set_after_edge_split_fn);
  m.def("set_add_retriangulation_edge", &pmp::set_add_retriangulation_edge_fn);
  m.def("set_before_edge_copy", &pmp::set_before_edge_copy_fn);
  m.def("set_after_edge_copy", &pmp::set_after_edge_copy_fn);
  m.def("set_before_edge_duplicated", &pmp::set_before_edge_duplicated_fn);
  m.def("set_after_edge_duplicated", &pmp::set_after_edge_duplicated_fn);
  m.def("set_intersection_edge_copy", &pmp::set_intersection_edge_copy_fn);
  m.def("set_new_vertex_added", &pmp::set_new_vertex_added_fn);
  m.def("set_intersection_point_detected", &pmp::set_intersection_point_detected_fn);
  m.def("set_before_vertex_copy", &pmp::set_before_vertex_copy_fn);
  m.def("set_after_vertex_copy", &pmp::set_after_vertex_copy_fn);
  m.def("set_start_filtering_intersections", &pmp::set_start_filtering_intersections_fn);
  m.def("set_progress_filtering_intersections", &pmp::set_progress_filtering_intersections_fn);
  m.def("set_end_filtering_intersections", &pmp::set_end_filtering_intersections_fn);
  m.def("set_start_triangulating_faces", &pmp::set_start_triangulating_faces_fn);
  m.def("set_triangulating_faces_step", &pmp::set_triangulating_faces_step_fn);
  m.def("set_end_triangulating_faces", &pmp::set_end_triangulating_faces_fn);
  m.def("set_start_handling_intersection_of_coplanar_faces", &pmp::set_start_handling_intersection_of_coplanar_faces_fn);
  m.def("set_intersection_of_coplanar_faces_step", &pmp::set_intersection_of_coplanar_faces_step_fn);
  m.def("set_end_handling_intersection_of_coplanar_faces", &pmp::set_end_handling_intersection_of_coplanar_faces_fn);
  m.def("set_start_handling_edge_face_intersections", &pmp::set_start_handling_edge_face_intersections_fn);
  m.def("set_edge_face_intersections_step", &pmp::set_edge_face_intersections_step_fn);
  m.def("set_end_handling_edge_face_intersections", &pmp::set_end_handling_edge_face_intersections_fn);
  m.def("set_start_building_output", &pmp::set_start_building_output_fn);
  m.def("set_end_building_output", &pmp::set_end_building_output_fn);
  m.def("set_filter_coplanar_edges", &pmp::set_filter_coplanar_edges_fn);
  m.def("set_detect_patches", &pmp::set_detect_patches_fn);
  m.def("set_classify_patches", &pmp::set_classify_patches_fn);
  m.def("set_classify_intersection_free_patches", &pmp::set_classify_intersection_free_patches_fn);
  m.def("set_out_of_place_operation", &pmp::set_out_of_place_operation_fn);
  m.def("set_in_place_operation", &pmp::set_in_place_operation_fn);
  m.def("set_in_place_operations", &pmp::set_in_place_operations_fn);



  using Av = pmp::Autorefinement_visitor;
  py::class_<Av>(m, "Autorefinement_visitor")
    .def(py::init<>())
    .def("set_number_of_output_triangles", &Av::set_number_of_output_triangles)
    .def("set_verbatim_triangle_copy", &Av::set_verbatim_triangle_copy)
    .def("set_new_subtriangle", &Av::set_new_subtriangle)
    ;

  // default visitor
  using Dv = pmp::Default_visitor<Pm>;
  py::class_<Dv>(m, "Default_visitor")
    .def(py::init<>())
    ;

  // corefine
  using Cv = pmp::Corefine_visitor<Pm>;
  py::class_<Cv>(m, "Corefine_visitor")
    .def(py::init<>())
    ;

  // non-manifold
  using Nmv = pmp::Non_manifold_output_visitor<Pm>;
  py::class_<Nmv>(m, "Non_manifold_output_visitor")
    // constructor with 2 PolygonMesh arguments
    .def(py::init<Pm&, Pm&>())
    // visitor.extract_intersection(points, polygons);
    .def("extract_intersection", &Nmv::my_extract_intersection)
    ;
}

