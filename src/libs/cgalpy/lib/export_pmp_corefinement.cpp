// Copyright (c) 2024 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com
//            Efi Fogel          <efifogel@gmail.com>

#include <array>
#include <functional>
#include <tuple>
#include <vector>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/stl/vector.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/tuple.h>

#include <CGAL/Polygon_mesh_processing/autorefinement.h>
#include <CGAL/Polygon_mesh_processing/clip.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>

// The local CGAL intersection_polylines.h header also declares the deprecated
// surface_intersection() overload with default arguments. Homebrew CGAL's
// intersection.h already declares the same defaults, so including both causes
// a redefinition-of-default-argument error. We only need intersection_polylines(),
// which is outside the deprecated-code guard.
#ifndef CGAL_NO_DEPRECATED_CODE
#define CGALPY_RESTORE_CGAL_NO_DEPRECATED_CODE
#define CGAL_NO_DEPRECATED_CODE
#endif
#include <CGAL/Polygon_mesh_processing/intersection_polylines.h>
#ifdef CGALPY_RESTORE_CGAL_NO_DEPRECATED_CODE
#undef CGAL_NO_DEPRECATED_CODE
#undef CGALPY_RESTORE_CGAL_NO_DEPRECATED_CODE
#endif

#include "cgalpy/Autorefinement_visitor.hpp"
#include "cgalpy/Corefine_visitor.hpp"
#include "cgalpy/Default_visitor.hpp"
#include "cgalpy/Non_manifold_output_visitor.hpp"
#include "cgalpy/pmp_helpers.hpp"
#include "cgalpy/polygon_mesh_processing_types.hpp"

namespace py = nanobind;

namespace cgalpy {
namespace pmp {

//!
template <typename PolygonMesh>
auto autorefine(PolygonMesh& tm, const py::dict& np = py::dict())
{ PMP::autorefine(tm); }

//!
auto autorefine_triangle_soup(std::vector<Point_3>& points, std::vector<std::vector<std::size_t>>& polygons,
                              const py::dict& np = py::dict()) {
  bool visitor = np.contains("visitor");
  if (visitor) {
    try {
      auto v = py::cast<pmp::Autorefinement_visitor>(np["visitor"]);
      PMP::autorefine_triangle_soup(points, polygons);
    }
    catch (const py::cast_error& e) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    PMP::autorefine_triangle_soup(points, polygons);
  }

  return std::make_tuple(points, polygons);
}

//!
template <typename PolygonMesh>
bool clip(PolygonMesh& tm, PolygonMesh& clipper, const py::dict& np_tm = py::dict(),
          const py::dict& np_c = py::dict()) {
  using Pm = PolygonMesh;

  auto eicm1 = get_edge_prop_map<Pm, bool>(tm, "INTERNAL_MAP0",
                                           np_tm.contains("edge_is_constrained_map") ?
                                           np_tm["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Pm, bool>(clipper, "INTERNAL_MAP1",
                                           np_c.contains("edge_is_constrained_map") ?
                                           np_c["edge_internal_map"] : py::none());

  bool visitor1 = np_tm.contains("visitor");
  bool visitor2 = np_c.contains("visitor");
  bool res;
  if (visitor1 && visitor2) {
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper);
    }
    catch (const py::cast_error& e) {
      std::cerr << "Visitor type not recognized\n";
    }
  }
  else if (visitor1) {
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np_tm["visitor"]);
      res = PMP::clip(tm, clipper);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_tm["visitor"]);
      res = PMP::clip(tm, clipper);
    }
    catch (const py::cast_error& e) {
      std::cerr << "Visitor type not recognized\n";
    }
  }
  else if (visitor2) {
    try {
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper);
    }
    catch (const py::cast_error& e) {
      std::cerr << "Visitor type not recognized\n";
    }
  }
  else {
    res = PMP::clip(tm, clipper);
  }
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np_tm.contains("edge_is_constrained_map")) {
    tm.remove_property_map(eicm1);
  }
  if (! np_c.contains("edge_is_constrained_map")) {
    clipper.remove_property_map(eicm2);
  }
#endif
  return res;
}

