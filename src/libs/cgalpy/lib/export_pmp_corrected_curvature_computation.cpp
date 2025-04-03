// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#include <functional>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/function.h>

#include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h>

//! \todo remove
#include "CGALPY/pmp_np_parser.hpp"
#include "CGALPY/pmp_helpers.hpp"

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

//!
template <typename PolygonMesh>
void interpolated_corrected_curvatures(PolygonMesh& pmesh,
                                       const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;

  auto vmcm = get_vertex_prop_map<Pm, FT>
    (pmesh, "INTERNAL_MAP0",
     np.contains("vertex_mean_curvature_map") ?
     np["vertex_mean_curvature_map"] : py::none());
  auto vgcm = get_vertex_prop_map<Pm, FT>
    (pmesh, "INTERNAL_MAP1",
     np.contains("vertex_Gaussian_curvature_map") ?
     np["vertex_Gaussian_curvature_map"] : py::none());

  using Pcad = PMP::Principal_curvatures_and_directions<Kernel>;
  auto vpcdm = get_vertex_prop_map<Pm, Pcad>
    (pmesh, "INTERNAL_MAP2",
     np.contains("vertex_principal_curvatures_and_directions_map") ?
     np["vertex_principal_curvatures_and_directions_map"] : py::none());


  if (np.contains("vertex_normal_map")) {
    auto vnm = get_vertex_prop_map<Pm, Vector_3>
      (pmesh, "INTERNAL_MAP3", np.contains("vertex_normal_map") ?
       np["vertex_normal_map"] : py::none());
    // auto vpmap = get_vertex_point_map(pm, np); // does not work
    PMP::interpolated_corrected_curvatures(pmesh, internal::parse_pmp_np<Pm>(np)
                                           .vertex_mean_curvature_map(vmcm)
                                           .vertex_Gaussian_curvature_map(vgcm)
                                           .vertex_principal_curvatures_and_directions_map(vpcdm)
                                           .vertex_normal_map(vnm)
                                           );

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
    if (np.contains("vertex_normal_map")) {
      pmesh.remove_property_map(vnm);
    }
#endif

  }
  else {
    PMP::interpolated_corrected_curvatures(pmesh, internal::parse_pmp_np<Pm>(np)
                                           .vertex_mean_curvature_map(vmcm)
                                           .vertex_Gaussian_curvature_map(vgcm)
                                           .vertex_principal_curvatures_and_directions_map(vpcdm)
                                           );
  }
  // delete the internal maps

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
auto interpolated_corrected_curvatures_v(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
                                         PolygonMesh& pm,
                                         const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  py::object vmc = py::none();
  py::object vgc = py::none();

  auto pcad = PMP::Principal_curvatures_and_directions<Kernel>();

  double vmc_d, vGc;
  if (np.contains("vertex_normal_map")) {
    auto vnm = get_vertex_prop_map<Pm, Vector_3>
      (pm, "INTERNAL_MAP1", np.contains("vertex_normal_map") ? np["vertex_normal_map"] : py::none());
    PMP::interpolated_corrected_curvatures(v, pm,
    internal::parse_pmp_np<Pm>(np)
                                           .vertex_mean_curvature(std::ref(vmc_d))
                                           .vertex_Gaussian_curvature(std::ref(vGc))
                                           .vertex_principal_curvatures_and_directions(std::ref(pcad))
                                           .vertex_normal_map(vnm)
                                           );

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
    pm.remove_property_map(vnm);
#endif

  }
  else {
    PMP::interpolated_corrected_curvatures(v, pm,
    internal::parse_pmp_np<Polygonal_mesh>(np)
                                           .vertex_mean_curvature(std::ref(vmc_d))
                                           .vertex_Gaussian_curvature(std::ref(vGc))
                                           .vertex_principal_curvatures_and_directions(std::ref(pcad))
                                           );
  }
  return std::make_tuple(vmc_d, vGc, pcad);
}

}

//!
void export_pmp_corrected_curvature_computation(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;

  using Pcad = PMP::Principal_curvatures_and_directions<Kernel>;
  py::class_<Pcad>(m, "Principal_curvatures_and_directions")
    .def(py::init<FT, FT, Vector_3, Vector_3>())
    .def_ro("min_curvature", &Pcad::min_curvature)
    .def_ro("max_curvature", &Pcad::max_curvature)
    .def_ro("min_direction", &Pcad::min_direction)
    .def_ro("max_direction", &Pcad::max_direction)
    ;

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  //! \todo Fix interpolated_corrected_curvatures to use epeck
  // Corrected Curvature Computation
  m.def("interpolated_corrected_curvatures",
        &pmp::interpolated_corrected_curvatures<Pm>,
        py::arg("pm"), py::arg("np") = py::dict());

  m.def("interpolated_corrected_curvatures",
        &pmp::interpolated_corrected_curvatures_v<Pm>,
        py::arg("v"), py::arg("pm"), py::arg("np") = py::dict());
#endif
}
