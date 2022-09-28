// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <stdexcept>

#include <boost/iterator/function_output_iterator.hpp>

#include <nanobind/nanobind.h>

#include <CGAL/Polygon_mesh_processing/connected_components.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace PARMS = CGAL::parameters;

namespace pmp {

template <typename PolygonalMesh>
struct Visitor : public PMP::Corefinement::Default_visitor<PolygonalMesh> {
  // Construct default
  Visitor() {}

  void progress_filtering_intersections(double d)
  { std::cout << "Visitor::progress_filtering_intersections()" << std::endl; }

  void start_filtering_intersections() const
  { std::cout << "Visitor::start_filtering_intersections()" << std::endl; }

  void end_filtering_intersections() const
  { std::cout << "Visitor::end_filtering_intersections()" << std::endl; }

  void start_triangulating_faces(std::size_t tf)const
  { std::cout << "Visitor::start_triangulation_faces()" << std::endl; }

  void face_triangulation(std::size_t i) const
  { std::cout << "Visitor::face_triangulation()" << std::endl; }

  void end_triangulating_faces() const
  { std::cout << "Visitor::end_triangulating_faces()" << std::endl; }

  void start_coplanar_faces(std::size_t i) const
  { std::cout << "Visitor::start_coplanar_faces()" << std::endl; }

  void intersection_of_coplanar_faces_step() const
  { std::cout << "Visitor::intersection_of_coplanar_faces_step()" << std::endl; }

  void end_coplanar_faces() const
  { std::cout << "Visitor::end_coplanar_faces()" << std::endl; }

  void start_intersection_points(std::size_t i) const
  { std::cout << "Visitor::start_intersection_points()" << std::endl; }

  void edge_face_intersections_step() const
  { std::cout << "Visitor::edge_face_intersections_step()" << std::endl; }

  void end_intersection_points() const
  { std::cout << "Visitor::end_intersection_points()" << std::endl; }

  void start_building_output() const
  { std::cout << "Visitor::start_building_output()" << std::endl; }

  void end_building_output() const
  { std::cout << "Visitor::end_building_output()" << std::endl; }
};

//
template <typename PolygonMesh>
py::list
connected_component(typename boost::graph_traits<PolygonMesh>::face_descriptor
                      seed_face,
                    const PolygonMesh& pm, py::dict parameters) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  py::list lst;
  auto op = [&] (Fd face_descriptor) mutable
            { lst.append(py::cast(face_descriptor)); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  PMP::connected_component(seed_face, pm, it);
  return lst;
}

//
template <typename PolygonMesh>
py::list connected_components(const PolygonMesh& pm,
                              py::dict parameters = py::dict()) {
  using Pm = PolygonMesh;

  auto fccmap = CGAL::get(CGAL::dynamic_face_property_t<std::size_t>(), pm);
  auto num = PMP::connected_components(pm, fccmap);
  py::dict dct;
  for (auto f : CGAL::faces(pm)) dct[py::cast(f)] = py::cast(get(fccmap, f));
  py::list lst;
  lst.append(num);
  lst.append(std::move(dct));
  return lst;
}

//
template <typename PolygonMesh>
PolygonMesh corefine_and_compute_union(PolygonMesh& pm1, PolygonMesh& pm2) {
  using Pm = PolygonMesh;

  Pm out;
  Visitor<Pm> visitor;

  bool valid =
    PMP::corefine_and_compute_union(pm1, pm2, out, PARMS::visitor(visitor));
  if (! valid) throw std::runtime_error("Cannot compute union!");
  return out;
}

}

// Export Polygon_mesh_processing
void export_polygon_mesh_processing(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;

  m.def("connected_component", &pmp::connected_component<Pm>,
        py::arg("seed_face"), py::arg("pm"),
        py::arg("parameters") = py::dict());
  m.def("connected_components", &pmp::connected_components<Pm>,
        py::arg("pm"), py::arg("parameters") = py::dict());

  // corefine
  m.def("corefine_and_compute_union", &pmp::corefine_and_compute_union<Pm>);
}