//!
template <typename TriangleMesh>
auto clip_c(TriangleMesh& tm, const Iso_cuboid_3& box, const py::dict& np = py::dict()) {
  using Pm = TriangleMesh;
  bool visitor = np.contains("visitor");
  if (visitor) {
    try {
      auto v = py::cast<pmp::Corefine_visitor<Pm>>(np["visitor"]);
      return PMP::clip(tm, box);
    }
    catch (const py::cast_error&) {
    }
    try {
      auto v = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np["visitor"]);
      return PMP::clip(tm, box);
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    return PMP::clip(tm, box);
  }
}

//!
template <typename TriangleMesh>
auto clip_p(TriangleMesh& tm, const Plane_3& plane, const py::dict& np = py::dict()) {
  using Pm = TriangleMesh;
  bool visitor = np.contains("visitor");
  if (visitor) {
    try {
      auto v = py::cast<pmp::Corefine_visitor<Pm>>(np["visitor"]);
      return PMP::clip(tm, plane);
    }
    catch (const py::cast_error&) {
    }
    try {
      auto v = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np["visitor"]);
      return PMP::clip(tm, plane);
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    return PMP::clip(tm, plane);
  }
}

//!
template <typename PolygonMesh>
void corefine(PolygonMesh& tm1, PolygonMesh& tm2, const py::dict& np1 = py::dict(), const py::dict& np2 = py::dict()) {
  auto eicm1 = get_edge_prop_map<PolygonMesh, bool>(tm1, "INTERNAL_MAP0",
                                                    np1.contains("edge_is_constrained_map") ?
                                                    np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<PolygonMesh, bool>(tm2, "INTERNAL_MAP1",
                                                    np2.contains("edge_is_constrained_map") ?
                                                    np2["edge_internal_map"] : py::none());
  // np1 can have a corefinement visitor
  bool visitor = np1.contains("visitor");
  if (visitor) {
    // try to cast to Non_manifold_output_visitor or Default_visitor
    try {
      auto visitor = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np1["visitor"]);
      PMP::corefine(tm1, tm2);
    }
    catch (const py::cast_error&) {
    }
    try {
      auto visitor = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np1["visitor"]);
      PMP::corefine(tm1, tm2);
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    PMP::corefine(tm1, tm2);
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np1.contains("edge_is_constrained_map")) tm1.remove_property_map(eicm1);
  if (! np2.contains("edge_is_constrained_map")) tm2.remove_property_map(eicm2);
#endif
}

//!
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
                                           np1.contains("edge_is_constrained_map") ?
                                           np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Pm, bool>(pm2, "INTERNAL_MAP1",
                                           np2.contains("edge_is_constrained_map") ?
                                           np2["edge_internal_map"] : py::none());

  // auto vpm_out1 = get_vertex_point_map(pm_out1, np_out[0]);
  // auto vpm_out2 = get_vertex_point_map(pm_out2, np_out[1]);
  // auto vpm_out3 = get_vertex_point_map(pm_out3, np_out[2]);
  // auto vpm_out4 = get_vertex_point_map(pm_out4, np_out[3]);
  auto eicm_out1 = get_edge_prop_map<Pm, bool>(pm1, "INTERNAL_MAP2",
                                               np_out[0].contains("edge_is_constrained_map") ?
                                               np_out[0]["edge_internal_map"] : py::none());
  auto eicm_out2 = get_edge_prop_map<Pm, bool>(pm2, "INTERNAL_MAP3",
                                               np_out[1].contains("edge_is_constrained_map") ?
                                               np_out[1]["edge_internal_map"] : py::none());
  auto eicm_out3 = get_edge_prop_map<Pm, bool>(pm1, "INTERNAL_MAP4",
                                               np_out[2].contains("edge_is_constrained_map") ?
                                               np_out[2]["edge_internal_map"] : py::none());
  auto eicm_out4 = get_edge_prop_map<Pm, bool>(pm2, "INTERNAL_MAP5",
                                               np_out[3].contains("edge_is_constrained_map") ?
                                               np_out[3]["edge_internal_map"] : py::none());

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
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP7",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs);
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visit = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP7",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs);
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
                                                     np1.contains("face_index_map") ?
                                                     np1["face_internal_map"] : py::none());
      auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP7",
                                                     np2.contains("face_index_map") ?
                                                     np2["face_internal_map"] : py::none());
      res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs);
    }
  }
  else if (fimb1) {
    if (visitor_flag) {
      try {
        auto visit = py::cast<pmp::Corefine_visitor<Pm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs);
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visit = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs);
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
                                                     np1.contains("face_index_map") ?
                                                     np1["face_internal_map"] : py::none());
      res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs);
    }
  }
  else if (fimb2) {
    if (visitor_flag) {
      try {
        auto visit = py::cast<pmp::Corefine_visitor<Pm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP6",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs);
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visit = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP6",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs);
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP6",
                                                     np2.contains("face_index_map") ?
                                                     np2["face_internal_map"] : py::none());
      res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs);
    }
  }
  else {
    res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs);
  }

  auto retv = std::make_tuple(res[0] ? py::cast(out_union) : py::none(),
                              res[1] ? py::cast(out_intersection) : py::none(),
                              res[2] ? py::cast(tm1_minus_tm2) : py::none(),
                              res[3] ? py::cast(tm2_minus_tm1) : py::none());
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (!np1.contains("edge_is_constrained_map")) pm1.remove_property_map(eicm1);
  if (!np2.contains("edge_is_constrained_map")) pm2.remove_property_map(eicm2);
  if (!np_out[0].contains("edge_is_constrained_map")) out_union.remove_property_map(eicm_out1);
  if (!np_out[1].contains("edge_is_constrained_map")) out_intersection.remove_property_map(eicm_out2);
  if (!np_out[2].contains("edge_is_constrained_map")) tm1_minus_tm2.remove_property_map(eicm_out3);
  if (!np_out[3].contains("edge_is_constrained_map")) tm2_minus_tm1.remove_property_map(eicm_out4);
