// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#include <array>
#include <stdexcept>
#include <variant>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/variant.h>

#include <CGAL/Polygon_mesh_processing/locate.h>

#include "cgalpy/kernel_types.hpp"
#include "cgalpy/polygon_mesh_processing_types.hpp"
#include "cgalpy/Pmp_docstrings.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace pmp_doc = cgalpy::pmp::docstrings;

namespace cgalpy {
namespace pmp {

//!
auto barycentic_coordinates(const Point_3& p, const Point_3& a,
                            const Point_3& b, const Point_3& c) {
  return PMP::barycentric_coordinates(p, a, b, c, Kernel());
}

//!
template <typename TriangleMesh>
auto get_descriptor_from_location(const std::pair<
                                  typename boost::graph_traits<TriangleMesh>::face_descriptor,
                                  std::array<FT, 3> >& loc,
                                  const TriangleMesh& tm) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Fd = typename Gt::face_descriptor;
  using Vd = typename Gt::vertex_descriptor;
  using Hd = typename Gt::halfedge_descriptor;
  using Barycentric_coordinates = std::array<FT, 3>;
  // returns a variant of vertex_descriptor, halfedge_descriptor, face_descriptor
  try {
    return py::cast(std::get<Vd>(PMP::get_descriptor_from_location(loc, tm)));
  }
  catch (const std::bad_variant_access& e) {
    try {
      return py::cast(std::get<Hd>(PMP::get_descriptor_from_location(loc, tm)));
    }
    catch (const std::bad_variant_access& e) {
      try {
        return py::cast(std::get<Fd>(PMP::get_descriptor_from_location(loc, tm)));
      }
      catch (const std::bad_variant_access& e) {
        throw std::runtime_error("get_descriptor_from_location failed");
      }
    }
  }
}

//!
template <typename TriangleMesh>
auto is_in_face_bar(const std::array<FT, 3>& bar, const TriangleMesh& tm)
{ return PMP::is_in_face(bar, tm); }

//!
template <typename TriangleMesh>
auto is_in_face_loc(const std::pair<
                    typename boost::graph_traits<TriangleMesh>::face_descriptor,
                    std::array<FT, 3>>& loc,
                    const TriangleMesh& tm)
{ return PMP::is_in_face(loc, tm); }

//!
template <typename TriangleMesh>
auto is_on_face_border(const std::pair<
                       typename boost::graph_traits<TriangleMesh>::face_descriptor,
                       std::array<FT, 3> >& loc,
                       const TriangleMesh& tm)
{ return PMP::is_on_face_border(loc, tm); }

//!
template <typename TriangleMesh>
auto is_on_halfedge(const std::pair<
                typename boost::graph_traits<TriangleMesh>::face_descriptor,
                std::array<FT, 3> >& loc,
                    const typename boost::graph_traits<TriangleMesh>::halfedge_descriptor& hd,
                    const TriangleMesh& tm)
{ return PMP::is_on_halfedge(loc, hd, tm); }

//!
template <typename TriangleMesh>
auto is_on_mesh_border(const std::pair<
                typename boost::graph_traits<TriangleMesh>::face_descriptor,
                std::array<FT, 3> >& loc,
                       const TriangleMesh& tm)
{ return PMP::is_on_mesh_border(loc, tm); }

//!
template <typename TriangleMesh>
auto is_on_vertex(const std::pair<
                  typename boost::graph_traits<TriangleMesh>::face_descriptor,
                  std::array<FT, 3> >& loc,
                   const typename boost::graph_traits<TriangleMesh>::vertex_descriptor& vd,
                   const TriangleMesh& tm)
{ return PMP::is_on_vertex(loc, vd, tm); }


#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
template <typename TriangleMesh>
auto make_polyhedron_location(const py::tuple& loc) {
  if (py::len(loc) != 2)
    throw std::runtime_error("Location must be a pair (face, barycentric coordinates).");

  using Face = typename TriangleMesh::Face;
  using Fd = typename boost::graph_traits<TriangleMesh>::face_descriptor;

  Face& face = py::cast<Face&>(loc[0]);
  auto bar = py::cast<std::array<FT, 3>>(loc[1]);
  return std::make_pair(Fd(&face), bar);
}

template <typename TriangleMesh>
auto get_descriptor_from_location_pol3(const py::tuple& loc, const TriangleMesh& tm) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Fd = typename Gt::face_descriptor;
  using Vd = typename Gt::vertex_descriptor;
  using Hd = typename Gt::halfedge_descriptor;

  const auto descriptor = PMP::get_descriptor_from_location
    (make_polyhedron_location<TriangleMesh>(loc), tm);

  try {
    return py::cast(*std::get<Vd>(descriptor), py::rv_policy::reference);
  }
  catch (const std::bad_variant_access&) {
    try {
      return py::cast(*std::get<Hd>(descriptor), py::rv_policy::reference);
    }
    catch (const std::bad_variant_access&) {
      try {
        return py::cast(*std::get<Fd>(descriptor), py::rv_policy::reference);
      }
      catch (const std::bad_variant_access&) {
        throw std::runtime_error("get_descriptor_from_location failed");
      }
    }
  }
}

