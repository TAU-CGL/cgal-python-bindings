// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <boost/property_map/vector_property_map.hpp>
#define CGAL_USE_BASIC_VIEWER

#include <string>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/boost/graph/generators.h>
#include <CGAL/property_map.h>
#include <CGAL/Dynamic_property_map.h>
#include <CGAL/IO/polygon_soup_io.h>
#include <CGAL/boost/graph/Face_filtered_graph.h>
#include <CGAL/boost/graph/helpers.h>
#ifdef CGALPY_HAS_VISUAL
#include <CGAL/draw_surface_mesh.h>
#endif

#include "CGALPY/add_attr.hpp"
#include "CGALPY/Property_map.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/surface_mesh_types.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/export_boost_mesh_utils.hpp"
#include "CGALPY/export_mesh_iterators.hpp"
#include "CGALPY/get.hpp"
#include "CGALPY/internal.hpp"

namespace py = nanobind;

namespace sm {

template <typename Map_type>
void register_map(py::module_& m, const std::string& map_name) {
  py::class_<Map_type>(m, map_name.c_str())
    .def(py::init<>())
    .def_ro("map_", &Map_type::map_)
    ;
}

template <typename Dp, typename Mesh>
void register_map_get(py::module_& m, const std::string& prop_name) {
  py::class_<Dp> prop(m, prop_name.c_str());
  prop.def(py::init<>());
  m.def("get", &internal::get<Dp, Mesh>,
        py::arg("property_map"), py::arg("sm"));
}

template <typename Pm, typename P>
void edge_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Ed = typename boost::graph_traits<Pm>::edge_descriptor;
  using dp = CGAL::dynamic_edge_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const map_type& p, const Ed& e) { return get(p, e); },
        py::arg("property_map"), py::arg("edge_descriptor"));
}

template <typename Pm, typename P>
void face_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Fd = typename boost::graph_traits<Pm>::face_descriptor;
  using dp = CGAL::dynamic_face_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const map_type& p, const Fd& f) { return get(p, f); },
        py::arg("property_map"), py::arg("face_descriptor"));
}

template <typename Pm, typename P>
void vertex_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Vd = typename boost::graph_traits<Pm>::vertex_descriptor;
  using dp = CGAL::dynamic_vertex_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const map_type& p, const Vd& v) { return get(p, v); },
        py::arg("property_map"), py::arg("vertex_descriptor"));
}

template <typename Pm, typename P>
void halfedge_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Hd = typename boost::graph_traits<Pm>::halfedge_descriptor;
  using dp = CGAL::dynamic_halfedge_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const map_type& p, const Hd& h) { return get(p, h); },
        py::arg("property_map"), py::arg("halfedge_descriptor"));
}

//
template <typename SurfaceMesh>
SurfaceMesh make_tetrahedron(const typename SurfaceMesh::Point& p1,
                             const typename SurfaceMesh::Point& p2,
                             const typename SurfaceMesh::Point& p3,
                             const typename SurfaceMesh::Point& p4) {
  using Sm = SurfaceMesh;

  Sm sm;
  CGAL::make_tetrahedron(p1, p2, p3, p4, sm);
  return sm;
}

// Read a surface mesh from a file.
template <typename SurfaceMesh>
SurfaceMesh read_polygon_mesh(const std::string& filename,
                              const py::dict& parameters = py::dict()) {
  using Sm = SurfaceMesh;

  Sm sm;
  if (! CGAL::IO::read_polygon_mesh(filename, sm,
                                    internal::parse_named_parameters(parameters)))
    throw std::runtime_error("Cannot read file!");
  return sm;
}

// Read Polygon soup from a file
template <typename SurfaceMesh>
auto read_polygon_soup(const std::string& fname,
                              const py::dict& np = py::dict()) {
  std::vector<Point_3> points;
  std::vector<std::vector<std::size_t> > polygons;

  if (! CGAL::IO::read_polygon_soup(fname, points, polygons))
    throw std::runtime_error("Cannot read file!");

  py::list pnt_lst, polygons_lst;
  for (auto p : points) {
    pnt_lst.append(p);
  }
  for (auto poly : polygons) {
    py::list new_poly;
    for (auto pt : poly) {
      new_poly.append(pt);
    }
    polygons_lst.append(new_poly);
  }

  return py::make_tuple(pnt_lst, polygons_lst);
}

