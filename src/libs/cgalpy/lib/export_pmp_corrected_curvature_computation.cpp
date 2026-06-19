// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#include <functional>
#include <stdexcept>
#include <utility>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/function.h>

#include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h>

#include "cgalpy/kernel_types.hpp"
#include "cgalpy/Named_parameter_geom_traits.hpp"
#include "cgalpy/Named_parameter_vertex_point_map.hpp"
#include "cgalpy/Named_parameter_wrapper.hpp"
#include "cgalpy/named_parameter_applicator.hpp"
#include "cgalpy/polygon_mesh_processing_config.hpp"
#include "cgalpy/polygon_mesh_processing_types.hpp"
#include "cgalpy/pmp_helpers.hpp"
#include "cgalpy/Pmp_docstrings.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace pmp_doc = cgalpy::pmp::docstrings;

namespace cgalpy {
namespace pmp {

//!
struct Named_parameter_ball_radius {
  const std::string m_name = "ball_radius";

  template <typename NamedParameters, typename Value>
  auto operator()(NamedParameters& np, Value& value) const
  { return np.ball_radius(py::cast<FT>(value)); }
};

//!
template <typename PolygonMesh>
struct Named_parameter_vertex_normal_map {
  const std::string m_name = "vertex_normal_map";

  template <typename NamedParameters, typename Value>
  auto operator()(NamedParameters& np, Value& value) const {
    using Vd = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
    using Vertex_normal_map = typename PolygonMesh::template Property_map<Vd, Vector_3>;
#elif CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
    using Vertex_normal_map =
      typename boost::property_map<PolygonMesh,
                                   CGAL::dynamic_vertex_property_t<Vector_3>>::type;
#else
#  error Unsupported PMP polygonal mesh type
#endif

    try {
      return np.vertex_normal_map(py::cast<Vertex_normal_map>(value));
    }
    catch (const py::cast_error&) {
      throw std::runtime_error
        ("Named parameter 'vertex_normal_map' must be a vertex normal property map.");
    }
  }
};

//!
template <typename NPType, typename PolygonMesh>
struct Interpolated_corrected_curvatures_wrapper {
  static void call(NPType&& np, PolygonMesh& pmesh)
  { PMP::interpolated_corrected_curvatures(pmesh, std::forward<NPType>(np)); }
};

//!
template <typename NPType, typename VertexDescriptor, typename PolygonMesh>
struct Interpolated_corrected_curvatures_v_wrapper {
  static void call(NPType&& np, VertexDescriptor v, PolygonMesh& pmesh)
  { PMP::interpolated_corrected_curvatures(v, pmesh, std::forward<NPType>(np)); }
};

//!
template <typename PolygonMesh>
void interpolated_corrected_curvatures(PolygonMesh& pmesh,
                                       const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Pcad = PMP::Principal_curvatures_and_directions<Kernel>;

  auto vmcm = get_vertex_prop_map<Pm, FT>
    (pmesh, "INTERNAL_MAP0",
     np.contains("vertex_mean_curvature_map") ?
     np["vertex_mean_curvature_map"] : py::none());

  auto vgcm = get_vertex_prop_map<Pm, FT>
    (pmesh, "INTERNAL_MAP1",
     np.contains("vertex_Gaussian_curvature_map") ?
     np["vertex_Gaussian_curvature_map"] : py::none());

  auto vpcdm = get_vertex_prop_map<Pm, Pcad>
    (pmesh, "INTERNAL_MAP2",
     np.contains("vertex_principal_curvatures_and_directions_map") ?
     np["vertex_principal_curvatures_and_directions_map"] : py::none());

  auto default_np =
    CGAL::parameters::vertex_mean_curvature_map(vmcm)
      .vertex_Gaussian_curvature_map(vgcm)
      .vertex_principal_curvatures_and_directions_map(vpcdm);

  cgalpy::Named_parameter_vertex_point_map<Pm> vertex_point_map_op;
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  Named_parameter_vertex_normal_map<Pm> vertex_normal_map_op;
  Named_parameter_ball_radius ball_radius_op;

  cgalpy::Named_parameter_wrapper
    <Interpolated_corrected_curvatures_wrapper, Pm&> wrapper(pmesh);

  cgalpy::named_parameter_applicator(wrapper, default_np, np,
                                     vertex_point_map_op,
                                     vertex_normal_map_op,
                                     geom_traits_op,
                                     ball_radius_op);

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("vertex_mean_curvature_map")) {
    pmesh.remove_property_map(vmcm);
  }
  if (! np.contains("vertex_Gaussian_curvature_map")) {
    pmesh.remove_property_map(vgcm);
  }
  if (! np.contains("vertex_principal_curvatures_and_directions_map")) {
    pmesh.remove_property_map(vpcdm);
  }
#endif
}

//!
template <typename PolygonMesh>
auto interpolated_corrected_curvatures_v
(
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
 typename boost::graph_traits<PolygonMesh>::vertex_descriptor vertex,
#elif CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
 typename PolygonMesh::Vertex& vertex,
#else
#  error Unsupported PMP polygonal mesh type
#endif
 PolygonMesh& pmesh, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Vd = typename boost::graph_traits<Pm>::vertex_descriptor;
  using Pcad = PMP::Principal_curvatures_and_directions<Kernel>;

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  Vd v = vertex;
#elif CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  Vd v(&vertex);
#endif

  FT vmc(0);
  FT vgc(0);
  Pcad pcad;

  auto default_np =
    CGAL::parameters::vertex_mean_curvature(std::ref(vmc))
      .vertex_Gaussian_curvature(std::ref(vgc))
      .vertex_principal_curvatures_and_directions(std::ref(pcad));

  cgalpy::Named_parameter_vertex_point_map<Pm> vertex_point_map_op;
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  Named_parameter_vertex_normal_map<Pm> vertex_normal_map_op;
  Named_parameter_ball_radius ball_radius_op;

  cgalpy::Named_parameter_wrapper
    <Interpolated_corrected_curvatures_v_wrapper, Vd, Pm&>
    wrapper(std::move(v), pmesh);

  cgalpy::named_parameter_applicator(wrapper, default_np, np,
                                     vertex_point_map_op,
                                     vertex_normal_map_op,
                                     geom_traits_op,
                                     ball_radius_op);

  return std::make_tuple(vmc, vgc, pcad);
}

}
} // namespace cgalpy