template <typename TriangleMesh>
auto is_in_face_loc_pol3(const py::tuple& loc, const TriangleMesh& tm)
{ return PMP::is_in_face(make_polyhedron_location<TriangleMesh>(loc), tm); }

template <typename TriangleMesh>
auto is_on_face_border_pol3(const py::tuple& loc, const TriangleMesh& tm)
{ return PMP::is_on_face_border(make_polyhedron_location<TriangleMesh>(loc), tm); }

template <typename TriangleMesh>
auto is_on_halfedge_pol3(const py::tuple& loc, typename TriangleMesh::Halfedge& halfedge,
                         const TriangleMesh& tm) {
  using Hd = typename boost::graph_traits<TriangleMesh>::halfedge_descriptor;
  return PMP::is_on_halfedge(make_polyhedron_location<TriangleMesh>(loc), Hd(&halfedge), tm);
}

template <typename TriangleMesh>
auto is_on_mesh_border_pol3(const py::tuple& loc, const TriangleMesh& tm)
{ return PMP::is_on_mesh_border(make_polyhedron_location<TriangleMesh>(loc), tm); }

template <typename TriangleMesh>
auto is_on_vertex_pol3(const py::tuple& loc, typename TriangleMesh::Vertex& vertex,
                       const TriangleMesh& tm) {
  using Vd = typename boost::graph_traits<TriangleMesh>::vertex_descriptor;
  return PMP::is_on_vertex(make_polyhedron_location<TriangleMesh>(loc), Vd(&vertex), tm);
}
#endif

}
} // namespace cgalpy

//!
void export_pmp_location(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;

  m.def("barycentric_coordinates", &cgalpy::pmp::barycentic_coordinates,
        py::arg("p"), py::arg("q"), py::arg("r"), py::arg("query"),
        pmp_doc::Polygon_mesh_processing_barycentric_coordinates);
  m.def("barycentic_coordinates", &cgalpy::pmp::barycentic_coordinates,
        py::arg("p"), py::arg("q"), py::arg("r"), py::arg("query"),
        pmp_doc::Polygon_mesh_processing_barycentric_coordinates);
  m.def("get_descriptor_from_location", &cgalpy::pmp::get_descriptor_from_location<Pm>,
        py::arg("loc"), py::arg("tm"),
        pmp_doc::Polygon_mesh_processing_get_descriptor_from_location);
  m.def("is_in_face", &cgalpy::pmp::is_in_face_bar<Pm>,
        py::arg("bar"), py::arg("tm"),
        pmp_doc::Polygon_mesh_processing_is_in_face);
  m.def("is_in_face", &cgalpy::pmp::is_in_face_loc<Pm>,
        py::arg("loc"), py::arg("tm"),
        pmp_doc::Polygon_mesh_processing_is_in_face);
  m.def("is_on_face_border", &cgalpy::pmp::is_on_face_border<Pm>,
        py::arg("loc"), py::arg("tm"),
        pmp_doc::Polygon_mesh_processing_is_on_face_border);
  m.def("is_on_halfedge", &cgalpy::pmp::is_on_halfedge<Pm>,
        py::arg("loc"), py::arg("hd"), py::arg("tm"),
        pmp_doc::Polygon_mesh_processing_is_on_halfedge);
  m.def("is_on_mesh_border", &cgalpy::pmp::is_on_mesh_border<Pm>,
        py::arg("loc"), py::arg("tm"),
        pmp_doc::Polygon_mesh_processing_is_on_mesh_border);
  m.def("is_on_vertex", &cgalpy::pmp::is_on_vertex<Pm>,
        py::arg("loc"), py::arg("vd"), py::arg("tm"),
        pmp_doc::Polygon_mesh_processing_is_on_vertex);
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("get_descriptor_from_location", &cgalpy::pmp::get_descriptor_from_location_pol3<Pm>,
        py::arg("loc"), py::arg("tm"),
        pmp_doc::Polygon_mesh_processing_get_descriptor_from_location);
  m.def("is_in_face", &cgalpy::pmp::is_in_face_loc_pol3<Pm>,
        py::arg("loc"), py::arg("tm"),
        pmp_doc::Polygon_mesh_processing_is_in_face);
  m.def("is_on_face_border", &cgalpy::pmp::is_on_face_border_pol3<Pm>,
        py::arg("loc"), py::arg("tm"),
        pmp_doc::Polygon_mesh_processing_is_on_face_border);
  m.def("is_on_halfedge", &cgalpy::pmp::is_on_halfedge_pol3<Pm>,
        py::arg("loc"), py::arg("hd"), py::arg("tm"),
        pmp_doc::Polygon_mesh_processing_is_on_halfedge);
  m.def("is_on_mesh_border", &cgalpy::pmp::is_on_mesh_border_pol3<Pm>,
        py::arg("loc"), py::arg("tm"),
        pmp_doc::Polygon_mesh_processing_is_on_mesh_border);
  m.def("is_on_vertex", &cgalpy::pmp::is_on_vertex_pol3<Pm>,
        py::arg("loc"), py::arg("vd"), py::arg("tm"),
        pmp_doc::Polygon_mesh_processing_is_on_vertex);
#endif
}