//
template <typename SurfaceMesh>
bool write_polygon_mesh(std::string fname, const SurfaceMesh& pm,
                        const py::dict& parameters = py::dict()) {
  return CGAL::IO::write_polygon_mesh(fname, pm,
                                      internal::parse_named_parameters(parameters));
}

// Draw a surface mesh.
#ifdef CGALPY_HAS_VISUAL
template <typename SurfaceMesh>
void draw(const SurfaceMesh& sm) { CGAL::draw(sm); }
#endif

// Obtain the null face.
template <typename SurfaceMesh>
static typename SurfaceMesh::Face_index null_face()
{ return SurfaceMesh::null_face(); }

// Add a face from a list of vertices.
template <typename SurfaceMesh>
typename SurfaceMesh::Face_index add_face(SurfaceMesh& sm, py::list& lst) {
  using Sm = SurfaceMesh;
  using Vi = typename Sm::Vertex_index;

  auto begin = stl_input_iterator<Vi>(lst);
  auto end = stl_input_iterator<Vi>(lst, false);
  return sm.add_face(CGAL::make_range(begin, end));
}

//
template <typename SurfaceMesh>
typename SurfaceMesh::Halfedge_index halfedge(const typename SurfaceMesh::Face_index& f,
                                              const SurfaceMesh& sm)
{ return CGAL::halfedge(f, sm); }

template <typename SurfaceMesh>
typename SurfaceMesh::Point my_point(const SurfaceMesh& sm,
                                     const typename SurfaceMesh::Vertex_index& v)
{ return sm.point(v); }

template <typename SurfaceMesh, typename Vi, typename Pnt>
auto points(const SurfaceMesh& sm) {
  return sm.points();
}

//
template <typename SurfaceMesh>
bool is_triangle(const typename SurfaceMesh::Halfedge_index& h, const SurfaceMesh& sm)
{ return CGAL::is_triangle(h, sm); }


//
template <typename SurfaceMesh>
auto has_valid_index_v(const SurfaceMesh& sm, typename SurfaceMesh::Vertex_index& vi)
{ return sm.is_valid(vi); }

//
template <typename SurfaceMesh>
auto has_valid_index_e(const SurfaceMesh& sm, typename SurfaceMesh::Edge_index& ei)
{ return sm.is_valid(ei); }

//
template <typename SurfaceMesh>
auto has_valid_index_h(const SurfaceMesh& sm, typename SurfaceMesh::Halfedge_index& hi)
{ return sm.is_valid(hi); }

//
template <typename SurfaceMesh>
auto has_valid_index_f(const SurfaceMesh& sm, typename SurfaceMesh::Face_index& fi)
{ return sm.is_valid(fi); }

/// \name Iterators
/// @{

//
template <typename SurfaceMesh>
py::object vertices(const SurfaceMesh& sm)
{ return make_iterator(sm.vertices_begin(), sm.vertices_end()); }

//
template <typename SurfaceMesh>
py::object halfedges(const SurfaceMesh& sm)
{ return make_iterator(sm.halfedges_begin(), sm.halfedges_end()); }

//
template <typename SurfaceMesh>
py::object edges(const SurfaceMesh& sm)
{ return make_iterator(sm.edges_begin(), sm.edges_end()); }

//
template <typename SurfaceMesh>
py::object faces(const SurfaceMesh& sm)
{ return make_iterator(sm.faces_begin(), sm.faces_end()); }

/// @}

