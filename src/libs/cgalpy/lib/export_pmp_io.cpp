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

#include "cgalpy/kernel_types.hpp"
#include "cgalpy/named_parameter_applicator.hpp"
#include "cgalpy/Named_parameter_repair_polygon_soup.hpp"
#include "cgalpy/Named_parameter_stream_precision.hpp"
#include "cgalpy/Named_parameter_use_binary_mode.hpp"
#include "cgalpy/Named_parameter_verbose.hpp"
#include "cgalpy/Named_parameter_wrapper.hpp"
#include "cgalpy/polygon_mesh_processing_types.hpp"
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
#include "cgalpy/polyhedron_3_types.hpp"
#endif

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace cgalpy {
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
  cgalpy::Named_parameter_verbose op1;
  cgalpy::Named_parameter_repair_polygon_soup op2;
  cgalpy::Named_parameter_wrapper<Read_polygon_mesh_wrapper, const std::string&, Pm&> wrapper(filename, prn);
  bool res = cgalpy::named_parameter_applicator(wrapper, np, params, op1, op2);
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

/*! A class template that wraps the function template
 * CGAL::IO::write_polygon_mesh()
 */
template <typename NamedParameter, typename... Args>
struct Write_polygon_mesh_wrapper {
  static auto call(NamedParameter& np, Args&&... args)
  { return CGAL::IO::write_polygon_mesh(std::forward<Args>(args)..., np); }
};

//!
template <typename PolygonalMesh>
bool write_polygon_mesh(const std::string& filename, const PolygonalMesh& pm,
                        const py::dict& params = py::dict()) {
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_verbose verbose_op;
  cgalpy::Named_parameter_stream_precision stream_precision_op;
  cgalpy::Named_parameter_use_binary_mode use_binary_mode_op;
  cgalpy::Named_parameter_wrapper<Write_polygon_mesh_wrapper,
                                  const std::string&, const PolygonalMesh&>
    wrapper(filename, pm);
  return cgalpy::named_parameter_applicator(wrapper, np, params, verbose_op,
                                           stream_precision_op, use_binary_mode_op);
}

}
} // namespace cgalpy

//!
void export_pmp_io(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;
  m.def("read_polygon_mesh", &cgalpy::pmp::read_polygon_mesh<Pm>,
        py::arg("filename"), py::arg("params") = py::dict(),
        "Reads a polygon mesh from a file.");
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("read_polygon_mesh", &cgalpy::pmp::read_polygon_mesh_with_traits<Pm, cgalpy::pol3::Traits>,
        py::arg("filename"), py::arg("traits"), py::arg("params") = py::dict(),
        py::keep_alive<0, 2>(),
        "Reads a polygon mesh from a file using the given traits object.");
#endif

  m.def("write_polygon_mesh", &cgalpy::pmp::write_polygon_mesh<Pm>,
        py::arg("filename"), py::arg("pm"), py::arg("params") = py::dict(),
        "Writes a polygon mesh to a file.");
  ;
}
