// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#include <nanobind/nanobind.h>

#include "cgalpy/spatial_searching_types.hpp"
#include "cgalpy/kernel_d_types.hpp"
#include "cgalpy/append_iterator.hpp"
#include "cgalpy/add_attr.hpp"
#include "cgalpy/stl_forward_iterator.hpp"
#include "cgalpy/Ss_docstrings.hpp"

namespace py = nanobind;
namespace ss_doc = cgalpy::ss::docstrings;

namespace cgalpy {
namespace ss {

//!
int get_spatial_searching_dimension() { return CGALPY_SPATIAL_SEARCHING_DIMENSION; }

//!
template <typename T>
static T* init_tree() { return new T(); }

//!
template <typename T>
void init_tree_from_list(T* tree, const py::list& lst) {
  using Pntd = typename T::Point_d;
  auto begin = stl_forward_iterator<Pntd>(lst);
  auto end = stl_forward_iterator<Pntd>(lst, false);
  new (tree) T(begin, end);     // placement new
}

//!
template <typename T>
void tree_insert(T& tree, const py::list& lst) {
  using Pntd = typename T::Point_d;
  auto begin = stl_forward_iterator<Pntd>(lst);
  auto end = stl_forward_iterator<Pntd>(lst, false);
  tree.insert(begin, end);
}

//!
template <typename T, typename FQI>
py::list tree_search(T& tree, FQI& q) {
  py::list lst;
  tree.search(append_iterator(lst), q);
  return lst;
}

//!
template<typename T>
py::list points(const T& tree) {
  py::list lst;
  for (auto p : tree) lst.append(p);
  return lst;
}

}
}

namespace ss_code = cgalpy::ss;

//!
template <typename T>
void bind_kd_tree(py::module_& m, const char* python_name) {
  using Fib = ss_code::Fuzzy_iso_box;
  using Fs = ss_code::Fuzzy_sphere;

  py::class_<T>(m, python_name, ss_doc::Kd_tree_class)
    .def(py::init<>(), ss_doc::Kd_tree_Kd_tree)
    .def("__init__", &ss_code::init_tree_from_list<T>, py::arg("points"), ss_doc::Kd_tree_Kd_tree_1)
#if (CGAL_VERSION_NR > 1060200910) || (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D)
    .def("insert", static_cast<void(T::*)(const typename T::Point_d&)>(&T::insert),
         py::arg("p"), ss_doc::Kd_tree_insert)
    .def("insert", &ss_code::tree_insert<T>,
         py::arg("points"), ss_doc::Kd_tree_insert_1)
    .def("remove", static_cast<void(T::*)(const typename T::Point_d&)>(&T::remove),
         py::arg("p"), ss_doc::Kd_tree_remove)
    .def("build", static_cast<void (T::*)()>(&T::build), ss_doc::Kd_tree_build)
    .def("invalidate_build", &T::invalidate_build, ss_doc::Kd_tree_invalidate_build)
    .def("search", &ss_code::tree_search<T, Fib>, py::arg("q"), ss_doc::Kd_tree_search)
    .def("search", &ss_code::tree_search<T, Fs>, py::arg("q"), ss_doc::Kd_tree_search)
#endif

    .def("points", &ss_code::points<T>, "Return the points stored in the tree.")
    .def("size", &T::size, ss_doc::Kd_tree_size)
    .def("capacity", &T::capacity, ss_doc::Kd_tree_capacity)
    .def("reserve", &T::reserve, py::arg("n"), "Reserve storage for at least n points.")
    ;
}

//!
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
    .def(py::init<const typename T::Tree&, typename T::Query_item, unsigned int>(),
         py::arg("tree"), py::arg("q"), py::arg("k"),
         ss_doc::K_neighbor_search_K_neighbor_search)
    .def("k_neighbors", &k_neighbors<T>,
         "Return the neighbor-search result as a Python list of (point, distance) pairs.")
    ;
}