// property maps
template <typename SurfaceMesh, typename Key, typename Value>
auto add_map(SurfaceMesh& sm, const std::string& name = std::string(),
             const Value& default_value = Value()) {
  using Sm = SurfaceMesh;
  using Vi = typename Sm::Vertex_index;
  using Pnt = typename Sm::Point;
  using Map = typename Sm::template Property_map<Vi, Pnt>;
  // Surface_mesh::Property_map<edge_descriptor, bool> is_constrained_map =
  //   mesh2.add_property_map<edge_descriptor, bool>("e:is_constrained", true).first;

  auto res = sm.template add_property_map<Key, Value>(name, default_value);
  return py::make_tuple(res.first, res.second);
}

template <typename T>
auto make_random_access_property_map(const py::list& lst) {
  std::vector<T> vec;
  for (auto v : lst) {
    vec.push_back(py::cast<T>(v));
  }
  return CGAL::make_random_access_property_map(vec);
}


} // namespace sm

//
template <typename SurfaceMesh, typename C>
void add_sm_index(C& c) {
  using Sm = SurfaceMesh;
  using size_type = typename Sm::size_type;
  using Sm_i = typename C::Type;

  c.def(py::init<>())
    .def(py::init<size_type>())
    .def("id", &Sm_i::id)
    .def("idx", &Sm_i::idx)
    .def("is_valid", &Sm_i::is_valid)
    .def("reset", &Sm_i::reset)
    ;
}

