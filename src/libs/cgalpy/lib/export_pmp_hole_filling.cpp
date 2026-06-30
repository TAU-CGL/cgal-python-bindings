// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com>
//            Efi Fogel          <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#include <array>
#include <functional>
#include <vector>
#include <utility>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/vector.h>

#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

#include "cgalpy/Named_parameter_density_control_factor.hpp"
#include "cgalpy/Named_parameter_do_not_use_cubic_algorithm.hpp"
#include "cgalpy/Named_parameter_fairing_continuity.hpp"
#include "cgalpy/Named_parameter_geom_traits.hpp"
#include "cgalpy/Named_parameter_vertex_point_map.hpp"
#include "cgalpy/Named_parameter_threshold_distance.hpp"
#include "cgalpy/Named_parameter_use_2d_constrained_delaunay_triangulation.hpp"
#include "cgalpy/Named_parameter_use_delaunay_triangulation.hpp"
#include "cgalpy/Named_parameter_wrapper.hpp"
#include "cgalpy/named_parameter_applicator.hpp"
#include "cgalpy/HFDefault_visitor.hpp"
#include "cgalpy/polygon_mesh_processing_types.hpp"
#include "cgalpy/ndarray_to_point_3_vector.hpp"
#include "cgalpy/Pmp_docstrings.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace pmp_doc = cgalpy::pmp::docstrings;