#endif
  return retv;
}

//!
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
                                           np1.contains("edge_is_constrained_map") ?
                                           np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Tm, bool>(pm2, "INTERNAL_MAP1",
                                           np2.contains("edge_is_constrained_map") ?
                                           np2["edge_internal_map"] : py::none());
  auto eicm_out = get_edge_prop_map<Tm, bool>(out, "INTERNAL_MAP2",
                                              np_out.contains("edge_is_constrained_map") ?
                                              np_out["edge_internal_map"] : py::none());

  bool fimb1 = np1.contains("face_index_map");
  bool fimb2 = np2.contains("face_index_map");
  bool visitor_flag = np1.contains("visitor");
  if (fimb1 && fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_difference(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_difference(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                     np1.contains("face_index_map") ?
                                                     np1["face_internal_map"] : py::none());
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
                                                     np2.contains("face_index_map") ?
                                                     np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_difference(pm1, pm2, out);
    }
  }
  else if (fimb1) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_difference(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_difference(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                     np1.contains("face_index_map") ?
                                                     np1["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_difference(pm1, pm2, out);
    }
  }
  else if (fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_difference(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_difference(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
                                                     np2.contains("face_index_map") ?
                                                     np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_difference(pm1, pm2, out);
    }
  }
  else {
    valid = PMP::corefine_and_compute_difference(pm1, pm2, out);
  }
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np1.contains("edge_is_constrained_map")) pm1.remove_property_map(eicm1);
  if (! np2.contains("edge_is_constrained_map")) pm2.remove_property_map(eicm2);
  if (! np_out.contains("edge_is_constrained_map")) out.remove_property_map(eicm_out);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
  if (! valid) throw std::runtime_error("Cannot compute difference!");
  return out;
}

