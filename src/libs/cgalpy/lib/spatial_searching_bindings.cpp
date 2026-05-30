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

#include "cgalpy/spatial_searching_config.hpp"
#include "cgalpy/kernel_d_types.hpp"
#include "cgalpy/General_distance_python.hpp"
#include "cgalpy/append_iterator.hpp"
#include "cgalpy/add_attr.hpp"
#include "cgalpy/stl_forward_iterator.hpp"
#include "cgalpy/Ss_docstrings.hpp"

namespace py = nanobind;
namespace ss_doc = cgalpy::ss::docstrings;

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
using Distance_python =
  General_distance_python<Dimension_tag, FT_d, Point_d, Point_d>;
using K_neighbor_search_python =
  CGAL::K_neighbor_search<Search_traits_d, Distance_python>;
using Euclidean_distance = CGAL::Euclidean_distance<Search_traits_d>;

int get_spatial_searching_dimension()
{ return CGALPY_SPATIAL_SEARCHING_DIMENSION; }

template <typename T>
static T* init_tree() { return new T(); }

template <typename T>
void init_tree_from_list(T* tree, const py::list& lst) {
  using Pntd = typename T::Point_d;
  auto begin = stl_forward_iterator<Pntd>(lst);
  auto end = stl_forward_iterator<Pntd>(lst, false);
  new (tree) T(begin, end);     // placement new
}

template <typename T>
void tree_insert(T& tree, const py::list& lst) {
  using Pntd = typename T::Point_d;
  auto begin = stl_forward_iterator<Pntd>(lst);
  auto end = stl_forward_iterator<Pntd>(lst, false);
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
  py::class_<T>(m, python_name, ss_doc::Kd_tree_class)
    .def(py::init<>(), ss_doc::Kd_tree_Kd_tree)
    .def("__init__", &init_tree_from_list<T>,
         py::arg("points"), ss_doc::Kd_tree_Kd_tree_1)
    .def("insert",
         static_cast<void(T::*)(const typename T::Point_d&)>(&T::insert),
         py::arg("p"), ss_doc::Kd_tree_insert)
    .def("insert", &tree_insert<T>,
         py::arg("points"), ss_doc::Kd_tree_insert_1)
    .def("remove",
         static_cast<void(T::*)(const typename T::Point_d&)>(&T::remove),
         py::arg("p"), ss_doc::Kd_tree_remove)
    .def("build", static_cast<void (T::*)()>(&T::build),
         ss_doc::Kd_tree_build)
    .def("invalidate_build", &T::invalidate_build,
         ss_doc::Kd_tree_invalidate_build)
    .def("points", &points<T>,
         "Return the points stored in the tree as a Python list.")
    .def("search", &tree_search<T, Fuzzy_iso_box>,
         py::arg("q"), ss_doc::Kd_tree_search)
    .def("search", &tree_search<T, Fuzzy_sphere>,
         py::arg("q"), ss_doc::Kd_tree_search)
    .def("size", &T::size,
         ss_doc::Kd_tree_size)
    .def("capacity", &T::capacity,
         ss_doc::Kd_tree_capacity)
    .def("reserve", &T::reserve,
         py::arg("n"),
         "Reserve storage for at least n points.")
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
  py::class_<T>(m, python_name, ss_doc::K_neighbor_search_class)
    .def(py::init<const typename T::Tree&, typename T::Query_item,
         unsigned int, FT_d, bool, typename T::Distance, bool>(),
         py::arg("tree"), py::arg("q"), py::arg("k"), py::arg("eps"),
         py::arg("search_nearest"), py::arg("d"), py::arg("sorted"),
         ss_doc::K_neighbor_search_K_neighbor_search)
    .def("k_neighbors", &k_neighbors<T>,
         "Return the neighbor-search result as a Python list of (point, distance) pairs.")
    ;
}

