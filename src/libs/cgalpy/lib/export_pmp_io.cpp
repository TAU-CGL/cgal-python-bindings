// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/named_parameter_applicator.hpp"
#include "CGALPY/Named_parameter_repair_polygon_soup.hpp"
#include "CGALPY/Named_parameter_verbose.hpp"
#include "CGALPY/Named_parameter_wrapper.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
#include "CGALPY/polyhedron_3_types.hpp"
#endif

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

/*! A class template that wraps the function template
 * CGAL::IO::read_polygon_mesh()
 */
template <typename NamedParameter, typename... Args>
struct Read_polygon_mesh_wrapper {
  static auto call(NamedParameter& np, Args&&... args)
  { return CGAL::IO::read_polygon_mesh(std::forward<Args>(args)..., np); }
};

/*! Read a surface mesh from a file.
 */
template <typename PolygonalMesh>
void read_polygon_mesh_impl(const std::string& filename,
                            PolygonalMesh& prn,
                            const py::dict& params = py::dict()) {
  using Pm = PolygonalMesh;
  auto np = CGAL::parameters::default_values();
  CGALPY::Named_parameter_verbose op1;
  CGALPY::Named_parameter_repair_polygon_soup op2;
  CGALPY::Named_parameter_wrapper<Read_polygon_mesh_wrapper, const std::string&, Pm&> wrapper(filename, prn);
  bool res = CGALPY::named_parameter_applicator(wrapper, np, params, op1, op2);
  if (! res) throw std::runtime_error("Cannot read file!");
}

// Read a polyhedron from a file.
template <typename PolygonalMesh>
PolygonalMesh read_polygon_mesh(const std::string& filename, const py::dict& params = py::dict()) {
  PolygonalMesh prn;
  read_polygon_mesh_impl(filename, prn, params);
  return prn;
}

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
// Read a polyhedron from a file.
  template <typename PolygonalMesh, typename Traits>
PolygonalMesh read_polygon_mesh_with_traits(const std::string& filename, const Traits& traits,
                                            const py::dict& params = py::dict()) {
  PolygonalMesh prn(traits);
  read_polygon_mesh_impl(filename, prn, params);
  return prn;
}
#endif

}

//!
void export_pmp_io(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;
  m.def("read_polygon_mesh", &pmp::read_polygon_mesh<Pm>,
        py::arg("filename"), py::arg("params") = py::dict());
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("read_polygon_mesh", &pmp::read_polygon_mesh_with_traits<Pm, pol3::Traits>,
        py::arg("filename"), py::arg("traits"), py::arg("params") = py::dict(),
        py::keep_alive<0, 2>());
#endif
  ;
}