//!
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
                                           np1.contains("edge_is_constrained_map") ?
                                           np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Tm, bool>(pm2, "INTERNAL_MAP1",
                                           np2.contains("edge_is_constrained_map") ?
                                           np2["edge_internal_map"] : py::none());
  auto eicm_out = get_edge_prop_map<Tm, bool>(out, "INTERNAL_MAP2",
                                              np_out.contains("edge_is_constrained_map") ?
                                              np_out["edge_internal_map"] : py::none());

  bool fimb1 = np1.contains("face_index_map");
  bool fimb2 = np2.contains("face_index_map");
  bool visitor_flag = np1.contains("visitor");
  if (fimb1 && fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_intersection(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_intersection(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                     np1.contains("face_index_map") ?
                                                     np1["face_internal_map"] : py::none());
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
                                                     np2.contains("face_index_map") ?
                                                     np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_intersection(pm1, pm2, out);
    }
  }
  else if (fimb1) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_intersection(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_intersection(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                     np1.contains("face_index_map") ?
                                                     np1["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_intersection(pm1, pm2, out);
    }
  }
  else if (fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_intersection(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_intersection(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
                                                     np2.contains("face_index_map") ?
                                                     np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_intersection(pm1, pm2, out);
    }
  }
  else {
    valid = PMP::corefine_and_compute_intersection(pm1, pm2, out);
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (! np1.contains("edge_is_constrained_map")) pm1.remove_property_map(eicm1);
  if (! np2.contains("edge_is_constrained_map")) pm2.remove_property_map(eicm2);
  if (! np_out.contains("edge_is_constrained_map")) out.remove_property_map(eicm_out);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
  if (! valid) throw std::runtime_error("Cannot compute difference!");
  return out;
}

//!
template <typename TriangleMesh>
TriangleMesh corefine_and_compute_union(TriangleMesh& pm1, TriangleMesh& pm2,
                                        const py::dict& np1 = py::dict(),
                                        const py::dict& np2 = py::dict(),
                                        const py::dict& np_out = py::dict()) {
  using Tm = TriangleMesh;

  Tm out;
  bool valid;

  auto eicm1 = get_edge_prop_map<Tm, bool>(pm1, "INTERNAL_MAP0",
                                           np1.contains("edge_is_constrained_map") ?
                                           np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Tm, bool>(pm2, "INTERNAL_MAP1",
                                           np2.contains("edge_is_constrained_map") ?
                                           np2["edge_internal_map"] : py::none());
  auto eicm_out = get_edge_prop_map<Tm, bool>(out, "INTERNAL_MAP2",
                                              np_out.contains("edge_is_constrained_map") ?
                                              np_out["edge_internal_map"] : py::none());

  bool fimb1 = np1.contains("face_index_map");
  bool fimb2 = np2.contains("face_index_map");
  bool visitor_flag = np1.contains("visitor");
  if (fimb1 && fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_union(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_union(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                     np1.contains("face_index_map") ?
                                                     np1["face_internal_map"] : py::none());
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
                                                     np2.contains("face_index_map") ?
                                                     np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_union(pm1, pm2, out);
    }
  }
  else if (fimb1) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_union(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
                                                       np1.contains("face_index_map") ?
                                                       np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_union(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_union(pm1, pm2, out);
    }
  }
  else if (fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_union(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
                                                       np2.contains("face_index_map") ?
                                                       np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_union(pm1, pm2, out);
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_union(pm1, pm2, out);
    }
  }
  else {
    valid = PMP::corefine_and_compute_union(pm1, pm2, out);
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np1.contains("edge_is_constrained_map")) pm1.remove_property_map(eicm1);
  if (! np2.contains("edge_is_constrained_map")) pm2.remove_property_map(eicm2);
  if (! np_out.contains("edge_is_constrained_map")) out.remove_property_map(eicm_out);
#endif

  if (! valid) throw std::runtime_error("Cannot compute union!");

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
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
  } else if (visitor1) {
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_tm["visitor"]);
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
  } else if (visitor2) {
    try {
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter);
    }
    catch (const py::cast_error& e) {
    }
  } else {
    PMP::split(pm, splitter);
  }
}

template <typename TriangleMesh>
auto split_c(TriangleMesh& tm, const Iso_cuboid_3& bbox, const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::split(tm, bbox);
}

template <typename TriangleMesh>
auto split_p(TriangleMesh& tm,
             const Plane_3& plane,
             const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::split(tm, plane);
}