// Export Surface_mesh.
template <typename SurfaceMesh>
void export_surface_mesh_impl(py::module_& m, const char* name) {
  using Sm = SurfaceMesh;
  using Pnt = typename Sm::Point;
  using size_type = typename Sm::size_type;
  using Vi = typename Sm::Vertex_index;
  using Ei = typename Sm::Edge_index;
  using Hi = typename Sm::Halfedge_index;
  using Fi = typename Sm::Face_index;
  using Gt = boost::graph_traits<Sm>;
  using Fd = typename Gt::face_descriptor;
  using Hd = typename Gt::halfedge_descriptor;
  using Vd = typename Gt::vertex_descriptor;

  constexpr auto ri(py::rv_policy::reference_internal);

  // Vertex_index
  using Sm_vi = typename CGAL::SM_Index<Vi>;
  if (! add_attr<Sm_vi>(m, "SM_Index_vertex_index")) {
    py::class_<Sm_vi> sm_vi_c(m, "SM_vertex_index");
    add_sm_index<Sm>(sm_vi_c);
  }

  if (! add_attr<Vi>(m, "Vertex_index")) {
    py::class_<Vi, Sm_vi>(m, "Vertex_index")
      .def(py::init<>())
      .def(py::init<size_type>())
      .def("__str__", [](const Vi& smi){ return std::to_string(smi.idx()); })
      .def("__repr__", [](const Vi& smi){ return std::to_string(smi.idx()); })
      ;
  }

  // Edge_index
  if (! add_attr<Ei>(m, "Edge_index")) {
    py::class_<Ei>(m, "Edge_index")
      .def(py::init<>())
      .def(py::init<size_type>())
      .def("halfedge", &Ei::halfedge)
      .def("idx", &Ei::idx)
      .def("reset", &Ei::reset)
      .def("is_valid", &Ei::is_valid)
      .def("__str__", [](const Ei& smi){ return std::to_string(smi.idx()); })
      .def("__repr__", [](const Ei& smi){ return std::to_string(smi.idx()); })
      ;
  }

  // 

  // Halfedge_index
  using Sm_hi = typename CGAL::SM_Index<Hi>;
  if (! add_attr<Sm_hi>(m, "SM_halfedge_index")) {
    py::class_<Sm_hi> sm_hi_c(m, "SM_halfedge_index");
    add_sm_index<Sm>(sm_hi_c);
  }

  if (! add_attr<Hi>(m, "Halfedge_index")) {
    py::class_<Hi, Sm_hi>(m, "Halfedge_index")
      .def(py::init<>())
      .def(py::init<size_type>())
      .def("__str__", [](const Hi& smi){ return std::to_string(smi.idx()); })
      .def("__repr__", [](const Hi& smi){ return std::to_string(smi.idx()); })
      ;
  }

  // Face_index
  using Sm_fi = typename CGAL::SM_Index<Fi>;
  if (! add_attr<Sm_fi>(m, "SM_face_index")) {
    py::class_<Sm_fi> sm_fi_c(m, "SM_face_index");
    add_sm_index<Sm>(sm_fi_c);
  }

  if (! add_attr<Fi>(m, "Face_index")) {
    py::class_<Fi, Sm_fi>(m, "Face_index")
      .def(py::init<>())
      .def(py::init<size_type>())
      .def("__str__", [](const Fi& smi){ return std::to_string(smi.idx()); })
      .def("__repr__", [](const Fi& smi){ return std::to_string(smi.idx()); })
      ;
  }

  // Face descriptor
  if (! add_attr<Fd>(m, "Face_descriptor")) {
    py::class_<Fd>(m, "Face_descriptor")
      .def(py::init<>())
      .def(py::init<Fi>())
      .def("idx", &Fd::idx)
      .def("is_valid", &Fd::is_valid)
      .def("__str__", [](const Fd& fd){ return std::to_string(fd.idx()); })
      .def("__repr__", [](const Fd& fd){ return std::to_string(fd.idx()); })
      ;
  }

  // Halfedge descriptor
  if (! add_attr<Hd>(m, "Halfedge_descriptor")) {
    py::class_<Hd>(m, "Halfedge_descriptor")
      .def(py::init<>())
      .def(py::init<Hi>())
      .def("idx", &Hd::idx)
      .def("is_valid", &Hd::is_valid)
      .def("__str__", [](const Hd& hd){ return std::to_string(hd.idx()); })
      .def("__repr__", [](const Hd& hd){ return std::to_string(hd.idx()); })
      ;
  }

  // Vertex_descriptor
  if (! add_attr<Vd>(m, "Vertex_descriptor")) {
    py::class_<Vd>(m, "Vertex_descriptor")
      .def(py::init<>())
      .def(py::init<Vi>())
      .def("idx", &Vd::idx)
      .def("is_valid", &Vd::is_valid)
      .def("__str__", [](const Vd& vd){ return std::to_string(vd.idx()); })
      .def("__repr__", [](const Vd& vd){ return std::to_string(vd.idx()); })
      ;
  }

  // Surface mesh
  if (! add_attr<Sm>(m, name)) {

    py::class_<Sm> sm_c(m, name);
    sm_c.def(py::init<>())
      .def(py::init<const Sm&>())
      // .def("assign", &Sm::assign, ri)
      .def("add_vertex", py::overload_cast<>(&Sm::add_vertex))
      .def("add_vertex", py::overload_cast<const Pnt&>(&Sm::add_vertex))
      .def("add_edge", py::overload_cast<>(&Sm::add_edge))
      .def("add_edge", py::overload_cast<Vi, Vi>(&Sm::add_edge))
      .def("add_face", static_cast<Fi(Sm::*)()>(&Sm::add_face))
      .def("add_face", static_cast<Fi(Sm::*)(Vi, Vi, Vi)>(&Sm::add_face))
      .def("add_face", static_cast<Fi(Sm::*)(Vi, Vi, Vi, Vi)>(&Sm::add_face))
      .def("add_face", &sm::add_face<Sm>)

      .def("has_valid_index", &sm::has_valid_index_v<Sm>)
      .def("has_valid_index", &sm::has_valid_index_e<Sm>)
      .def("has_valid_index", &sm::has_valid_index_h<Sm>)
      .def("has_valid_index", &sm::has_valid_index_f<Sm>)

      .def("remove_vertex", &Sm::remove_vertex)
      .def("remove_edge", &Sm::remove_edge)
      .def("remove_face", &Sm::remove_face)
      .def("num_vertices", &Sm::num_vertices)
      .def("number_of_vertices", &Sm::number_of_vertices)
      .def("num_halfedges", &Sm::num_halfedges)
      .def("number_of_halfedges", &Sm::number_of_halfedges)
      .def("num_edges", &Sm::num_edges)
      .def("number_of_edges", &Sm::number_of_edges)
      .def("num_faces", &Sm::num_faces)
      .def("number_of_faces", &Sm::number_of_faces)
      .def("is_empty", &Sm::is_empty)
      .def("clear_without_removing_property_maps", &Sm::clear_without_removing_property_maps)
      .def("clear", &Sm::clear)
      // void reserve(size_type nvertices, size_type nedges, size_type nfaces )
      // void resize(size_type nvertices, size_type nedges, size_type nfaces )
      // join(const Surface_mesh& other)
      .def("edge", &Sm::edge)
      .def("face", &Sm::face)
      .def("join", &Sm::join)
      .def("next", &Sm::next)
      .def("prev", &Sm::prev)
      .def("resize", &Sm::resize)
      .def("set_target", &Sm::set_target)
      .def("has_garbage", &Sm::has_garbage)
      .def("is_isolated", &Sm::is_isolated)
      .def("halfedges_end", &Sm::halfedges_end)
      .def("set_next_only", &Sm::set_next_only)
      .def("set_prev_only", &Sm::set_prev_only)
      .def("shrink_to_fit", &Sm::shrink_to_fit)
      .def("set_recycle_garbage", &Sm::set_recycle_garbage)
      .def("does_recycle_garbage", &Sm::does_recycle_garbage)

      .def("number_of_removed_edges", &Sm::number_of_removed_edges)
      .def("number_of_removed_faces", &Sm::number_of_removed_faces)
      .def("number_of_removed_vertices", &Sm::number_of_removed_vertices)
      .def("number_of_removed_halfedges", &Sm::number_of_removed_halfedges)


      .def("null_vertex", &Sm::null_vertex)
      .def("null_halfedge", &Sm::null_halfedge)

      .def("next_around_source", &Sm::next_around_source)
      .def("prev_around_source", &Sm::prev_around_source)
      .def("next_around_target", &Sm::next_around_target)
      .def("prev_around_target", &Sm::prev_around_target)

      // .def("property_stats", &Sm::property_stats)

      .def("point", &sm::my_point<Sm>, ri)
      .def("points", &sm::points<Sm, Vi, Pnt>)

      .def("add_property_map_vertex_point", &sm::add_map<Sm, Vi, Pnt>,
           py::arg("name") = std::string(), py::arg("default_value") = Pnt())
      .def("add_property_map_vertex_bool", &sm::add_map<Sm, Vi, bool>,
           py::arg("name") = std::string(), py::arg("default_value") = bool())
      .def("add_property_map_vertex_size_t", &sm::add_map<Sm, Vi, std::size_t>,
           py::arg("name") = std::string(), py::arg("default_value") = std::size_t())
      .def("add_property_map_vertex_vector", &sm::add_map<Sm, Vi, Vector_3>,
           py::arg("name") = std::string(), py::arg("default_value") = Vector_3())
      .def("add_property_map_vertex_int", &sm::add_map<Sm, Vi, int>,
           py::arg("name") = std::string(), py::arg("default_value") = int())

      .def("add_property_map_edge_bool", &sm::add_map<Sm, Ei, bool>,
           py::arg("name") = std::string(), py::arg("default_value") = bool())

      .def("add_property_map_face_double", &sm::add_map<Sm, Fi, double>,
           py::arg("name") = std::string(), py::arg("default_value") = double())
      .def("add_property_map_face_vector", &sm::add_map<Sm, Fi, Vector_3>,
           py::arg("name") = std::string(), py::arg("default_value") = Vector_3())
      .def("add_property_map_face_size_t", &sm::add_map<Sm, Fi, std::size_t>,
           py::arg("name") = std::string(), py::arg("default_value") = std::size_t())

      .def("add_property_map_halfedge_size_t", &sm::add_map<Sm, Hi, std::size_t>,
           py::arg("name") = std::string(), py::arg("default_value") = std::size_t())


      .def("is_valid", py::overload_cast<bool>(&Sm::is_valid, py::const_))
#if CGAL_VERSION_NR >= 1050600000
      .def("is_valid", py::overload_cast<Vi, bool>(&Sm::is_valid, py::const_))
      .def("is_valid", py::overload_cast<Ei, bool>(&Sm::is_valid, py::const_))
      .def("is_valid", py::overload_cast<Hi, bool>(&Sm::is_valid, py::const_))
      .def("is_valid", py::overload_cast<Fi, bool>(&Sm::is_valid, py::const_))
#endif
      ;

    add_attr<Vi>(sm_c, "Vertex_index");
    add_attr<Ei>(sm_c, "Edge_index");
    add_attr<Hi>(sm_c, "Halfedge_index");
    add_attr<Fi>(sm_c, "Face_index");
    // add_attr<Halfedge>(sm_c, "Halfedge");

    using Vci = typename Sm::Vertex_iterator;
    using Hci = typename Sm::Halfedge_iterator;
    using Eci = typename Sm::Edge_iterator;
    using Fci = typename Sm::Face_iterator;

    add_iterator<Vci, Vci>("Vertex_iterator", sm_c);
    add_iterator<Hci, Hci>("Halfedge_iterator", sm_c);
    add_iterator<Eci, Eci>("Edge_iterator", sm_c);
    add_iterator<Fci, Fci>("Face_iterator", sm_c);

    sm_c.def("vertices", &sm::vertices<Sm>, py::keep_alive<0, 1>())
      .def("halfedges", &sm::halfedges<Sm>, py::keep_alive<0, 1>())
      .def("edges", &sm::edges<Sm>, py::keep_alive<0, 1>())
      .def("faces", &sm::faces<Sm>, py::keep_alive<0, 1>())
      ;

    add_insertion(sm_c, "__str__");
    add_insertion(sm_c, "__repr__");
  }

#ifdef CGALPY_HAS_VISUAL
  m.def("draw", &sm::draw<Sm>);
#endif
}

