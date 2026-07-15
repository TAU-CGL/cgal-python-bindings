// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Cartesian_d.h>
#include <CGAL/Kd_tree.h>
#include <CGAL/Kd_tree_rectangle.h>
#include <CGAL/Search_traits_d.h>
#include <CGAL/K_neighbor_search.h>
#include <CGAL/Fuzzy_iso_box.h>
#include <CGAL/Euclidean_distance.h>
#include <CGAL/Fuzzy_sphere.h>

#include "CGALPY/spatial_searching_config.hpp"
#include "CGALPY/kernel_d_types.hpp"
#include "CGALPY/General_distance_python.hpp"
#include "CGALPY/append_iterator.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/stl_forward_iterator.hpp"

namespace py = nanobind;

namespace ss {

using Search_traits_d = CGAL::Search_traits_d<Kernel_d, Dimension_tag>;
//typedef CGAL::Orthogonal_incremental_neighbor_search<Search_traits_d> Orthogonal_incremental_neighbor_search;
//typedef Orthogonal_incremental_neighbor_search::iterator NN_iterator;
//typedef Orthogonal_incremental_neighbor_search::Tree Orthogonal_incremental_neighbor_search_tree;
using Kd_tree = CGAL::Kd_tree<Search_traits_d>;
using Splitter = CGAL::Sliding_midpoint<Search_traits_d>;
using Fuzzy_iso_box = CGAL::Fuzzy_iso_box<Search_traits_d>;
using Fuzzy_sphere = CGAL::Fuzzy_sphere<Search_traits_d>;
using Kd_tree_rectangle = CGAL::Kd_tree_rectangle<FT_d, Dimension_tag>;
using K_neighbor_search = CGAL::K_neighbor_search<Search_traits_d>;
using Distance_python = General_distance_python<Dimension_tag, FT_d, Point_d, Point_d>;
using K_neighbor_search_python = CGAL::K_neighbor_search<Search_traits_d, Distance_python>;
using Euclidean_distance = CGAL::Euclidean_distance<Search_traits_d>;

}

/*!
 */
int get_spatial_searching_dimension() { return CGALPY_SPATIAL_SEARCHING_DIMENSION; }

/*!
 */
template <typename T>
static T* init_tree() { return new T(); }

/*!
 */
template <typename T>
void init_tree_from_list(T* tree, const py::list& lst) {
  using Pntd = typename T::Point_d;
  auto begin = stl_forward_iterator<Pntd>(lst);
  auto end = stl_forward_iterator<Pntd>(lst, false);
  new (tree) T(begin, end);     // placement new
}

/*!
 */
template <typename T>
void tree_insert(T& tree, const py::list& lst) {
  using Pntd = typename T::Point_d;
  auto begin = stl_forward_iterator<Pntd>(lst);
  auto end = stl_forward_iterator<Pntd>(lst, false);
  tree.insert(begin, end);
}

/*!
 */
template <typename T, typename FQI>
py::list tree_search(T& tree, FQI& q) {
  py::list lst;
  tree.search(append_iterator(lst), q);
  return lst;
}

/*!
 */
template<typename T>
py::list points(const T& tree) {
  py::list lst;
  for (auto p : tree) lst.append(p);
  return lst;
}

/*!
 */
void bind_fuzzy_iso_box(py::module_& m, const char* python_name) {
  using Fib = ss::Fuzzy_iso_box;
  if (add_attr<Fib>(m, python_name)) return;

  using Ptd = Fib::Point_d;
  py::class_<Fib>(m, "Fuzzy_iso_box")
    .def(py::init<Ptd, Ptd>())
    .def(py::init<Ptd, Ptd, FT_d>())
    .def("contains", &Fib::contains)
    .def("inner_range_intersects", &Fib::inner_range_intersects)
    .def("outer_range_contains", &Fib::outer_range_contains)
    ;
}

/*!
 */
void bind_fuzzy_sphere(py::module_& m, const char* python_name) {
  using Fs = ss::Fuzzy_sphere;
  if (add_attr<Fs>(m, python_name)) return;

  py::class_<Fs>(m, python_name)
    .def(py::init<Point_d, FT_d, FT_d>())
    .def("contains", &Fs::contains)
    .def("inner_range_intersects", &Fs::inner_range_intersects)
    .def("outer_range_intersects", &Fs::outer_range_contains)
    ;
}

/*!
 */
void bind_kd_tree_rectangle(py::module_& m, const char* python_name) {
  using Ktr = ss::Kd_tree_rectangle;
  if (add_attr<Ktr>(m, python_name)) return;

    py::class_<Ktr>(m, python_name)
      .def(py::init<int>())
      .def("min_coord", &Ktr::min_coord)
      .def("max_coord", &Ktr::max_coord)
      .def("set_upper_bound", &Ktr::set_upper_bound)
      .def("set_lower_bound", &Ktr::set_lower_bound)
      .def("max_span_coord", &Ktr::max_span_coord)
      .def("max_span", &Ktr::max_span)
      .def("dimension", &Ktr::dimension)
      .def("split", &Ktr::split)
      ;
  }