template <typename PolygonMesh>
auto intersection_polylines(const PolygonMesh& tm1, const PolygonMesh& tm2,
                            const py::dict& np1 = py::dict(), const py::dict& np2 = py::dict()) {
  std::vector< std::vector<Point_3> > polylines;
  // auto vpm1 = get_vertex_point_map(tm1, np1);
  // auto vpm2 = get_vertex_point_map(tm2, np2);
  PMP::intersection_polylines(tm1, tm2, std::back_inserter(polylines));
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

//!
void set_before_subface_creations_fn(Cv& v, const std::function<void(Fd, Pm&)>& f) { v.set_before_subface_creations(f); }

//!
void set_after_subface_creations_fn(Cv& v, const std::function<void(Pm&)>& f) { v.set_after_subface_creations(f); }

//!
void set_before_subface_created_fn(Cv& v, const std::function<void(Pm&)>& f) { v.set_before_subface_created(f); }

//!
void set_after_subface_created_fn(Cv& v, const std::function<void(Fd, Pm&)>& f) { v.set_after_subface_created(f); }

//!
void set_before_face_copy_fn(Cv& v, const std::function<void(Fd, const Pm&, Pm&)>& f) { v.set_before_face_copy(f); }

//!
void set_after_face_copy_fn(Cv& v, const std::function<void(Fd, const Pm&, Fd, Pm&)>& f) { v.set_after_face_copy(f); }

//!
void set_before_edge_split_fn(Cv& v, const std::function<void(Hd, Pm&)>& f) { v.set_before_edge_split(f); }

//!
void set_edge_split_fn(Cv& v, const std::function<void(Hd, Pm&)>& f) { v.set_edge_split(f); }

//!
void set_after_edge_split_fn(Cv& v, const std::function<void()>& f) { v.set_after_edge_split(f); }

//!
void set_add_retriangulation_edge_fn(Cv& v, const std::function<void(Hd, Pm&)>& f) { v.set_add_retriangulation_edge(f); }

//!
void set_before_edge_copy_fn(Cv& v, const std::function<void(Hd, const Pm&, Pm&)>& f) { v.set_before_edge_copy(f); }

//!
void set_after_edge_copy_fn(Cv& v, const std::function<void(Hd, const Pm&, Hd, Pm&)>& f) { v.set_after_edge_copy(f); }

//!
void set_before_edge_duplicated_fn(Cv& v, const std::function<void(Hd, Pm&)>& f) { v.set_before_edge_duplicated(f); }

//!
void set_after_edge_duplicated_fn(Cv& v, const std::function<void(Hd, Hd, Pm&)>& f) { v.set_after_edge_duplicated(f); }

//!
void set_intersection_edge_copy_fn(Cv& v, const std::function<void(Hd, const Pm&, Hd, const Pm&, Hd, Pm&)>& f)
{ v.set_intersection_edge_copy(f); }

//!
void set_new_vertex_added_fn(Cv& v, const std::function<void(std::size_t, Vd, const Pm&)>& f)
{ v.set_new_vertex_added(f); }

//!
void set_intersection_point_detected_fn(Cv& v,
                                        const std::function<void(std::size_t, int, Hd, Hd, const Pm&, const Pm&, bool, bool)>& f)
{ v.set_intersection_point_detected(f); }

//!
void set_before_vertex_copy_fn(Cv& v, const std::function<void(Vd, const Pm&, Pm&)>& f) { v.set_before_vertex_copy(f); }

//!
void set_after_vertex_copy_fn(Cv& v, const std::function<void(Vd, const Pm&, Vd, Pm&)>& f)
{ v.set_after_vertex_copy(f); }

//!
void set_start_filtering_intersections_fn(Cv& v, const std::function<void()>& f)
{ v.set_start_filtering_intersections(f); }

//!
void set_progress_filtering_intersections_fn(Cv& v, const std::function<void(double)>& f)
{ v.set_progress_filtering_intersections(f); }

//!
void set_end_filtering_intersections_fn(Cv& v, const std::function<void()>& f)
{ v.set_end_filtering_intersections(f); }

//!
void set_start_triangulating_faces_fn(Cv& v, const std::function<void(std::size_t)>& f)
{ v.set_start_triangulating_faces(f); }

//!
void set_triangulating_faces_step_fn(Cv& v, const std::function<void()>& f) { v.set_triangulating_faces_step(f); }

//!
void set_end_triangulating_faces_fn(Cv& v, const std::function<void()>& f) { v.set_end_triangulating_faces(f); }

//!
void set_start_handling_intersection_of_coplanar_faces_fn(Cv& v, const std::function<void(std::size_t)>& f)
{ v.set_start_handling_intersection_of_coplanar_faces(f); }

//!
void set_intersection_of_coplanar_faces_step_fn(Cv& v, const std::function<void()>& f)
{ v.set_intersection_of_coplanar_faces_step(f); }

//!
void set_end_handling_intersection_of_coplanar_faces_fn(Cv& v, const std::function<void()>& f)
{ v.set_end_handling_intersection_of_coplanar_faces(f); }

//!
void set_start_handling_edge_face_intersections_fn(Cv& v, const std::function<void(std::size_t)>& f)
{ v.set_start_handling_edge_face_intersections(f); }

//!
void set_edge_face_intersections_step_fn(Cv& v, const std::function<void()>& f)
{ v.set_edge_face_intersections_step(f); }

//!
void set_end_handling_edge_face_intersections_fn(Cv& v, const std::function<void()>& f)
{ v.set_end_handling_edge_face_intersections(f); }

//!
void set_start_building_output_fn(Cv& v, const std::function<void()>& f) { v.set_start_building_output(f); }

//!
void set_end_building_output_fn(Cv& v, const std::function<void()>& f) { v.set_end_building_output(f); }

//!
void set_filter_coplanar_edges_fn(Cv& v, const std::function<void()>& f) { v.set_filter_coplanar_edges(f); }

//!
void set_detect_patches_fn(Cv& v, const std::function<void()>& f) { v.set_detect_patches(f); }

//!
void set_classify_patches_fn(Cv& v, const std::function<void()>& f) { v.set_classify_patches(f); }

//!
void set_classify_intersection_free_patches_fn(Cv& v, const std::function<void(const Pm&)>& f)
{ v.set_classify_intersection_free_patches(f); }

//!
void set_out_of_place_operation_fn(Cv& v, const std::function<void(Boolean_operation_type)>& f)
{ v.set_out_of_place_operation(f); }

//!
void set_in_place_operation_fn(Cv& v, const std::function<void(Boolean_operation_type)>& f)
{ v.set_in_place_operation(f); }

//!
void set_in_place_operations_fn(Cv& v, const std::function<void(Boolean_operation_type, Boolean_operation_type)>& f)
{ v.set_in_place_operations(f); }

}
} // namespace cgalpy // namespace pmp