namespace cgalpy {
namespace pmp {

using Point_3_vec = std::vector<Point_3>;

//! Apply simple triangulation named parameters.
template <template <typename...> class Wrapper, typename... Args>
auto apply_hole_triangulation_named_parameters(const py::dict& params,
                                               Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_use_delaunay_triangulation use_delaunay_op;
  cgalpy::Named_parameter_use_2d_constrained_delaunay_triangulation
    use_2d_cdt_op;
  cgalpy::Named_parameter_threshold_distance threshold_distance_op;
  cgalpy::Named_parameter_do_not_use_cubic_algorithm do_not_use_cubic_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator
    (wrapper, np, params, geom_traits_op, use_delaunay_op, use_2d_cdt_op,
     threshold_distance_op, do_not_use_cubic_op);
}

//! Apply simple triangulation/refinement named parameters.
template <template <typename...> class Wrapper, typename... Args>
auto apply_hole_refinement_named_parameters(const py::dict& params,
                                            Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_use_delaunay_triangulation use_delaunay_op;
  cgalpy::Named_parameter_use_2d_constrained_delaunay_triangulation
    use_2d_cdt_op;
  cgalpy::Named_parameter_threshold_distance threshold_distance_op;
  cgalpy::Named_parameter_do_not_use_cubic_algorithm do_not_use_cubic_op;
  cgalpy::Named_parameter_density_control_factor density_control_factor_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator
    (wrapper, np, params, geom_traits_op, use_delaunay_op, use_2d_cdt_op,
     threshold_distance_op, do_not_use_cubic_op, density_control_factor_op);
}

//! Apply simple triangulation/refinement/fairing named parameters.
template <template <typename...> class Wrapper, typename... Args>
auto apply_hole_fairing_named_parameters(const py::dict& params,
                                         Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_use_delaunay_triangulation use_delaunay_op;
  cgalpy::Named_parameter_use_2d_constrained_delaunay_triangulation
    use_2d_cdt_op;
  cgalpy::Named_parameter_threshold_distance threshold_distance_op;
  cgalpy::Named_parameter_do_not_use_cubic_algorithm do_not_use_cubic_op;
  cgalpy::Named_parameter_density_control_factor density_control_factor_op;
  cgalpy::Named_parameter_fairing_continuity fairing_continuity_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator
    (wrapper, np, params, geom_traits_op, use_delaunay_op, use_2d_cdt_op,
     threshold_distance_op, do_not_use_cubic_op, density_control_factor_op,
     fairing_continuity_op);
}

//! Apply simple triangulation named parameters, starting with a visitor.
template <template <typename...> class Wrapper, typename Visitor,
          typename... Args>
auto apply_hole_triangulation_named_parameters_with_visitor
(const py::dict& params, Visitor& visitor, Args&&... args)
{
  auto np = CGAL::parameters::default_values().visitor(visitor);
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_use_delaunay_triangulation use_delaunay_op;
  cgalpy::Named_parameter_use_2d_constrained_delaunay_triangulation
    use_2d_cdt_op;
  cgalpy::Named_parameter_threshold_distance threshold_distance_op;
  cgalpy::Named_parameter_do_not_use_cubic_algorithm do_not_use_cubic_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator
    (wrapper, np, params, geom_traits_op, use_delaunay_op, use_2d_cdt_op,
     threshold_distance_op, do_not_use_cubic_op);
}

//! Apply mesh triangulation named parameters, starting with a visitor.
template <typename PolygonMesh, template <typename...> class Wrapper,
          typename Visitor, typename... Args>
auto apply_hole_mesh_triangulation_named_parameters_with_visitor
(const py::dict& params, Visitor& visitor, Args&&... args)
{
  auto np = CGAL::parameters::default_values().visitor(visitor);
  cgalpy::Named_parameter_vertex_point_map<PolygonMesh> vertex_point_map_op;
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_use_delaunay_triangulation use_delaunay_op;
  cgalpy::Named_parameter_use_2d_constrained_delaunay_triangulation
    use_2d_cdt_op;
  cgalpy::Named_parameter_threshold_distance threshold_distance_op;
  cgalpy::Named_parameter_do_not_use_cubic_algorithm do_not_use_cubic_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator
    (wrapper, np, params, vertex_point_map_op, geom_traits_op,
     use_delaunay_op, use_2d_cdt_op, threshold_distance_op,
     do_not_use_cubic_op);
}

//! Apply mesh triangulation/refinement named parameters, starting with a visitor.
template <typename PolygonMesh, template <typename...> class Wrapper,
          typename Visitor, typename... Args>
auto apply_hole_mesh_refinement_named_parameters_with_visitor
(const py::dict& params, Visitor& visitor, Args&&... args)
{
  auto np = CGAL::parameters::default_values().visitor(visitor);
  cgalpy::Named_parameter_vertex_point_map<PolygonMesh> vertex_point_map_op;
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_use_delaunay_triangulation use_delaunay_op;
  cgalpy::Named_parameter_use_2d_constrained_delaunay_triangulation
    use_2d_cdt_op;
  cgalpy::Named_parameter_threshold_distance threshold_distance_op;
  cgalpy::Named_parameter_do_not_use_cubic_algorithm do_not_use_cubic_op;
  cgalpy::Named_parameter_density_control_factor density_control_factor_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator
    (wrapper, np, params, vertex_point_map_op, geom_traits_op,
     use_delaunay_op, use_2d_cdt_op, threshold_distance_op,
     do_not_use_cubic_op, density_control_factor_op);
}

//! Apply mesh triangulation/refinement/fairing named parameters, starting with a visitor.
template <typename PolygonMesh, template <typename...> class Wrapper,
          typename Visitor, typename... Args>
auto apply_hole_mesh_fairing_named_parameters_with_visitor
(const py::dict& params, Visitor& visitor, Args&&... args)
{
  auto np = CGAL::parameters::default_values().visitor(visitor);
  cgalpy::Named_parameter_vertex_point_map<PolygonMesh> vertex_point_map_op;
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_use_delaunay_triangulation use_delaunay_op;
  cgalpy::Named_parameter_use_2d_constrained_delaunay_triangulation
    use_2d_cdt_op;
  cgalpy::Named_parameter_threshold_distance threshold_distance_op;
  cgalpy::Named_parameter_do_not_use_cubic_algorithm do_not_use_cubic_op;
  cgalpy::Named_parameter_density_control_factor density_control_factor_op;
  cgalpy::Named_parameter_fairing_continuity fairing_continuity_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator
    (wrapper, np, params, vertex_point_map_op, geom_traits_op,
     use_delaunay_op, use_2d_cdt_op, threshold_distance_op,
     do_not_use_cubic_op, density_control_factor_op,
     fairing_continuity_op);
}

//! Apply mesh triangulation named parameters.
template <typename PolygonMesh, template <typename...> class Wrapper,
          typename... Args>
auto apply_hole_mesh_triangulation_named_parameters(const py::dict& params,
                                                    Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_vertex_point_map<PolygonMesh> vertex_point_map_op;
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_use_delaunay_triangulation use_delaunay_op;
  cgalpy::Named_parameter_use_2d_constrained_delaunay_triangulation
    use_2d_cdt_op;
  cgalpy::Named_parameter_threshold_distance threshold_distance_op;
  cgalpy::Named_parameter_do_not_use_cubic_algorithm do_not_use_cubic_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator
    (wrapper, np, params, vertex_point_map_op, geom_traits_op,
     use_delaunay_op, use_2d_cdt_op, threshold_distance_op,
     do_not_use_cubic_op);
}

//! Apply mesh triangulation/refinement named parameters.
template <typename PolygonMesh, template <typename...> class Wrapper,
          typename... Args>
auto apply_hole_mesh_refinement_named_parameters(const py::dict& params,
                                                 Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_vertex_point_map<PolygonMesh> vertex_point_map_op;
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_use_delaunay_triangulation use_delaunay_op;
  cgalpy::Named_parameter_use_2d_constrained_delaunay_triangulation
    use_2d_cdt_op;
  cgalpy::Named_parameter_threshold_distance threshold_distance_op;
  cgalpy::Named_parameter_do_not_use_cubic_algorithm do_not_use_cubic_op;
  cgalpy::Named_parameter_density_control_factor density_control_factor_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator
    (wrapper, np, params, vertex_point_map_op, geom_traits_op,
     use_delaunay_op, use_2d_cdt_op, threshold_distance_op,
     do_not_use_cubic_op, density_control_factor_op);
}

//! Apply mesh triangulation/refinement/fairing named parameters.
template <typename PolygonMesh, template <typename...> class Wrapper,
          typename... Args>
auto apply_hole_mesh_fairing_named_parameters(const py::dict& params,
                                              Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_vertex_point_map<PolygonMesh> vertex_point_map_op;
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_use_delaunay_triangulation use_delaunay_op;
  cgalpy::Named_parameter_use_2d_constrained_delaunay_triangulation
    use_2d_cdt_op;
  cgalpy::Named_parameter_threshold_distance threshold_distance_op;
  cgalpy::Named_parameter_do_not_use_cubic_algorithm do_not_use_cubic_op;
  cgalpy::Named_parameter_density_control_factor density_control_factor_op;
  cgalpy::Named_parameter_fairing_continuity fairing_continuity_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator
    (wrapper, np, params, vertex_point_map_op, geom_traits_op,
     use_delaunay_op, use_2d_cdt_op, threshold_distance_op,
     do_not_use_cubic_op, density_control_factor_op,
     fairing_continuity_op);
}

//! Wrap CGAL::Polygon_mesh_processing::triangulate_and_refine_hole().
template <typename NamedParameter, typename... Args>
struct Triangulate_and_refine_hole_wrapper;

template <typename NamedParameter, typename PolygonMesh,
          typename BorderHalfedge>
struct Triangulate_and_refine_hole_wrapper<NamedParameter, PolygonMesh,
                                           BorderHalfedge> {
  static auto call(NamedParameter& np, PolygonMesh&& pmesh,
                   BorderHalfedge&& border_halfedge)
  {
    return PMP::triangulate_and_refine_hole
      (std::forward<PolygonMesh>(pmesh),
       std::forward<BorderHalfedge>(border_halfedge), np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::triangulate_hole().
template <typename NamedParameter, typename... Args>
struct Triangulate_hole_wrapper;

template <typename NamedParameter, typename PolygonMesh,
          typename BorderHalfedge>
struct Triangulate_hole_wrapper<NamedParameter, PolygonMesh,
                                BorderHalfedge> {
  static auto call(NamedParameter& np, PolygonMesh&& pmesh,
                   BorderHalfedge&& border_halfedge)
  {
    return PMP::triangulate_hole(std::forward<PolygonMesh>(pmesh),
                                 std::forward<BorderHalfedge>(border_halfedge),
                                 np);
  }
};

//! Wrap the one-polyline CGAL::Polygon_mesh_processing::triangulate_hole_polyline().
template <typename NamedParameter, typename... Args>
struct Triangulate_hole_polyline_wrapper;

template <typename NamedParameter, typename PointRange,
          typename OutputIterator>
struct Triangulate_hole_polyline_wrapper<NamedParameter, PointRange,
                                         OutputIterator> {
  static auto call(NamedParameter& np, PointRange&& polyline,
                   OutputIterator&& out)
  {
    return PMP::triangulate_hole_polyline
      (std::forward<PointRange>(polyline), std::forward<OutputIterator>(out),
       np);
  }
};

//! Wrap the two-polyline CGAL::Polygon_mesh_processing::triangulate_hole_polyline().
template <typename NamedParameter, typename... Args>
struct Triangulate_hole_polyline_2_wrapper;

template <typename NamedParameter, typename PointRange1,
          typename PointRange2, typename OutputIterator>
struct Triangulate_hole_polyline_2_wrapper<NamedParameter, PointRange1,
                                           PointRange2, OutputIterator> {
  static auto call(NamedParameter& np, PointRange1&& polyline1,
                   PointRange2&& polyline2, OutputIterator&& out)
  {
    return PMP::triangulate_hole_polyline
      (std::forward<PointRange1>(polyline1),
       std::forward<PointRange2>(polyline2),
       std::forward<OutputIterator>(out), np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::triangulate_refine_and_fair_hole().
template <typename NamedParameter, typename... Args>
struct Triangulate_refine_and_fair_hole_wrapper;

template <typename NamedParameter, typename PolygonMesh,
          typename BorderHalfedge>
struct Triangulate_refine_and_fair_hole_wrapper<NamedParameter, PolygonMesh,
                                                BorderHalfedge> {
  static auto call(NamedParameter& np, PolygonMesh&& pmesh,
                   BorderHalfedge&& border_halfedge)
  {
    return PMP::triangulate_refine_and_fair_hole
      (std::forward<PolygonMesh>(pmesh),
       std::forward<BorderHalfedge>(border_halfedge), np);
  }
};

// HFDefault_visitor
void set_start_planar_phase(HFDefault_visitor& v, const py::object& f)
{ v.set_start_planar_phase(f); }

//!
void set_end_planar_phase(HFDefault_visitor& v, const py::object& f)
{ v.set_end_planar_phase(f); }

//!
void set_start_quadratic_phase(HFDefault_visitor& v, const py::object& f)
{ v.set_start_quadratic_phase(f); }

//!
void set_quadratic_step(HFDefault_visitor& v, const py::object& f)
{ v.set_quadratic_step(f); }

//!
void set_end_quadratic_phase(HFDefault_visitor& v, const py::object& f)
{ v.set_end_quadratic_phase(f); }

//!
void set_start_cubic_phase(HFDefault_visitor& v, const py::object& f)
{ v.set_start_cubic_phase(f); }

//!
void set_cubic_step(HFDefault_visitor& v, const py::object& f)
{ v.set_cubic_step(f); }

//!
void set_end_cubic_phase(HFDefault_visitor& v, const py::object& f)
{ v.set_end_cubic_phase(f); }

//!
void set_start_refine_phase(HFDefault_visitor& v, const py::object& f)
{ v.set_start_refine_phase(f); }

//!
void set_end_refine_phase(HFDefault_visitor& v, const py::object& f)
{ v.set_end_refine_phase(f); }

//!
void set_start_fair_phase(HFDefault_visitor& v, const py::object& f)
{ v.set_start_fair_phase(f); }

//!
void set_end_fair_phase(HFDefault_visitor& v, const py::object& f)
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
      apply_hole_mesh_refinement_named_parameters_with_visitor
        <Pm, Triangulate_and_refine_hole_wrapper>(np, visitor, pmesh,
                                                  border_halfedge);
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    apply_hole_mesh_refinement_named_parameters
      <Pm, Triangulate_and_refine_hole_wrapper>(np, pmesh, border_halfedge);
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
      apply_hole_mesh_triangulation_named_parameters_with_visitor
        <Pm, Triangulate_hole_wrapper>(np, visitor, pmesh, border_halfedge);
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    apply_hole_mesh_triangulation_named_parameters
      <Pm, Triangulate_hole_wrapper>(np, pmesh, border_halfedge);
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
      apply_hole_triangulation_named_parameters_with_visitor
        <Triangulate_hole_polyline_2_wrapper>(np, visitor, polyline1,
                                              polyline2,
                                              std::back_inserter(out));
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    apply_hole_triangulation_named_parameters
      <Triangulate_hole_polyline_2_wrapper>(np, polyline1, polyline2,
                                            std::back_inserter(out));
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
      apply_hole_triangulation_named_parameters_with_visitor
        <Triangulate_hole_polyline_wrapper>(np, visitor, polyline,
                                            std::back_inserter(out));
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    apply_hole_triangulation_named_parameters
      <Triangulate_hole_polyline_wrapper>(np, polyline,
                                          std::back_inserter(out));
  }
  std::array<int, 3> out2 = {out[0].first, out[0].second, out[0].third};
  return out2;
}

//!
template <typename PolygonMesh>
auto triangulate_hole_polyline_np(const py::ndarray<>& polyline_array,
                                  const py::dict& np = py::dict()) {
  auto polyline =
    cgalpy::ndarray_to_point_3_vector<Point_3>(polyline_array, "points");
  return triangulate_hole_polyline<PolygonMesh>(polyline, np);
}

//!
template <typename PolygonMesh>
auto triangulate_hole_polyline_2_np_list(const py::ndarray<>& polyline1_array,
                                         const Point_3_vec& polyline2,
                                         const py::dict& np = py::dict()) {
  auto polyline1 =
    cgalpy::ndarray_to_point_3_vector<Point_3>(polyline1_array, "points");
  return triangulate_hole_polyline_2<PolygonMesh>(polyline1, polyline2, np);
}

//!
template <typename PolygonMesh>
auto triangulate_hole_polyline_2_list_np(const Point_3_vec& polyline1,
                                         const py::ndarray<>& polyline2_array,
                                         const py::dict& np = py::dict()) {
  auto polyline2 =
    cgalpy::ndarray_to_point_3_vector<Point_3>(polyline2_array, "third_points");
  return triangulate_hole_polyline_2<PolygonMesh>(polyline1, polyline2, np);
}

//!
template <typename PolygonMesh>
auto triangulate_hole_polyline_2_np_np(const py::ndarray<>& polyline1_array,
                                       const py::ndarray<>& polyline2_array,
                                       const py::dict& np = py::dict()) {
  auto polyline1 =
    cgalpy::ndarray_to_point_3_vector<Point_3>(polyline1_array, "points");
  auto polyline2 =
    cgalpy::ndarray_to_point_3_vector<Point_3>(polyline2_array, "third_points");
  return triangulate_hole_polyline_2<PolygonMesh>(polyline1, polyline2, np);
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
    auto res = apply_hole_mesh_fairing_named_parameters_with_visitor
      <Pm, Triangulate_refine_and_fair_hole_wrapper>(np, visitor, pmesh,
                                                     border_halfedge);
    return py::make_tuple(std::get<0>(res), fids, vids);
  }
  else {
    auto res = apply_hole_mesh_fairing_named_parameters
      <Pm, Triangulate_refine_and_fair_hole_wrapper>(np, pmesh,
                                                     border_halfedge);
    return py::make_tuple(std::get<0>(res), fids, vids);
  }
}

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH

//! Convert a Polyhedron halfedge object to a halfedge descriptor and
//! triangulate a hole.
template <typename PolygonMesh>
auto triangulate_hole_halfedge(PolygonMesh& pmesh,
                               typename PolygonMesh::Halfedge& border_halfedge,
                               const py::dict& np = py::dict())
{
  using Hd = typename boost::graph_traits<PolygonMesh>::halfedge_descriptor;
  auto descriptor = Hd(&border_halfedge);
  return triangulate_hole<PolygonMesh>(pmesh, descriptor, np);
}

//! Convert a Polyhedron halfedge object to a halfedge descriptor and
//! triangulate/refine a hole.
template <typename PolygonMesh>
auto triangulate_and_refine_hole_halfedge
(PolygonMesh& pmesh, typename PolygonMesh::Halfedge& border_halfedge,
 const py::dict& np = py::dict())
{
  using Hd = typename boost::graph_traits<PolygonMesh>::halfedge_descriptor;
  auto descriptor = Hd(&border_halfedge);
  return triangulate_and_refine_hole<PolygonMesh>(pmesh, descriptor, np);
}

//! Convert a Polyhedron halfedge object to a halfedge descriptor and
//! triangulate/refine/fair a hole.
template <typename PolygonMesh>
auto triangulate_refine_and_fair_hole_halfedge
(PolygonMesh& pmesh, typename PolygonMesh::Halfedge& border_halfedge,
 const py::dict& np = py::dict())
{
  using Hd = typename boost::graph_traits<PolygonMesh>::halfedge_descriptor;
  auto descriptor = Hd(&border_halfedge);
  return triangulate_refine_and_fair_hole<PolygonMesh>(pmesh, descriptor, np);
}

#endif

}
} // namespace cgalpy

//!
void export_pmp_hole_filling(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;

  using Hfv = cgalpy::pmp::HFDefault_visitor;
  static PyType_Slot hole_filling_visitor_slots[] = {
    {Py_tp_traverse, (void*) Hfv::tp_traverse},
    {Py_tp_clear, (void*) Hfv::tp_clear},
    {0, nullptr}
  };
  py::class_<Hfv>(m, "Hole_filling_default_visitor",
                  pmp_doc::PMPHolefillingVisitor_class,
                  py::type_slots(hole_filling_visitor_slots))
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
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("triangulate_and_refine_hole",
        &cgalpy::pmp::triangulate_and_refine_hole_halfedge<Pm>,
        py::arg("pm"), py::arg("hole_boundary"), py::arg("np") = py::dict(),
        "Triangulates and refines a hole of a polygon mesh.");
  m.def("triangulate_hole", &cgalpy::pmp::triangulate_hole_halfedge<Pm>,
        py::arg("pmesh"), py::arg("border_halfedge"), py::arg("np") = py::dict(),
        "Triangulates a hole of a polygon mesh.");
#else
  m.def("triangulate_and_refine_hole", &cgalpy::pmp::triangulate_and_refine_hole<Pm>,
        py::arg("pm"), py::arg("hole_boundary"), py::arg("np") = py::dict(),
        "Triangulates and refines a hole of a polygon mesh.");
  m.def("triangulate_hole", &cgalpy::pmp::triangulate_hole<Pm>,
        py::arg("pmesh"), py::arg("border_halfedge"), py::arg("np") = py::dict(),
        "Triangulates a hole of a polygon mesh.");
#endif
  m.def("triangulate_hole_polyline", &cgalpy::pmp::triangulate_hole_polyline<Pm>,
        py::arg("points"), py::arg("np") = py::dict(),
        "Triangulates a hole bounded by a polyline.");
  m.def("triangulate_hole_polyline", &cgalpy::pmp::triangulate_hole_polyline_np<Pm>,
        py::arg("points"), py::arg("np") = py::dict(),
        "Triangulates a hole bounded by a polyline.");
  m.def("triangulate_hole_polyline", &cgalpy::pmp::triangulate_hole_polyline_2<Pm>,
        py::arg("points"), py::arg("third_points"), py::arg("np") = py::dict(),
        "Triangulates a hole bounded by two polylines.");
  m.def("triangulate_hole_polyline", &cgalpy::pmp::triangulate_hole_polyline_2_np_list<Pm>,
        py::arg("points"), py::arg("third_points"), py::arg("np") = py::dict(),
        "Triangulates a hole bounded by two polylines.");
  m.def("triangulate_hole_polyline", &cgalpy::pmp::triangulate_hole_polyline_2_list_np<Pm>,
        py::arg("points"), py::arg("third_points"), py::arg("np") = py::dict(),
        "Triangulates a hole bounded by two polylines.");
  m.def("triangulate_hole_polyline", &cgalpy::pmp::triangulate_hole_polyline_2_np_np<Pm>,
        py::arg("points"), py::arg("third_points"), py::arg("np") = py::dict(),
        "Triangulates a hole bounded by two polylines.");
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("triangulate_refine_and_fair_hole",
        &cgalpy::pmp::triangulate_refine_and_fair_hole_halfedge<Pm>,
        py::arg("pmesh"), py::arg("border_halfedge"), py::arg("np") = py::dict(),
        "Triangulates, refines, and fairs a hole of a polygon mesh.");
#else
  m.def("triangulate_refine_and_fair_hole", &cgalpy::pmp::triangulate_refine_and_fair_hole<Pm>,
        py::arg("pmesh"), py::arg("border_halfedge"), py::arg("np") = py::dict(),
        "Triangulates, refines, and fairs a hole of a polygon mesh.");
#endif
}