// Export Surface_mesh<Pnt>
void export_surface_mesh(py::module_& m) {
  // 2D Surface mesh
  // using Sm_2 = sm::Surface_mesh_2;
  // export_surface_mesh_impl<Sm_2>(m, "Surface_mesh_2");

  // 3D Surface mesh
  using Sm_3 = sm::Surface_mesh_3;
  using Vi = typename Sm_3::Vertex_index;
  using Fi = typename Sm_3::Face_index;
  using Hi = typename Sm_3::Halfedge_index;
  using Ei = typename Sm_3::Edge_index;
  using Pnt = Point_3;
  constexpr auto ri(py::rv_policy::reference_internal);

  export_surface_mesh_impl<Sm_3>(m, "Surface_mesh_3");

  py::class_<std::_Bit_reference>(m, "bit_reference")
    .def(py::init<>())
    ;

  internal::export_property_map<Sm_3, Vi, Pnt>(m, "Vertex_point_map"); //this is the Pm::Property_map
  sm::vertex_map<Sm_3, Pnt>(m, "vertex_point_boost_map", "Vertex_point_boost_map"); //this is the boost::property_map
  internal::export_property_map<Sm_3, Vi, bool>(m, "Vertex_bool_map");
  sm::vertex_map<Sm_3, bool>(m, "vertex_bool_boost_map", "Vertex_bool_boost_map");
  internal::export_property_map<Sm_3, Vi, std::size_t>(m, "Vertex_size_t_map");
  sm::vertex_map<Sm_3, std::size_t>(m, "vertex_size_t_boost_map", "Vertex_size_t_boost_map");
  internal::export_property_map<Sm_3, Vi, Vector_3>(m, "Vertex_vector_map");
  sm::vertex_map<Sm_3, Vector_3>(m, "vertex_vector_boost_map", "Vertex_vector_boost_map");
  internal::export_property_map<Sm_3, Vi, int>(m, "Vertex_int_map");
  sm::vertex_map<Sm_3, int>(m, "vertex_int_boost_map", "Vertex_int_boost_map");


  internal::export_property_map<Sm_3, Ei, bool>(m, "Edge_bool_map");
  sm::edge_map<Sm_3, bool>(m, "edge_bool_boost_map", "Edge_bool_boost_map");
  m.def("get", [](const Sm_3::Property_map<Ei, bool>& p, const Ei& e) { return bool(get(p, e)); }, //this overrides get
        py::arg("property_map"), py::arg("edge_descriptor"));

  internal::export_property_map<Sm_3, Fi, double>(m, "Face_double_map");
  internal::export_property_map<Sm_3, Fi, Vector_3>(m, "Face_vector_map");
  internal::export_property_map<Sm_3, Fi, std::size_t>(m, "Face_size_t_map");
  sm::face_map<Sm_3, std::size_t>(m, "face_size_t_boost_map", "Face_size_t_boost_map");

  internal::export_property_map<Sm_3, Hi, std::size_t>(m, "Halfedge_size_t_map");

  // internal::export_property_map<Sm_3, Pnt, Pnt>(m, "Point_point_map"); // this is a dict not a map??

  // implemented:
  // vertex_point_map
  // edge_is_constrained_map
  // face_index_map
  // point_map
  // vertex_is_constrained_map
  // face_patch_map
  // vertex_corner_map
  // VPMap
  // GeomTraits::Plane_3

  // ???
  // region_primitive_map:
  // a property map filled by this function and that will contain for each region the plane (or only its orthognonal vector) estimated that approximates it.
  // Type: a class model of WritablePropertyMap with the value type of RegionMap as key and GeomTraits::Plane_3 or GeomTraits::Vector_3 as value type, GeomTraits being the type of the parameter geom_traits
  // Default: None

  
  py::class_<boost::vector_property_map<Vector_3>>(m, "Vector_vector_3_map")
    .def(py::init<>())
    ;

  m.def("Halfedge", &sm::halfedge<Sm_3>);

  m.def("null_face", &sm::null_face<Sm_3>);
  m.def("read_polygon_mesh", &sm::read_polygon_mesh<Sm_3>,
        py::arg("fname"), py::arg("parameters") = py::dict());
  m.def("read_polygon_soup", &sm::read_polygon_soup<Sm_3>,
        py::arg("fname"), py::arg("np") = py::dict());
  m.def("make_tetrahedron", &sm::make_tetrahedron<Sm_3>);
  m.def("write_polygon_mesh", &sm::write_polygon_mesh<Sm_3>,
        py::arg("fname"), py::arg("pm"), py::arg("parameters") = py::dict());
  m.def("halfedge", &sm::halfedge<Sm_3>);
  m.def("is_triangle", &sm::is_triangle<Sm_3>);
  m.def("is_triangle_mesh", &CGAL::is_triangle_mesh<Sm_3>);

  // boost
  m.def("num_vertices", &boost_utils::num_vertices<Sm_3>);
  m.def("num_edges", &boost_utils::num_edges<Sm_3>);
  m.def("degree", &boost_utils::degree_v<Sm_3>);
  m.def("degree", &boost_utils::degree_f<Sm_3>);
  m.def("out_degree", &boost_utils::out_degree<Sm_3>);
  m.def("in_degree", &boost_utils::in_degree<Sm_3>);
  m.def("source", &boost_utils::source_e<Sm_3>);
  m.def("source", &boost_utils::source_h<Sm_3>);
  m.def("target", &boost_utils::target_e<Sm_3>);
  m.def("target", &boost_utils::target_h<Sm_3>);
  m.def("vertices", &boost_utils::my_vertices<Sm_3>);
  m.def("edges", &boost_utils::my_edges<Sm_3>);
  m.def("in_edges", &boost_utils::my_in_edges<Sm_3>);
  m.def("out_edges", &boost_utils::my_out_edges<Sm_3>);
  m.def("adjacent_vertices", &boost_utils::my_adjacent_vertices<Sm_3>);
  m.def("edge", &boost_utils::edge<Sm_3>);
  m.def("next", &boost_utils::next<Sm_3>);
  m.def("prev", &boost_utils::prev<Sm_3>);
  m.def("opposite", &boost_utils::opposite<Sm_3>);
  m.def("edge", &boost_utils::edge_h<Sm_3>);
  m.def("halfedge", &boost_utils::halfedge_e<Sm_3>);
  m.def("halfedge", &boost_utils::halfedge_v<Sm_3>);
  m.def("halfedge", &boost_utils::halfedge_vv<Sm_3>);
  m.def("halfedges", &boost_utils::my_halfedges<Sm_3>);
  m.def("num_halfedges", &boost_utils::num_halfedges<Sm_3>);
  m.def("set_next", &boost_utils::set_next<Sm_3>);
  m.def("set_target", &boost_utils::set_target<Sm_3>);
  m.def("set_halfedge", &boost_utils::set_halfedge_vh<Sm_3>);
  m.def("collect_garbage", &boost_utils::my_collect_garbage<Sm_3>);
  m.def("add_edge", &boost_utils::add_edge<Sm_3>);
  m.def("halfedge", &boost_utils::halfedge_f<Sm_3>);
  m.def("face", &boost_utils::face_h<Sm_3>);
  m.def("set_face", &boost_utils::set_face<Sm_3>);
  m.def("set_halfedge", &boost_utils::set_halfedge_fh<Sm_3>);
  m.def("num_faces", &boost_utils::num_faces<Sm_3>);
  m.def("faces", &boost_utils::my_faces<Sm_3>);
  m.def("add_vertex", &boost_utils::add_vertex<Sm_3>);
  m.def("add_vertex", &boost_utils::add_vertex_p<Sm_3>);
  m.def("reserve", &boost_utils::reserve<Sm_3>);
  m.def("remove_vertex", &boost_utils::remove_vertex<Sm_3>);
  // m.def("remove_edge", &boost_utils::remove_edge_vv<Sm_3>); // vv only for sm
  m.def("remove_edge", &boost_utils::remove_edge_e<Sm_3>);
  m.def("remove_face", &boost_utils::remove_face<Sm_3>);
  m.def("remove_all_elements", &boost_utils::remove_all_elements<Sm_3>);
  m.def("add_face", &boost_utils::add_face<Sm_3>);
  // m.def("normalize_border", &boost_utils::normalize_border<Sm_3>); ???
  m.def("is_valid_vertex_descriptor", &boost_utils::my_is_valid_vertex_descriptor<Sm_3>,
        py::arg("v"), py::arg("g"), py::arg("verbose") = false);
  m.def("is_valid_halfedge_descriptor", &boost_utils::my_is_valid_halfedge_descriptor<Sm_3>,
        py::arg("h"), py::arg("g"), py::arg("verbose") = false);
  m.def("is_valid_edge_descriptor", &boost_utils::my_is_valid_edge_descriptor<Sm_3>,
        py::arg("e"), py::arg("g"), py::arg("verbose") = false);
  m.def("is_valid_face_descriptor", &boost_utils::my_is_valid_face_descriptor<Sm_3>,
        py::arg("f"), py::arg("g"), py::arg("verbose") = false);

  // helpers from helpers.h

  m.def("is_border", &boost_utils::my_is_border_h<Sm_3>,
        py::arg("hd"), py::arg("g"));
  m.def("is_border_edge", &boost_utils::my_is_border_edge<Sm_3>,
        py::arg("vd"), py::arg("g"));
  m.def("is_border", &boost_utils::my_is_border_e<Sm_3>,
        py::arg("ed"), py::arg("g"));
  m.def("is_border", &boost_utils::my_is_border_v<Sm_3>,
        py::arg("vd"), py::arg("g"));


  // iterators
  m.def("halfedges_around_face", &boost_utils::my_halfedges_around_face<Sm_3>);
  m.def("halfedges_around_target", &boost_utils::my_halfedges_around_target<Sm_3>);

}