//!
void export_pmp_corrected_curvature_computation(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;

  using Pcad = PMP::Principal_curvatures_and_directions<Kernel>;
  py::class_<Pcad>(m, "Principal_curvatures_and_directions",
                  pmp_doc::Polygon_mesh_processing_Principal_curvatures_and_directions_class)
    .def(py::init<FT, FT, Vector_3, Vector_3>(),
         py::arg("min_curvature"), py::arg("max_curvature"),
         py::arg("min_direction"), py::arg("max_direction"),
         "Constructs principal curvatures and directions.")
    .def_ro("min_curvature", &Pcad::min_curvature,
            pmp_doc::Polygon_mesh_processing_Principal_curvatures_and_directions_min_curvature)
    .def_ro("max_curvature", &Pcad::max_curvature,
            pmp_doc::Polygon_mesh_processing_Principal_curvatures_and_directions_max_curvature)
    .def_ro("min_direction", &Pcad::min_direction,
            pmp_doc::Polygon_mesh_processing_Principal_curvatures_and_directions_min_direction)
    .def_ro("max_direction", &Pcad::max_direction,
            pmp_doc::Polygon_mesh_processing_Principal_curvatures_and_directions_max_direction)
    ;

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  //! \todo Fix interpolated_corrected_curvatures to use epeck
  // Corrected Curvature Computation
  m.def("interpolated_corrected_curvatures",
        &cgalpy::pmp::interpolated_corrected_curvatures<Pm>,
        py::arg("pmesh"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_interpolated_corrected_curvatures);

  m.def("interpolated_corrected_curvatures",
        &cgalpy::pmp::interpolated_corrected_curvatures_v<Pm>,
        py::arg("v"), py::arg("pmesh"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_interpolated_corrected_curvatures_1);
#endif
}