/*!
 */
void bind_kd_tree(py::module_& m, const char* python_name) {
  using Kt = ss::Kd_tree;
  if (add_attr<Kt>(m, python_name)) return;

  using Ptd = Kt::Point_d;
  py::class_<Kt>(m, python_name)
    .def(py::init<>())
    .def("__init__", &init_tree_from_list<Kt>)
    .def("insert", static_cast<void(Kt::*)(const Ptd&)>(&Kt::insert))
    .def("insert", &tree_insert<Kt>)
    .def("remove", static_cast<void(Kt::*)(const Ptd&)>(&Kt::remove))
    .def("build", static_cast<void (Kt::*)()>(&Kt::build))
    .def("invalidate_build", &Kt::invalidate_build)
    .def("points", &points<Kt>)
    .def("search", &tree_search<Kt, ss::Fuzzy_iso_box>)
    .def("search", &tree_search<Kt, ss::Fuzzy_sphere>)
    .def("size", &Kt::size)
    .def("capacity", &Kt::capacity)
    .def("reserve", &Kt::reserve)
    ;
}

/*!
 */
void bind_distance_python(py::module_& m, const char* python_name) {
  using Dp = ss::Distance_python;
  if (add_attr<Dp>(m, python_name)) return;

  using Ptd = Dp::Point_d;
  using Qi = Dp::Query_item;
  py::class_<Dp>(m, "Distance_python")
    .def(py::init<py::object, py::object, py::object, py::object, py::object>())
    .def("transformed_distance", py::overload_cast<const Qi&, const Ptd&>(&Dp::transformed_distance, py::const_))
    .def("min_distance_to_rectangle", &Dp::min_distance_to_rectangle)
    .def("max_distance_to_rectangle", &Dp::max_distance_to_rectangle)
    .def("transformed_distance", py::overload_cast<const FT_d&>(&Dp::transformed_distance, py::const_))
    .def("inverse_of_transformed_distance", &Dp::inverse_of_transformed_distance)
    ;
}

/*!
 */
void bind_euclidean_distance(py::module_& m,  const char* python_name) {
  using Ed = ss::Euclidean_distance;
  if (add_attr<Ed>(m, python_name)) return;

  using Ptd = Ed::Point_d;
  using Qi = Ed::Query_item;
  py::class_<Ed>(m, python_name)
    .def(py::init<>())
    .def("transformed_distance",
         py::overload_cast<const Qi&, const Ptd&>(&Ed::transformed_distance, py::const_))
    .def("min_distance_to_rectangle",
         py::overload_cast<const Qi&, const ss::Kd_tree_rectangle&>(&Ed::min_distance_to_rectangle, py::const_))
    .def("max_distance_to_rectangle",
         py::overload_cast<const Qi&, const ss::Kd_tree_rectangle&>(&Ed::max_distance_to_rectangle, py::const_))
    .def("transformed_distance", py::overload_cast<FT_d>(&Ed::transformed_distance, py::const_))
    //.def("inverse_of_transformed_distance", &Ed::inverse_of_transformed_distance)
    ;
}

/*!
 */
template <typename T>
py::list k_neighbors(T& neighbor_search) {
  py::list lst;
  for (auto it = neighbor_search.begin(); it != neighbor_search.end(); ++it)
    lst.append(py::make_tuple(it->first, it->second));
  return lst;
}

/*!
 */
template <typename T>
void bind_neighbor_search(py::module_& m, const char* python_name) {
  py::class_<T>(m, python_name)
    .def(py::init<const typename T::Tree&, typename T::Query_item, unsigned int, FT_d, bool,
         typename T::Distance, bool>())
    .def("k_neighbors", &k_neighbors<T>)
    ;
}

/*!
 */
void export_spatial_searching(py::module_& m) {
  auto res = add_attr<Point_d>(m, "Point_d");
  BOOST_ASSERT(res);

  bind_fuzzy_iso_box(m, "Fuzzy_iso_box");
  bind_fuzzy_sphere(m, "Fuzzy_sphere");
  bind_kd_tree_rectangle(m, "Kd_tree_rectangle");
  bind_kd_tree(m, "Kd_tree");
  bind_distance_python(m, "Distance_python");
  bind_euclidean_distance(m, "Euclidean_distance");
  bind_neighbor_search<ss::K_neighbor_search_python>(m, "K_neighbor_search_python");
  bind_neighbor_search<ss::K_neighbor_search>(m, "K_neighbor_search");

  m.def("get_spatial_searching_dimension", &get_spatial_searching_dimension);
}
