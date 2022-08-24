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
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

typedef CGAL::Search_traits_d<Kernel_d, Dimension_tag>  Search_traits_d;
//typedef CGAL::Orthogonal_incremental_neighbor_search<Search_traits_d> Orthogonal_incremental_neighbor_search;
//typedef Orthogonal_incremental_neighbor_search::iterator NN_iterator;
//typedef Orthogonal_incremental_neighbor_search::Tree Orthogonal_incremental_neighbor_search_tree;
typedef CGAL::Kd_tree<Search_traits_d>                  Kd_tree;
typedef CGAL::Sliding_midpoint<Search_traits_d>         Splitter;
typedef CGAL::Fuzzy_iso_box<Search_traits_d>            Fuzzy_iso_box;
typedef CGAL::Fuzzy_sphere<Search_traits_d>             Fuzzy_sphere;
typedef CGAL::Kd_tree_rectangle<FT_d, Dimension_tag>    Kd_tree_rectangle;
typedef CGAL::K_neighbor_search<Search_traits_d>        K_neighbor_search;
typedef General_distance_python<Dimension_tag, FT_d, Point_d, Point_d>
                                                        Distance_python;
typedef CGAL::K_neighbor_search<Search_traits_d, Distance_python>
                                                        K_neighbor_search_python;
typedef CGAL::Euclidean_distance<Search_traits_d>       Euclidean_distance;

int get_spatial_searching_dimension()
{ return CGALPY_SPATIAL_SEARCHING_DIMENSION; }

template <typename T>
static T* init_tree() { return new T(); }

template <typename T>
void init_tree_from_list(T& tree, const py::list& lst) {
  auto begin = stl_input_iterator<typename T::Point_d>(lst);
  auto end = stl_input_iterator<typename T::Point_d>(lst, false);
  new (&tree) T(begin, end);
}

template <typename T>
void tree_insert(T& tree, const py::list& lst) {
  auto begin = stl_input_iterator<typename T::Point_d>(lst);
  auto end = stl_input_iterator<typename T::Point_d>(lst, false);
  tree.insert(begin, end);
}

template <typename T, typename FQI>
py::list tree_search(T& tree, FQI& q) {
  py::list lst;
  tree.search(append_iterator(lst), q);
  return lst;
}

template<typename T>
py::list points(const T& tree) {
  py::list lst;
  for (auto p : tree) lst.append(p);
  return lst;
}

template <typename T>
void bind_kd_tree(py::module_& m, const char* python_name) {
  py::class_<T>(m, python_name)
    .def(py::init<>())
    .def("__init__", &init_tree_from_list<T>)
    .def("insert", static_cast<void(T::*)(const typename T::Point_d&)>(&T::insert))
    .def("insert", &tree_insert<T>)
    .def("remove", static_cast<void(T::*)(const typename T::Point_d&)>(&T::remove))
    .def("build", static_cast<void (T::*)()>(&T::build))
    .def("invalidate_build", &T::invalidate_build)
    .def("points", &points<T>)
    .def("search", &tree_search<T, Fuzzy_iso_box>)
    .def("search", &tree_search<T, Fuzzy_sphere>)
    .def("size", &T::size)
    .def("capacity", &T::capacity)
    .def("reserve", &T::reserve)
    ;
}

template <typename T>
py::list k_neighbors(T& neighbor_search) {
  py::list lst;
  for (auto it = neighbor_search.begin(); it != neighbor_search.end(); ++it)
    lst.append(py::make_tuple(it->first, it->second));
  return lst;
}

template <typename T>
void bind_neighbor_search(py::module_& m, const char* python_name) {
  py::class_<T>(m, python_name)
    .def(py::init<const typename T::Tree&, typename T::Query_item,
         unsigned int, FT_d, bool, typename T::Distance, bool>())
    .def("k_neighbors", &k_neighbors<T>)
    ;
}

