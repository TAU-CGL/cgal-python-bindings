// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com
//            Efi Fogel          <efifogel@gmail.com>

#include <utility>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>

#include <CGAL/Polygon_mesh_processing/compute_normal.h>

//! \todo remove
#include "CGALPY/pmp_np_parser.hpp"

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/Named_parameter_wrapper.hpp"
#include "CGALPY/named_parameter_applicator.hpp"
#include "CGALPY/Named_parameter_geom_traits.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

//!
template <typename PolygonMesh>
Vector_3 compute_face_normal(const typename boost::graph_traits<PolygonMesh>::face_descriptor& f,
                             const PolygonMesh& mesh,
                             const py::dict& params = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::compute_face_normal(f, mesh, internal::parse_pmp_np<Pm>(params));
}

/*! A class template that wraps the function template
 * PMP::compute_face_normals()
 */
template <typename T, typename... Args>
struct Compute_face_normals_wrapper {
  static void call(T np, Args&&... args) {
    PMP::compute_face_normals(std::forward<Args>(args)..., std::forward<T>(np));
  }
};

//!
template <typename PolygonMesh, typename FaceNormalMap>
void compute_face_normals(const PolygonMesh& mesh,
                          FaceNormalMap face_normals,
                          const py::dict& params = py::dict()) {
  using Pm = PolygonMesh;
  using Fn_map = FaceNormalMap;

  auto np = CGAL::parameters::default_values();
  CGALPY::Named_parameter_geom_traits op;
  CGALPY::Named_parameter_wrapper<Compute_face_normals_wrapper,
                                  const Pm&, const Fn_map&>
    wrapper(mesh, face_normals);
  CGALPY::named_parameter_applicator(wrapper, np, params, op);
}

//!
template<typename PolygonMesh, typename VertexNormalMap, typename FaceNormalMap>
auto compute_normals(const PolygonMesh& pm,
                     VertexNormalMap vnormals,
                     FaceNormalMap fnormals,
                     const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::compute_normals(pm, vnormals, fnormals,
                              internal::parse_pmp_np<Pm>(np));
}

//!
template <typename PolygonMesh>
Vector_3 compute_vertex_normal(const typename boost::graph_traits<PolygonMesh>::vertex_descriptor& v,
                               const PolygonMesh& mesh,
                               const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::compute_vertex_normal(v, mesh, internal::parse_pmp_np<Pm>(np));
}

//!
template <typename PolygonMesh, typename VertexNormalMap>
auto compute_vertex_normals(const PolygonMesh& mesh,
                            VertexNormalMap vertex_normals,
                            const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::compute_vertex_normals(mesh, vertex_normals,
                                     internal::parse_pmp_np<Pm>(np));
}

}

//!
void export_pmp_normal_computation(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  using Vertex_vector_map = Pm::Property_map<Vd, Vector_3>;
  using Face_vector_map = Pm::Property_map<Fd, Vector_3>;
#endif

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  using Vector_vector_tag = CGAL::dynamic_vertex_property_t<Vector_3>;
  using Vertex_vector_map = boost::property_map<Pm, Vector_vector_tag>::type;

  using Face_vector_tag = CGAL::dynamic_face_property_t<Vector_3>;
  using Face_vector_map = boost::property_map<Pm, Face_vector_tag>::type;
#endif

#if ((CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH) && \
     (CGALPY_POL3_GEOMETRY_TRAITS == CGALPY_POL3_WITH_NORMALS_GEOMETRY_TRAITS))
  using Face_normal_map = pol3::Internal_face_plane_3_map<Pm>;
#else
  using Face_normal_map = Face_vector_map;
#endif

  m.def("compute_face_normal", &pmp::compute_face_normal<Pm>,
        py::arg("f"), py::arg("pmesh"),
        py::arg("np") = py::dict());

  m.def("compute_face_normals",
        &pmp::compute_face_normals<Pm, Face_normal_map>,
        py::arg("pmesh"), py::arg("face_normals"),
        py::arg("np") = py::dict());

  m.def("compute_normals",
        &pmp::compute_normals<Pm, Vertex_vector_map, Face_vector_map>,
        py::arg("vnormals"), py::arg("fnormals"), py::arg("pmesh"),
        py::arg("np") = py::dict());

  m.def("compute_vertex_normal", &pmp::compute_vertex_normal<Pm>,
        py::arg("v"), py::arg("pmesh"),
        py::arg("np") = py::dict());

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  m.def("compute_vertex_normals",
        &pmp::compute_vertex_normals<Pm, Vertex_vector_map>,
        py::arg("pmesh"), py::arg("vertex_normals"),
        py::arg("np") = py::dict());
#endif
}