//!
void export_pmp_corefinement(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;

  // Corefinement and Boolean Operations
  m.def("autorefine", &cgalpy::pmp::autorefine<Pm>,
        py::arg("tm"), py::arg("np") = py::dict(),
        "Autorefines a triangle mesh.");
  m.def("autorefine_triangle_soup", &cgalpy::pmp::autorefine_triangle_soup,
        py::arg("soup_points"), py::arg("soup_triangles"), py::arg("np") = py::dict(),
        "Autorefines a triangle soup.");
  m.def("clip", &cgalpy::pmp::clip_c<Pm>,
        py::arg("tm"), py::arg("iso_cuboid"), py::arg("np") = py::dict(),
        "Clips a triangle mesh by an iso-cuboid.");
#if CGAL_VERSION_NR > 1060100900
  m.def("clip", &cgalpy::pmp::clip_p<Pm>,
        py::arg("tm"), py::arg("plane"), py::arg("np") = py::dict(),
        "Clips a triangle mesh by a plane.");
#endif
  m.def("clip", &cgalpy::pmp::clip<Pm>,
        py::arg("tm"), py::arg("clipper"), py::arg("np_tm") = py::dict(), py::arg("np_c") = py::dict(),
        "Clips a triangle mesh by another triangle mesh.");
  m.def("corefine", &cgalpy::pmp::corefine<Pm>,
        py::arg("pm1"), py::arg("pm2"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        "Corefines two polygon meshes.");
  m.def("corefine_and_compute_boolean_operations", &cgalpy::pmp::corefine_and_compute_boolean_operations<Pm>,
        py::arg("pm1"), py::arg("pm2"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        py::arg("np_out") = py::tuple(),
        "Corefines two polygon meshes and computes Boolean operation outputs.");
  m.def("corefine_and_compute_difference", &cgalpy::pmp::corefine_and_compute_difference<Pm>,
        py::arg("tm1"), py::arg("tm2"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        py::arg("np_out") = py::dict(),
        "Corefines two polygon meshes and computes their difference.");
  m.def("corefine_and_compute_intersection", &cgalpy::pmp::corefine_and_compute_intersection<Pm>,
        py::arg("pm1"), py::arg("pm2"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        py::arg("np_out") = py::dict(),
        "Corefines two polygon meshes and computes their intersection.");
  m.def("corefine_and_compute_union", &cgalpy::pmp::corefine_and_compute_union<Pm>,
        py::arg("pm1"), py::arg("pm2"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        py::arg("np_out") = py::dict(),
        "Corefines two polygon meshes and computes their union.");
  m.def("split", &cgalpy::pmp::split_c<Pm>,
        py::arg("tm"), py::arg("iso_cuboid"), py::arg("np") = py::dict(),
        "Splits a triangle mesh by an iso-cuboid.");
  m.def("split", &cgalpy::pmp::split_p<Pm>,
        py::arg("tm"), py::arg("plane"), py::arg("np") = py::dict(),
        "Splits a triangle mesh by a plane.");
  m.def("split", &cgalpy::pmp::split<Pm>,
        py::arg("tm"), py::arg("splitter"), py::arg("np_tm") = py::dict(), py::arg("np_s") = py::dict(),
        "Splits a triangle mesh by another triangle mesh.");
  m.def("intersection_polylines", &cgalpy::pmp::intersection_polylines<Pm>,
        py::arg("tm1"), py::arg("tm2"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        "Computes intersection polylines of two triangle meshes.");

  // Corefine_visitor
  m.def("set_before_subface_creations", &cgalpy::pmp::set_before_subface_creations_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked before subface creations.");
  m.def("set_after_subface_creations", &cgalpy::pmp::set_after_subface_creations_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked after subface creations.");
  m.def("set_before_subface_created", &cgalpy::pmp::set_before_subface_created_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked before each subface is created.");
  m.def("set_after_subface_created", &cgalpy::pmp::set_after_subface_created_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked after each subface is created.");
  m.def("set_before_face_copy", &cgalpy::pmp::set_before_face_copy_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked before a face is copied.");
  m.def("set_after_face_copy", &cgalpy::pmp::set_after_face_copy_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked after a face is copied.");
  m.def("set_before_edge_split", &cgalpy::pmp::set_before_edge_split_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked before an edge is split.");
  m.def("set_edge_split", &cgalpy::pmp::set_edge_split_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when an edge is split.");
  m.def("set_after_edge_split", &cgalpy::pmp::set_after_edge_split_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked after an edge is split.");
  m.def("set_add_retriangulation_edge", &cgalpy::pmp::set_add_retriangulation_edge_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when a retriangulation edge is added.");
  m.def("set_before_edge_copy", &cgalpy::pmp::set_before_edge_copy_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked before an edge is copied.");
  m.def("set_after_edge_copy", &cgalpy::pmp::set_after_edge_copy_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked after an edge is copied.");
  m.def("set_before_edge_duplicated", &cgalpy::pmp::set_before_edge_duplicated_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked before an edge is duplicated.");
  m.def("set_after_edge_duplicated", &cgalpy::pmp::set_after_edge_duplicated_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked after an edge is duplicated.");
  m.def("set_intersection_edge_copy", &cgalpy::pmp::set_intersection_edge_copy_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when an intersection edge is copied.");
  m.def("set_new_vertex_added", &cgalpy::pmp::set_new_vertex_added_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when a new vertex is added.");
  m.def("set_intersection_point_detected", &cgalpy::pmp::set_intersection_point_detected_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when an intersection point is detected.");
  m.def("set_before_vertex_copy", &cgalpy::pmp::set_before_vertex_copy_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked before a vertex is copied.");
  m.def("set_after_vertex_copy", &cgalpy::pmp::set_after_vertex_copy_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked after a vertex is copied.");
  m.def("set_start_filtering_intersections", &cgalpy::pmp::set_start_filtering_intersections_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when intersection filtering starts.");
  m.def("set_progress_filtering_intersections", &cgalpy::pmp::set_progress_filtering_intersections_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked during intersection filtering progress.");
  m.def("set_end_filtering_intersections", &cgalpy::pmp::set_end_filtering_intersections_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when intersection filtering ends.");
  m.def("set_start_triangulating_faces", &cgalpy::pmp::set_start_triangulating_faces_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when face triangulation starts.");
  m.def("set_triangulating_faces_step", &cgalpy::pmp::set_triangulating_faces_step_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked during a face triangulation step.");
  m.def("set_end_triangulating_faces", &cgalpy::pmp::set_end_triangulating_faces_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when face triangulation ends.");
  m.def("set_start_handling_intersection_of_coplanar_faces", &cgalpy::pmp::set_start_handling_intersection_of_coplanar_faces_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when coplanar face handling starts.");
  m.def("set_intersection_of_coplanar_faces_step", &cgalpy::pmp::set_intersection_of_coplanar_faces_step_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked during coplanar face handling.");
  m.def("set_end_handling_intersection_of_coplanar_faces", &cgalpy::pmp::set_end_handling_intersection_of_coplanar_faces_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when coplanar face handling ends.");
  m.def("set_start_handling_edge_face_intersections", &cgalpy::pmp::set_start_handling_edge_face_intersections_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when edge-face intersection handling starts.");
  m.def("set_edge_face_intersections_step", &cgalpy::pmp::set_edge_face_intersections_step_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked during edge-face intersection handling.");
  m.def("set_end_handling_edge_face_intersections", &cgalpy::pmp::set_end_handling_edge_face_intersections_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when edge-face intersection handling ends.");
  m.def("set_start_building_output", &cgalpy::pmp::set_start_building_output_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when output construction starts.");
  m.def("set_end_building_output", &cgalpy::pmp::set_end_building_output_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when output construction ends.");
  m.def("set_filter_coplanar_edges", &cgalpy::pmp::set_filter_coplanar_edges_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when coplanar edges are filtered.");
  m.def("set_detect_patches", &cgalpy::pmp::set_detect_patches_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when patches are detected.");
  m.def("set_classify_patches", &cgalpy::pmp::set_classify_patches_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when patches are classified.");
  m.def("set_classify_intersection_free_patches", &cgalpy::pmp::set_classify_intersection_free_patches_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked when intersection-free patches are classified.");
  m.def("set_out_of_place_operation", &cgalpy::pmp::set_out_of_place_operation_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked for an out-of-place Boolean operation.");
  m.def("set_in_place_operation", &cgalpy::pmp::set_in_place_operation_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked for an in-place Boolean operation.");
  m.def("set_in_place_operations", &cgalpy::pmp::set_in_place_operations_fn,
        py::arg("visitor"), py::arg("callback"),
        "Sets the callback invoked for paired in-place Boolean operations.");

  using Av = cgalpy::pmp::Autorefinement_visitor;
  py::class_<Av>(m, "Autorefinement_visitor",
                 "Visitor for autorefinement callbacks.")
    .def(py::init<>(),
         "Constructs an autorefinement visitor.")
    .def("set_number_of_output_triangles", &Av::set_number_of_output_triangles,
         py::arg("callback"),
         "Sets the callback invoked with the number of output triangles.")
    .def("set_verbatim_triangle_copy", &Av::set_verbatim_triangle_copy,
         py::arg("callback"),
         "Sets the callback invoked when a triangle is copied verbatim.")
    .def("set_new_subtriangle", &Av::set_new_subtriangle,
         py::arg("callback"),
         "Sets the callback invoked when a new subtriangle is created.")
    ;

  // default visitor
  using Dv = cgalpy::pmp::Default_visitor<Pm>;
  py::class_<Dv>(m, "Default_visitor",
                 "Default visitor for corefinement operations.")
    .def(py::init<>(),
         "Constructs a default corefinement visitor.")
    ;

  // corefine
  using Cv = cgalpy::pmp::Corefine_visitor<Pm>;
  py::class_<Cv>(m, "Corefine_visitor",
                 "Visitor for corefinement callbacks.")
    .def(py::init<>(),
         "Constructs a corefinement visitor.")
    ;

  // non-manifold
  using Nmv = cgalpy::pmp::Non_manifold_output_visitor<Pm>;
  py::class_<Nmv>(m, "Non_manifold_output_visitor",
                  "Visitor that can extract non-manifold corefinement output.")
    // constructor with 2 PolygonMesh arguments
    .def(py::init<Pm&, Pm&>(),
         py::arg("mesh1"), py::arg("mesh2"),
         "Constructs a non-manifold output visitor for two meshes.")
    // visitor.extract_intersection(points, polygons);
    .def("extract_intersection", &Nmv::my_extract_intersection,
         "Extracts the intersection as points and polygons.")
    ;
}
