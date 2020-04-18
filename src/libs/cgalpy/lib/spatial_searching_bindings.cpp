// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include "CGALPY/config.hpp"
#ifdef CGALPY_SPATIAL_SEARCHING_BINDINGS
#include "CGALPY/common.hpp"

#include "CGAL/Cartesian_d.h"
#include "CGAL/Kd_tree.h"
#include <CGAL/Kd_tree_rectangle.h>
#include <CGAL/Search_traits_d.h>
#include <CGAL/K_neighbor_search.h>
#include <CGAL/Fuzzy_iso_box.h>
#include <CGAL/Euclidean_distance.h>
#include <CGAL/Fuzzy_sphere.h>
#include "CGALPY/General_distance_python.hpp"

typedef CGAL::Cartesian_d<FT> K;
typedef K::Point_d Point_d;
typedef CGAL::Search_traits_d<K, CGAL::Dimension_tag<CGALPY_DIMENSION>> Search_traits_d;
//typedef CGAL::Orthogonal_incremental_neighbor_search<Search_traits_d> Orthogonal_incremental_neighbor_search;
//typedef Orthogonal_incremental_neighbor_search::iterator NN_iterator;
//typedef Orthogonal_incremental_neighbor_search::Tree Orthogonal_incremental_neighbor_search_tree;
typedef CGAL::Kd_tree<Search_traits_d> Kd_tree;
typedef CGAL::Sliding_midpoint<Search_traits_d> Splitter;
typedef CGAL::Fuzzy_iso_box<Search_traits_d> Fuzzy_iso_box;
typedef CGAL::Fuzzy_sphere<Search_traits_d> Fuzzy_sphere;
typedef CGAL::Kd_tree_rectangle<FT, CGAL::Dimension_tag<CGALPY_DIMENSION>> Kd_tree_rectangle;
typedef CGAL::K_neighbor_search<Search_traits_d> K_neighbor_search;
typedef General_distance_python<CGAL::Dimension_tag<CGALPY_DIMENSION>, FT, Point_d, Point_d> Distance_python;
typedef CGAL::K_neighbor_search<Search_traits_d, Distance_python> K_neighbor_search_python;
typedef CGAL::Euclidean_distance<Search_traits_d> Euclidean_distance;


int get_kd_tree_dimension()
{
  return CGALPY_DIMENSION;
}

template<typename T>
size_t hash(T& immutable)
{
  std::ostringstream stream;
  stream << immutable;
  std::string s = stream.str();
  return boost::hash<std::string>()(s);
}

static Point_d* init_point_d(int d, bp::list& lst)
{
  auto begin = boost::python::stl_input_iterator<FT>(lst);
  auto end = boost::python::stl_input_iterator<FT>();
  return new Point_d(d, begin, end);
}

template <typename T>
static T* init_tree()
{
  return new T();
}

template <typename T>
static T* init_tree_from_list(bp::list& lst)
{
  auto begin = boost::python::stl_input_iterator<typename T::Point_d >(lst);
  auto end = boost::python::stl_input_iterator<typename T::Point_d >();
  return new T(begin, end);
}

template <typename T>
void tree_insert(T& tree, bp::list& lst)
{
  //copying into a vector because of an apparent bug with boost::python::stl_input_iterator
  auto begin = boost::python::stl_input_iterator<typename T::Point_d >(lst);
  auto end = boost::python::stl_input_iterator<typename T::Point_d >();
  auto v = std::vector<typename T::Point_d>(begin, end);
  tree.insert(v.begin(), v.end());
}

template <typename T, typename FQI>
void tree_search(T& tree, FQI& q, bp::list& lst)
{
  auto v = std::vector<typename T::Point_d>();
  tree.search(std::back_inserter(v), q);
  for (auto p : v)
    lst.append(p);
}

template<typename T>
void points(T& tree, bp::list& lst)
{
  for (auto p : tree)
    lst.append(p);
}