void export_spatial_searching(py::module_& m) {
  auto res = add_attr<Point_d>(m, "Point_d");
  BOOST_ASSERT(res);

  py::class_<Fuzzy_iso_box>(
      m, "Fuzzy_iso_box", ss_doc::Fuzzy_iso_box_class)
    .def(py::init<Fuzzy_iso_box::Point_d, Fuzzy_iso_box::Point_d>(),
         py::arg("p"), py::arg("q"),
         ss_doc::Fuzzy_iso_box_Fuzzy_iso_box)
    .def(py::init<Fuzzy_iso_box::Point_d, Fuzzy_iso_box::Point_d, FT_d>(),
         py::arg("p"), py::arg("q"), py::arg("epsilon"),
         ss_doc::Fuzzy_iso_box_Fuzzy_iso_box)
    .def("contains", &Fuzzy_iso_box::contains,
         py::arg("p"), ss_doc::Fuzzy_iso_box_contains)
    .def("inner_range_intersects", &Fuzzy_iso_box::inner_range_intersects,
         py::arg("rectangle"), ss_doc::Fuzzy_iso_box_inner_range_intersects)
    .def("outer_range_contains", &Fuzzy_iso_box::outer_range_contains,
         py::arg("rectangle"), ss_doc::Fuzzy_iso_box_outer_range_contains)
    ;

  py::class_<Fuzzy_sphere>(
      m, "Fuzzy_sphere", ss_doc::Fuzzy_sphere_class)
    .def(py::init<Point_d, FT_d, FT_d>(),
         py::arg("center"), py::arg("radius"), py::arg("epsilon"),
         ss_doc::Fuzzy_sphere_Fuzzy_sphere)
    .def("contains", &Fuzzy_sphere::contains,
         py::arg("p"), ss_doc::Fuzzy_sphere_contains)
    .def("inner_range_intersects", &Fuzzy_sphere::inner_range_intersects,
         py::arg("rectangle"), ss_doc::Fuzzy_sphere_inner_range_intersects)
    .def("outer_range_intersects", &Fuzzy_sphere::outer_range_contains,
         py::arg("rectangle"), ss_doc::Fuzzy_sphere_outer_range_contains)
    ;

  py::class_<Kd_tree_rectangle>(
      m, "Kd_tree_rectangle", ss_doc::Kd_tree_rectangle_class)
    .def(py::init<int>(),
         py::arg("dimension"), ss_doc::Kd_tree_rectangle_Kd_tree_rectangle)
    .def("min_coord", &Kd_tree_rectangle::min_coord,
         py::arg("i"), ss_doc::Kd_tree_rectangle_min_coord)
    .def("max_coord", &Kd_tree_rectangle::max_coord,
         py::arg("i"), ss_doc::Kd_tree_rectangle_max_coord)
    .def("set_upper_bound", &Kd_tree_rectangle::set_upper_bound,
         py::arg("i"), py::arg("x"),
         ss_doc::Kd_tree_rectangle_set_upper_bound)
    .def("set_lower_bound", &Kd_tree_rectangle::set_lower_bound,
         py::arg("i"), py::arg("x"),
         ss_doc::Kd_tree_rectangle_set_lower_bound)
    .def("max_span_coord", &Kd_tree_rectangle::max_span_coord,
         ss_doc::Kd_tree_rectangle_max_span_coord)
    .def("max_span", &Kd_tree_rectangle::max_span,
         ss_doc::Kd_tree_rectangle_max_span)
    .def("dimension", &Kd_tree_rectangle::dimension,
         ss_doc::Kd_tree_rectangle_dimension)
    .def("split", &Kd_tree_rectangle::split,
         py::arg("r"), py::arg("d"), py::arg("value"),
         ss_doc::Kd_tree_rectangle_split)
    ;

  bind_kd_tree<Kd_tree>(m, "Kd_tree");

  using Dp = Distance_python;
  py::class_<Dp>(m, "Distance_python")
    .def(py::init<py::object, py::object, py::object, py::object, py::object>(),
         py::arg("transformed_distance"),
         py::arg("min_distance_to_rectangle"),
         py::arg("max_distance_to_rectangle"),
         py::arg("transformed_distance_from_ft"),
         py::arg("inverse_of_transformed_distance"))
    .def("transformed_distance",
         py::overload_cast<const Dp::Query_item&, const Dp::Point_d&>
         (&Dp::transformed_distance, py::const_),
         py::arg("q"), py::arg("r"),
         ss_doc::GeneralDistance_transformed_distance)
    .def("min_distance_to_rectangle", &Dp::min_distance_to_rectangle,
         py::arg("q"), py::arg("r"),
         ss_doc::GeneralDistance_min_distance_to_rectangle)
    .def("max_distance_to_rectangle", &Dp::max_distance_to_rectangle,
         py::arg("q"), py::arg("r"),
         ss_doc::GeneralDistance_max_distance_to_rectangle)
    .def("transformed_distance",
         py::overload_cast<const FT_d&>(&Dp::transformed_distance, py::const_),
         py::arg("d"),
         ss_doc::GeneralDistance_transformed_distance_1)
    .def("inverse_of_transformed_distance", &Dp::inverse_of_transformed_distance,
         py::arg("d"),
         ss_doc::GeneralDistance_inverse_of_transformed_distance)
    ;

  using Ed = Euclidean_distance;
  py::class_<Ed>(m, "Euclidean_distance",
                 ss_doc::Euclidean_distance_class)
    .def(py::init<>(),
         ss_doc::Euclidean_distance_Euclidean_distance)
    .def("transformed_distance",
         py::overload_cast<const Ed::Query_item&, const Ed::Point_d&>
         (&Ed::transformed_distance, py::const_),
         py::arg("q"), py::arg("p"),
         ss_doc::Euclidean_distance_transformed_distance)
    .def("min_distance_to_rectangle",
         py::overload_cast<const Ed::Query_item&, const Kd_tree_rectangle&>
         (&Ed::min_distance_to_rectangle, py::const_),
         py::arg("q"), py::arg("r"),
         ss_doc::Euclidean_distance_min_distance_to_rectangle)
    .def("max_distance_to_rectangle",
         py::overload_cast<const Ed::Query_item&, const Kd_tree_rectangle&>
         (&Ed::max_distance_to_rectangle, py::const_),
         py::arg("q"), py::arg("r"),
         ss_doc::Euclidean_distance_max_distance_to_rectangle)
    .def("transformed_distance",
         py::overload_cast<FT_d>(&Ed::transformed_distance, py::const_),
         py::arg("d"),
         ss_doc::Euclidean_distance_transformed_distance_1)
    //.def("inverse_of_transformed_distance", &Ed::inverse_of_transformed_distance)
    ;

  bind_neighbor_search<K_neighbor_search_python>(m, "K_neighbor_search_python");

  bind_neighbor_search<K_neighbor_search>(m, "K_neighbor_search");

  m.def("get_spatial_searching_dimension", &get_spatial_searching_dimension,
        "Return the configured spatial-searching dimension.");
}