//!
void export_spatial_searching(py::module_& m) {
  // Fuzzy_iso_box
  using Fib = ss_code::Fuzzy_iso_box;
  using Fib_point_d = Fib::Point_d;
  py::class_<Fib> fib_c(m, "Fuzzy_iso_box", ss_doc::Fuzzy_iso_box_class);
  fib_c

    // The following fails to compile due to a bug in CGAL; Comment out until the bug is fixed.
#if (CGAL_VERSION_NR > 1060200910) || (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D)
    .def(py::init<Fib_point_d, Fib_point_d>(),
         py::arg("p"), py::arg("q"),
         ss_doc::Fuzzy_iso_box_Fuzzy_iso_box)
    .def(py::init<Fib_point_d, Fib_point_d, FT_d>(),
         py::arg("p"), py::arg("q"), py::arg("epsilon"),
         ss_doc::Fuzzy_iso_box_Fuzzy_iso_box)
#endif

    .def("contains", &Fib::contains,
         py::arg("p"), ss_doc::Fuzzy_iso_box_contains)
    .def("inner_range_intersects", &Fib::inner_range_intersects,
         py::arg("rectangle"), ss_doc::Fuzzy_iso_box_inner_range_intersects)
    .def("outer_range_contains", &Fib::outer_range_contains,
         py::arg("rectangle"), ss_doc::Fuzzy_iso_box_outer_range_contains)
    ;
  auto res_fib = add_attr<Fib_point_d>(fib_c, "Point_d");
  BOOST_ASSERT(res);

  // Fuzzy_sphere
  using Fs = ss_code::Fuzzy_sphere;
  // The following is missing (CGAL bug)
  // using Fs_point_d = Fs::Point_d;
  using Fs_point_d = ss_code::Search_traits_d::Point_d;
  py::class_<Fs> fs_c(m, "Fuzzy_sphere", ss_doc::Fuzzy_sphere_class);
  fs_c
    .def(py::init<Fs_point_d, FT_d, FT_d>(),
         py::arg("center"), py::arg("radius"), py::arg("epsilon"),
         ss_doc::Fuzzy_sphere_Fuzzy_sphere)
    .def("contains", &Fs::contains,
         py::arg("p"), ss_doc::Fuzzy_sphere_contains)
    .def("inner_range_intersects", &Fs::inner_range_intersects,
         py::arg("rectangle"), ss_doc::Fuzzy_sphere_inner_range_intersects)
    .def("outer_range_intersects", &Fs::outer_range_contains,
         py::arg("rectangle"), ss_doc::Fuzzy_sphere_outer_range_contains)
    ;
  auto res_fs = add_attr<Fs_point_d>(fs_c, "Point_d");

  // Kd_tree_rectangle
  using Kdtr = ss_code::Kd_tree_rectangle;
  py::class_<Kdtr>(m, "Kd_tree_rectangle", ss_doc::Kd_tree_rectangle_class)
    .def(py::init<int>(),
         py::arg("dimension"), ss_doc::Kd_tree_rectangle_Kd_tree_rectangle)
    .def("min_coord", &Kdtr::min_coord,
         py::arg("i"), ss_doc::Kd_tree_rectangle_min_coord)
    .def("max_coord", &Kdtr::max_coord,
         py::arg("i"), ss_doc::Kd_tree_rectangle_max_coord)
    .def("set_upper_bound", &Kdtr::set_upper_bound,
         py::arg("i"), py::arg("x"),
         ss_doc::Kd_tree_rectangle_set_upper_bound)
    .def("set_lower_bound", &Kdtr::set_lower_bound,
         py::arg("i"), py::arg("x"),
         ss_doc::Kd_tree_rectangle_set_lower_bound)
    .def("max_span_coord", &Kdtr::max_span_coord,
         ss_doc::Kd_tree_rectangle_max_span_coord)
    .def("max_span", &Kdtr::max_span,
         ss_doc::Kd_tree_rectangle_max_span)
    .def("dimension", &Kdtr::dimension,
         ss_doc::Kd_tree_rectangle_dimension)
    .def("split", &Kdtr::split,
         py::arg("r"), py::arg("d"), py::arg("value"),
         ss_doc::Kd_tree_rectangle_split)
    ;

  // Kd_tree
  bind_kd_tree<ss_code::Kd_tree>(m, "Kd_tree");

  // Distance_python
  using Dp = ss_code::Distance_python;
  py::class_<Dp>(m, "Distance_python")
    .def(py::init<py::object, py::object, py::object, py::object, py::object>(),
         py::arg("transformed_distance"),
         py::arg("min_distance_to_rectangle"),
         py::arg("max_distance_to_rectangle"),
         py::arg("transformed_distance_from_ft"),
         py::arg("inverse_of_transformed_distance"))
    .def("transformed_distance",
         py::overload_cast<const Dp::Query_item&, const Dp::Point_d&>(&Dp::transformed_distance, py::const_),
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

  // Euclidean_distance
  using Ed = ss_code::Euclidean_distance;
  py::class_<Ed>(m, "Euclidean_distance", ss_doc::Euclidean_distance_class)
    .def(py::init<>(),
         ss_doc::Euclidean_distance_Euclidean_distance)
    .def("transformed_distance",
         py::overload_cast<const Ed::Query_item&, const Ed::Point_d&>(&Ed::transformed_distance, py::const_),
         py::arg("q"), py::arg("p"),
         ss_doc::Euclidean_distance_transformed_distance)
    .def("min_distance_to_rectangle",
         py::overload_cast<const Ed::Query_item&, const Kdtr&>(&Ed::min_distance_to_rectangle, py::const_),
         py::arg("q"), py::arg("r"),
         ss_doc::Euclidean_distance_min_distance_to_rectangle)
    .def("max_distance_to_rectangle",
         py::overload_cast<const Ed::Query_item&, const Kdtr&>(&Ed::max_distance_to_rectangle, py::const_),
         py::arg("q"), py::arg("r"),
         ss_doc::Euclidean_distance_max_distance_to_rectangle)
    .def("transformed_distance",
         py::overload_cast<FT_d>(&Ed::transformed_distance, py::const_),
         py::arg("d"),
         ss_doc::Euclidean_distance_transformed_distance_1)
    // .def("inverse_of_transformed_distance", &Ed::inverse_of_transformed_distance)
    ;

  bind_neighbor_search<ss_code::K_neighbor_search_python>(m, "K_neighbor_search_python");

  bind_neighbor_search<ss_code::K_neighbor_search>(m, "K_neighbor_search");

  m.def("get_spatial_searching_dimension", &ss_code::get_spatial_searching_dimension,
        "Return the configured spatial-searching dimension.");
}