template <typename T>
void bind_kd_tree(const char* python_name)
{
  using namespace bp;
  class_<T, boost::noncopyable>(python_name)
    .def(init<>())
    .def("__init__", make_constructor(&init_tree_from_list<T>))
    .def("insert", static_cast<void (T::*) (const typename T::Point_d&)>(&T::insert))
    .def("insert", &tree_insert<T>)
    .def("remove", static_cast<void (T::*) (const typename T::Point_d&)>(&T::remove))
    .def("build", &T::build)
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
void k_neighbors(T& neighbor_search, bp::list& lst)
{
  for (auto it = neighbor_search.begin(); it != neighbor_search.end(); ++it)
  {
    lst.append(bp::make_tuple(it->first, it->second));
  }
}

template <typename T>
void bind_neighbor_search(const char* python_name)
{
  using namespace bp;
  class_<T>(python_name, init<const typename T::Tree&, typename T::Query_item,
   unsigned int, FT, bool, typename T::Distance, bool>())
    .def("k_neighbors", &k_neighbors<T>)
    ;
}

void export_spatial_searching()
{
  using namespace bp;
  class_<Point_d>("Point_d")
    .def(init<>())
    .def("__init__", make_constructor(&init_point_d))
    .def("dimension", &Point_d::dimension)
    .def("cartesian", &Point_d::cartesian)
    .def("__getitem__", &Point_d::operator[])
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .def("__hash__", &hash<Point_d>)
    ;

  class_<Fuzzy_iso_box>("Fuzzy_iso_box")
    .def(init<Fuzzy_iso_box::Point_d, Fuzzy_iso_box::Point_d>())
    .def(init<Fuzzy_iso_box::Point_d, Fuzzy_iso_box::Point_d, FT>())
    .def("contains", &Fuzzy_iso_box::contains)
    .def("inner_range_intersects", &Fuzzy_iso_box::inner_range_intersects)
    .def("outer_range_contains", &Fuzzy_iso_box::outer_range_contains)
    ;

  class_<Fuzzy_sphere>("Fuzzy_sphere")
    .def(init<Point_d, FT, FT>())
    .def("contains", &Fuzzy_sphere::contains)
    .def("inner_range_intersects", &Fuzzy_sphere::inner_range_intersects)
    .def("outer_range_intersects", &Fuzzy_sphere::outer_range_contains)
    ;

  class_<Kd_tree_rectangle>("Kd_tree_rectangle")
    .def(init<int>())
    .def("min_coord", &Kd_tree_rectangle::min_coord)
    .def("max_coord", &Kd_tree_rectangle::max_coord)
    .def("set_upper_bound", &Kd_tree_rectangle::set_upper_bound)
    .def("set_lower_bound", &Kd_tree_rectangle::set_lower_bound)
    .def("max_span_coord", &Kd_tree_rectangle::max_span_coord)
    .def("max_span", &Kd_tree_rectangle::max_span)
    .def("dimension", &Kd_tree_rectangle::dimension)
    .def("split", &Kd_tree_rectangle::split)
    ;

  bind_kd_tree<Kd_tree>("Kd_tree");

  class_<Distance_python>("Distance_python")
    .def(init<bp::object, bp::object, bp::object, bp::object, bp::object>())
    .def<FT (Distance_python::*) (const Distance_python::Query_item&, const Distance_python::Point_d&)const>("transformed_distance", &Distance_python::transformed_distance)
    .def("min_distance_to_rectangle", &Distance_python::min_distance_to_rectangle)
    .def("max_distance_to_rectangle", &Distance_python::max_distance_to_rectangle)
    .def<FT (Distance_python::*) (const FT&)const>("transformed_distance", &Distance_python::transformed_distance)
    .def("inverse_of_transformed_distance", &Distance_python::inverse_of_transformed_distance)
    ;

  class_<Euclidean_distance>("Euclidean_distance")
    .def(init<>())
    .def<FT (Euclidean_distance::*) (const Euclidean_distance::Query_item&, const Euclidean_distance::Point_d&) const>("transformed_distance", &Euclidean_distance::transformed_distance)
    .def<FT (Euclidean_distance::*) (const Euclidean_distance::Query_item&, const Kd_tree_rectangle&) const>("min_distance_to_rectangle", &Euclidean_distance::min_distance_to_rectangle)
    .def<FT (Euclidean_distance::*) (const Euclidean_distance::Query_item&, const Kd_tree_rectangle&) const>("max_distance_to_rectangle", &Euclidean_distance::max_distance_to_rectangle)
    .def<FT (Euclidean_distance::*) (FT) const>("transformed_distance", &Euclidean_distance::transformed_distance)
    //.def("inverse_of_transformed_distance", &Euclidean_distance::inverse_of_transformed_distance)
    ;

  bind_neighbor_search<K_neighbor_search_python>("K_neighbor_search_python");

  bind_neighbor_search<K_neighbor_search>("K_neighbor_search");

  def("get_kd_tree_dimension", &get_kd_tree_dimension);
}
#endif
