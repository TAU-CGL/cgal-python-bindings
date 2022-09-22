// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <boost/iterator/function_output_iterator.hpp>

#include <nanobind/nanobind.h>

#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/connected_components.h>
#include <CGAL/boost/graph/Face_filtered_graph.h>

#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;
namespace pmp = CGAL::Polygon_mesh_processing;

namespace detail {

//
template <typename PolygonMesh>
py::list
connected_component(typename boost::graph_traits<PolygonMesh>::face_descriptor
                      seed_face,
                    const PolygonMesh& pmesh,
                    py::dict parameters) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  py::list lst;
  auto op = [&] (Fd face_descriptor) mutable
            { lst.append(py::cast(face_descriptor)); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  pmp::connected_component(seed_face, pmesh, it);
  return lst;
}

//
template <typename PolygonMesh>
py::list connected_components(const PolygonMesh& pmesh,
                               py::dict parameters = py::dict()) {
  using Pm = PolygonMesh;

  auto fccmap = CGAL::get(CGAL::dynamic_face_property_t<std::size_t>(), pmesh);
  auto num = pmp::connected_components(pmesh, fccmap);
  py::dict dct;
  for (auto f : pmesh.faces()) dct[py::cast(f)] = py::cast(get(fccmap, f));
  py::list lst;
  lst.append(num);
  lst.append(std::move(dct));
  return lst;
}

}

// Export Polygon_mesh_processing
void export_polygon_mesh_processing(py::module_& m) {
  using Sm_3 = CGAL::Surface_mesh<Kernel::Point_3>;

  m.def("connected_component", &detail::connected_component<Sm_3>,
        py::arg("seed_face"), py::arg("pmesh"),
        py::arg("parameters") = py::dict());
  m.def("connected_components", &detail::connected_components<Sm_3>,
        py::arg("pmesh"), py::arg("parameters") = py::dict());
}