void export_spatial_searching(py::module_& m) {
  auto res = add_attr<Point_d>("Point_d", m);
  BOOST_ASSERT(res);

  py::class_<Fuzzy_iso_box>(m, "Fuzzy_iso_box")
    .def(py::init<Fuzzy_iso_box::Point_d, Fuzzy_iso_box::Point_d>())
    .def(py::init<Fuzzy_iso_box::Point_d, Fuzzy_iso_box::Point_d, FT_d>())
    .def("contains", &Fuzzy_iso_box::contains)
    .def("inner_range_intersects", &Fuzzy_iso_box::inner_range_intersects)
    .def("outer_range_contains", &Fuzzy_iso_box::outer_range_contains)
    ;

  py::class_<Fuzzy_sphere>(m, "Fuzzy_sphere")
    .def(py::init<Point_d, FT_d, FT_d>())
    .def("contains", &Fuzzy_sphere::contains)
    .def("inner_range_intersects", &Fuzzy_sphere::inner_range_intersects)
    .def("outer_range_intersects", &Fuzzy_sphere::outer_range_contains)
    ;

  py::class_<Kd_tree_rectangle>(m, "Kd_tree_rectangle")
    .def(py::init<int>())
    .def("min_coord", &Kd_tree_rectangle::min_coord)
    .def("max_coord", &Kd_tree_rectangle::max_coord)
    .def("set_upper_bound", &Kd_tree_rectangle::set_upper_bound)
    .def("set_lower_bound", &Kd_tree_rectangle::set_lower_bound)
    .def("max_span_coord", &Kd_tree_rectangle::max_span_coord)
    .def("max_span", &Kd_tree_rectangle::max_span)
    .def("dimension", &Kd_tree_rectangle::dimension)
    .def("split", &Kd_tree_rectangle::split)
    ;

  bind_kd_tree<Kd_tree>(m, "Kd_tree");

  py::class_<Distance_python>(m, "Distance_python")
    .def(py::init<py::object, py::object, py::object, py::object, py::object>())
    .def<FT_d (Distance_python::*) (const Distance_python::Query_item&, const Distance_python::Point_d&)const>("transformed_distance", &Distance_python::transformed_distance)
    .def("min_distance_to_rectangle", &Distance_python::min_distance_to_rectangle)
    .def("max_distance_to_rectangle", &Distance_python::max_distance_to_rectangle)
    .def<FT_d (Distance_python::*) (const FT_d&)const>("transformed_distance", &Distance_python::transformed_distance)
    .def("inverse_of_transformed_distance", &Distance_python::inverse_of_transformed_distance)
    ;

  py::class_<Euclidean_distance>(m, "Euclidean_distance")
    .def(py::init<>())
    .def<FT_d (Euclidean_distance::*) (const Euclidean_distance::Query_item&, const Euclidean_distance::Point_d&) const>("transformed_distance", &Euclidean_distance::transformed_distance)
    .def<FT_d (Euclidean_distance::*) (const Euclidean_distance::Query_item&, const Kd_tree_rectangle&) const>("min_distance_to_rectangle", &Euclidean_distance::min_distance_to_rectangle)
    .def<FT_d (Euclidean_distance::*) (const Euclidean_distance::Query_item&, const Kd_tree_rectangle&) const>("max_distance_to_rectangle", &Euclidean_distance::max_distance_to_rectangle)
    .def<FT_d (Euclidean_distance::*) (FT_d) const>("transformed_distance", &Euclidean_distance::transformed_distance)
    //.def("inverse_of_transformed_distance", &Euclidean_distance::inverse_of_transformed_distance)
    ;

  bind_neighbor_search<K_neighbor_search_python>(m, "K_neighbor_search_python");

  bind_neighbor_search<K_neighbor_search>(m, "K_neighbor_search");

  m.def("get_spatial_searching_dimension", &get_spatial_searching_dimension);
}
