// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com
//            Efi Fogel          <efifogel@gmail.com>

#include <array>
#include <functional>
#include <vector>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/vector.h>

#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

#include "CGALPY/HFDefault_visitor.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "cgalpy/Pmp_docstrings.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace pmp_doc = cgalpy::pmp::docstrings;

namespace cgalpy {
namespace pmp {

using Point_3_vec = std::vector<Point_3>;

// HFDefault_visitor
void set_start_planar_phase(HFDefault_visitor& v, const std::function<void()>& f)
{ v.set_start_planar_phase(f); }

//!
void set_end_planar_phase(HFDefault_visitor& v, const std::function<void(bool)>& f)
{ v.set_end_planar_phase(f); }

//!
void set_start_quadratic_phase(HFDefault_visitor& v, const std::function<void(std::size_t)>& f)
{ v.set_start_quadratic_phase(f); }

//!
void set_quadratic_step(HFDefault_visitor& v, const std::function<void()>& f)
{ v.set_quadratic_step(f); }

//!
void set_end_quadratic_phase(HFDefault_visitor& v, const std::function<void(bool)>& f)
{ v.set_end_quadratic_phase(f); }

//!
void set_start_cubic_phase(HFDefault_visitor& v, const std::function<void(std::size_t)>& f)
{ v.set_start_cubic_phase(f); }

//!
void set_cubic_step(HFDefault_visitor& v, const std::function<void()>& f)
{ v.set_cubic_step(f); }

//!
void set_end_cubic_phase(HFDefault_visitor& v, const std::function<void()>& f)
{ v.set_end_cubic_phase(f); }

//!
void set_start_refine_phase(HFDefault_visitor& v, const std::function<void()>& f)
{ v.set_start_refine_phase(f); }

//!
void set_end_refine_phase(HFDefault_visitor& v, const std::function<void()>& f)
{ v.set_end_refine_phase(f); }

//!
void set_start_fair_phase(HFDefault_visitor& v, const std::function<void()>& f)
{ v.set_start_fair_phase(f); }

//!
void set_end_fair_phase(HFDefault_visitor& v, const std::function<void()>& f)
{ v.set_end_fair_phase(f); }

//!
template <typename PolygonMesh>
auto triangulate_and_refine_hole(PolygonMesh& pmesh,
                                 typename boost::graph_traits<PolygonMesh>::halfedge_descriptor border_halfedge,
                                 const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  std::vector<Fd> faces;
  if (np.contains("visitor")) {
    // HFDefault_visitor
    try {
      auto visitor = py::cast<pmp::HFDefault_visitor>(np["visitor"]);
      PMP::triangulate_and_refine_hole(pmesh, border_halfedge);
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    PMP::triangulate_and_refine_hole(pmesh, border_halfedge);
  }
  return faces;
}

//!
template <typename PolygonMesh>
auto triangulate_hole(PolygonMesh& pmesh,
                      typename boost::graph_traits<PolygonMesh>::halfedge_descriptor border_halfedge,
    const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  if (np.contains("visitor")) {
    // HFDefault_visitor
    try {
      auto visitor = py::cast<pmp::HFDefault_visitor>(np["visitor"]);
      PMP::triangulate_hole(pmesh, border_halfedge);
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    PMP::triangulate_hole(pmesh, border_halfedge);
  }
}

//!
template <typename PolygonMesh>
auto triangulate_hole_polyline_2(const Point_3_vec& polyline1, const Point_3_vec& polyline2,
                                 const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Triangle_int = CGAL::Triple<int, int, int>;
  std::vector<Triangle_int> out;
  if (np.contains("visitor")) {
    // HFDefault_visitor
    try {
      auto visitor = py::cast<pmp::HFDefault_visitor>(np["visitor"]);
      PMP::triangulate_hole_polyline(polyline1, std::back_inserter(out));
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    PMP::triangulate_hole_polyline(polyline1, polyline2, std::back_inserter(out));
  }
  std::array<int, 3> out2 = {out[0].first, out[0].second, out[0].third};
  return out2;
}

//!
template <typename PolygonMesh>
auto triangulate_hole_polyline(const Point_3_vec& polyline, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Triangle_int = CGAL::Triple<int, int, int>;
  std::vector<Triangle_int> out;
  if (np.contains("visitor")) {
    // HFDefault_visitor
    try {
      auto visitor = py::cast<pmp::HFDefault_visitor>(np["visitor"]);
      PMP::triangulate_hole_polyline(polyline, std::back_inserter(out));
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    PMP::triangulate_hole_polyline(polyline, std::back_inserter(out));
  }
  std::array<int, 3> out2 = {out[0].first, out[0].second, out[0].third};
  return out2;
}

//
template <typename PolygonMesh>
auto triangulate_refine_and_fair_hole(PolygonMesh& pmesh,
                                      typename boost::graph_traits<PolygonMesh>::halfedge_descriptor& border_halfedge,
                                      const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Graph_traits = boost::graph_traits<Pm>;
  using halfedge_descriptor = typename Graph_traits::halfedge_descriptor;
  using Fd = typename Graph_traits::face_descriptor;
  using My_visitor = pmp::HFDefault_visitor;
  using Visitor = CGAL::Polygon_mesh_processing::Hole_filling::Default_visitor;

  ///// change this to a more general type
  using Vertex_identifier = typename Graph_traits::vertex_descriptor;
  using Face_identifier = typename Graph_traits::face_descriptor;
  /////

  std::vector<Face_identifier> fids;
  std::vector<Vertex_identifier> vids;
  auto it1 = std::back_inserter(fids);
  auto it2 = std::back_inserter(vids);
  if (np.contains("visitor")) {
    My_visitor visitor = py::cast<My_visitor>(np["visitor"]);
    auto res = PMP::triangulate_refine_and_fair_hole(pmesh, border_halfedge);
    return py::make_tuple(std::get<0>(res), fids, vids);
  }
  else {
    auto res = PMP::triangulate_refine_and_fair_hole(pmesh, border_halfedge);
    return py::make_tuple(std::get<0>(res), fids, vids);
  }
}

}
} // namespace cgalpy

//!
void export_pmp_hole_filling(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;

  using Hfv = cgalpy::pmp::HFDefault_visitor;
  py::class_<Hfv>(m, "Hole_filling_default_visitor",
                  pmp_doc::PMPHolefillingVisitor_class)
    .def(py::init<>(),
         "Constructs a default hole-filling visitor.")
    ;

  // HFDefault_visitor
  m.def("set_start_planar_phase", &cgalpy::pmp::set_start_planar_phase,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPHolefillingVisitor_start_planar_phase);
  m.def("set_end_planar_phase", &cgalpy::pmp::set_end_planar_phase,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPHolefillingVisitor_end_planar_phase);
  m.def("set_start_quadratic_phase", &cgalpy::pmp::set_start_quadratic_phase,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPHolefillingVisitor_start_quadratic_phase);
  m.def("set_quadratic_step", &cgalpy::pmp::set_quadratic_step,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPHolefillingVisitor_quadratic_step);
  m.def("set_end_quadratic_phase", &cgalpy::pmp::set_end_quadratic_phase,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPHolefillingVisitor_end_quadratic_phase);
  m.def("set_start_cubic_phase", &cgalpy::pmp::set_start_cubic_phase,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPHolefillingVisitor_start_cubic_phase);
  m.def("set_cubic_step", &cgalpy::pmp::set_cubic_step,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPHolefillingVisitor_cubic_step);
  m.def("set_end_cubic_phase", &cgalpy::pmp::set_end_cubic_phase,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPHolefillingVisitor_end_cubic_phase);
  m.def("set_start_refine_phase", &cgalpy::pmp::set_start_refine_phase,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPHolefillingVisitor_start_refine_phase);
  m.def("set_end_refine_phase", &cgalpy::pmp::set_end_refine_phase,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPHolefillingVisitor_end_refine_phase);
  m.def("set_start_fair_phase", &cgalpy::pmp::set_start_fair_phase,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPHolefillingVisitor_start_fair_phase);
  m.def("set_end_fair_phase", &cgalpy::pmp::set_end_fair_phase,
        py::arg("visitor"), py::arg("callback"),
        pmp_doc::PMPHolefillingVisitor_end_fair_phase);

  // hole filling
  m.def("triangulate_and_refine_hole", &cgalpy::pmp::triangulate_and_refine_hole<Pm>,
        py::arg("pm"), py::arg("hole_boundary"), py::arg("np") = py::dict(),
        "Triangulates and refines a hole of a polygon mesh.");
  m.def("triangulate_hole", &cgalpy::pmp::triangulate_hole<Pm>,
        py::arg("pmesh"), py::arg("border_halfedge"), py::arg("np") = py::dict(),
        "Triangulates a hole of a polygon mesh.");
  m.def("triangulate_hole_polyline", &cgalpy::pmp::triangulate_hole_polyline<Pm>,
        py::arg("points"), py::arg("np") = py::dict(),
        "Triangulates a hole bounded by a polyline.");
  m.def("triangulate_hole_polyline", &cgalpy::pmp::triangulate_hole_polyline_2<Pm>,
        py::arg("points"), py::arg("third_points"), py::arg("np") = py::dict(),
        "Triangulates a hole bounded by two polylines.");
  m.def("triangulate_refine_and_fair_hole", &cgalpy::pmp::triangulate_refine_and_fair_hole<Pm>,
        py::arg("pmesh"), py::arg("border_halfedge"), py::arg("np") = py::dict(),
        "Triangulates, refines, and fairs a hole of a polygon mesh.");
}
